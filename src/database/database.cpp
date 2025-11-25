#include "database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QDateTime>
#include <QVariant>

Database::Database(QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("campus_jobs.db");
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool Database::connectToDatabase()
{
    if (!db.open())
    {
        qDebug() << "Error connecting to database:" << db.lastError().text();
        return false;
    }
    return createTables();
}

bool Database::createTables()
{
    QSqlQuery query;

    // Create users table with new fields
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "full_name TEXT NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "emplid TEXT NOT NULL,"
        "major TEXT,"
        "gpa TEXT,"
        "grad_date TEXT,"
        "transcript_path TEXT,"
        "cv_path TEXT,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")");
    if (!success)
    {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    // Create documents table
    success = query.exec(
        "CREATE TABLE IF NOT EXISTS documents ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "document_type TEXT NOT NULL,"
        "file_path TEXT NOT NULL,"
        "upload_date TEXT NOT NULL,"
        "status TEXT DEFAULT 'Pending',"
        "notes TEXT,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ")");
    if (!success)
    {
        qDebug() << "Error creating documents table:" << query.lastError().text();
        return false;
    }

    // Create index on user_id for faster queries
    query.exec("CREATE INDEX IF NOT EXISTS idx_documents_user_id ON documents(user_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_documents_status ON documents(status)");

    // Ensure jobs table exists (legacy minimal schema may exist in older DBs)
    success = query.exec(
        "CREATE TABLE IF NOT EXISTS jobs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "department TEXT NOT NULL,"
        "category TEXT DEFAULT '',"
        "description TEXT NOT NULL,"
        "pay_rate_min REAL DEFAULT 0,"
        "pay_rate_max REAL DEFAULT 0,"
        "hours_per_week INTEGER DEFAULT 0,"
        "positions_available INTEGER DEFAULT 0,"
        "status INTEGER DEFAULT 2,"
        "work_study_eligible INTEGER DEFAULT 0,"
        "international_eligible INTEGER DEFAULT 0,"
        "deadline TEXT DEFAULT '',"
        "requirements TEXT DEFAULT '',"
        "duties TEXT DEFAULT '',"
        "skills TEXT DEFAULT '',"
        "supervisor_info TEXT DEFAULT '',"
        "contact_email TEXT DEFAULT '',"
        "schedule TEXT DEFAULT ''"
        ")");
    if (!success)
    {
        qDebug() << "Error creating jobs table:" << query.lastError().text();
        return false;
    }

    // Add international_eligible column if missing (for upgrades)
    {
        QSqlQuery pragma;
        bool hasIntl = false;
        if (pragma.exec("PRAGMA table_info(jobs)"))
        {
            while (pragma.next())
            {
                if (pragma.value(1).toString() == "international_eligible")
                {
                    hasIntl = true;
                    break;
                }
            }
        }
        if (!hasIntl)
        {
            QSqlQuery alter;
            alter.exec("ALTER TABLE jobs ADD COLUMN international_eligible INTEGER DEFAULT 0");
        }
    }

    // Insert sample jobs if table is empty (legacy minimal sample)
    query.exec("SELECT COUNT(*) FROM jobs");
    if (query.next() && query.value(0).toInt() == 0)
    {
        success = query.exec(
            "INSERT INTO jobs (title, department, description, pay_rate) VALUES "
            "('Student Assistant', 'Computer Science', 'Help with lab sessions', 15.00),"
            "('Library Assistant', 'Library', 'Help students find resources', 16.00),"
            "('IT Support', 'IT Services', 'Provide technical support', 17.50)");
        if (!success)
        {
            qDebug() << "Error inserting sample jobs:" << query.lastError().text();
            return false;
        }
    }

    return true;
}

bool Database::validateLogin(const QString &email, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE email = ?");
    query.addBindValue(email);

    if (!query.exec() || !query.next())
    {
        return false;
    }

    QString hashedPassword = QString(QCryptographicHash::hash(
                                         password.toUtf8(), QCryptographicHash::Sha256)
                                         .toHex());
    return query.value(0).toString() == hashedPassword;
}

bool Database::registerUser(const QString &fullName, const QString &password,
                            const QString &email, const QString &emplid,
                            const QString &major, const QString &gpa,
                            const QString &gradDate)
{
    QString hashedPassword = QString(QCryptographicHash::hash(
                                         password.toUtf8(), QCryptographicHash::Sha256)
                                         .toHex());

    QSqlQuery query;
    query.prepare("INSERT INTO users (full_name, email, password, emplid, major, gpa, grad_date) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(fullName);
    query.addBindValue(email);
    query.addBindValue(hashedPassword);
    query.addBindValue(emplid);
    query.addBindValue(major);
    query.addBindValue(gpa);
    query.addBindValue(gradDate);

    bool success = query.exec();
    if (!success)
    {
        qDebug() << "Error registering user:" << query.lastError().text();
    }
    return success;
}

std::vector<Job> Database::getJobs()
{
    std::vector<Job> jobs;
    QSqlQuery query;

    if (query.exec("SELECT * FROM jobs ORDER BY status, title"))
    {
        while (query.next())
        {
            JobStatus status = static_cast<JobStatus>(query.value("status").toInt());
            jobs.emplace_back(
                query.value("id").toInt(),
                query.value("title").toString(),
                query.value("department").toString(),
                query.value("category").toString(),
                query.value("description").toString(),
                query.value("pay_rate_min").toDouble(),
                query.value("pay_rate_max").toDouble(),
                query.value("hours_per_week").toInt(),
                query.value("positions_available").toInt(),
                status,
                query.value("work_study_eligible").toBool(),
                query.value("international_eligible").toBool(),
                query.value("deadline").toString(),
                query.value("requirements").toString(),
                query.value("duties").toString(),
                query.value("skills").toString(),
                query.value("supervisor_info").toString(),
                query.value("contact_email").toString(),
                query.value("schedule").toString());
        }
    }

    return jobs;
}

User Database::getUserData(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE email = ?");
    query.addBindValue(email);

    if (query.exec() && query.next())
    {
        return User(
            query.value("id").toInt(),
            query.value("email").toString(),
            query.value("full_name").toString(),
            query.value("emplid").toString(),
            query.value("major").toString(),
            query.value("gpa").toString(),
            query.value("grad_date").toString());
    }

    return User();
}

User Database::getUserDataById(int userId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = ?");
    query.addBindValue(userId);

    if (query.exec() && query.next())
    {
        return User(
            query.value("id").toInt(),
            query.value("email").toString(),
            query.value("full_name").toString(),
            query.value("emplid").toString(),
            query.value("major").toString(),
            query.value("gpa").toString(),
            query.value("grad_date").toString());
    }

    return User();
}

int Database::getUserIdByEmail(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE email = ?");
    query.addBindValue(email);

    if (query.exec() && query.next())
    {
        return query.value("id").toInt();
    }

    return -1;
}

bool Database::updateProfile(const QString &email, const QString &major,
                             const QString &gpa, const QString &gradDate)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET major = ?, gpa = ?, grad_date = ? WHERE email = ?");
    query.addBindValue(major);
    query.addBindValue(gpa);
    query.addBindValue(gradDate);
    query.addBindValue(email);

    bool success = query.exec();
    if (!success)
    {
        qDebug() << "Error updating profile:" << query.lastError().text();
    }
    return success;
}

bool Database::updateProfileById(int userId, const QString &major,
                                 const QString &gpa, const QString &gradDate)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET major = ?, gpa = ?, grad_date = ? WHERE id = ?");
    query.addBindValue(major);
    query.addBindValue(gpa);
    query.addBindValue(gradDate);
    query.addBindValue(userId);

    bool success = query.exec();
    if (!success)
    {
        qDebug() << "Error updating profile by ID:" << query.lastError().text();
    }
    return success;
}

// Document Management Methods

bool Database::addDocument(int userId, const QString &documentType,
                           const QString &filePath, const QString &status)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO documents (user_id, document_type, file_path, upload_date, status) "
        "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(documentType);
    query.addBindValue(filePath);
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    query.addBindValue(status);

    bool success = query.exec();
    if (!success)
    {
        qDebug() << "Error adding document:" << query.lastError().text();
    }
    return success;
}

bool Database::updateDocumentStatus(int documentId, const QString &status)
{
    QSqlQuery query;
    query.prepare("UPDATE documents SET status = ? WHERE id = ?");
    query.addBindValue(status);
    query.addBindValue(documentId);

    bool success = query.exec();
    if (!success)
    {
        qDebug() << "Error updating document status:" << query.lastError().text();
    }
    return success;
}

bool Database::deleteDocument(int documentId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM documents WHERE id = ?");
    query.addBindValue(documentId);

    bool success = query.exec();
    if (!success)
    {
        qDebug() << "Error deleting document:" << query.lastError().text();
    }
    return success;
}

std::vector<Document> Database::getUserDocuments(int userId)
{
    std::vector<Document> documents;
    QSqlQuery query;

    query.prepare(
        "SELECT id, user_id, document_type, file_path, upload_date, status "
        "FROM documents WHERE user_id = ? ORDER BY upload_date DESC");
    query.addBindValue(userId);

    if (query.exec())
    {
        while (query.next())
        {
            documents.emplace_back(
                query.value("id").toInt(),
                query.value("user_id").toInt(),
                query.value("document_type").toString(),
                query.value("file_path").toString(),
                query.value("upload_date").toString(),
                query.value("status").toString());
        }
    }
    else
    {
        qDebug() << "Error retrieving user documents:" << query.lastError().text();
    }

    return documents;
}

Document Database::getLatestDocument(int userId, const QString &documentType)
{
    QSqlQuery query;
    query.prepare(
        "SELECT id, user_id, document_type, file_path, upload_date, status "
        "FROM documents WHERE user_id = ? AND document_type = ? "
        "ORDER BY upload_date DESC LIMIT 1");
    query.addBindValue(userId);
    query.addBindValue(documentType);

    if (query.exec() && query.next())
    {
        return Document(
            query.value("id").toInt(),
            query.value("user_id").toInt(),
            query.value("document_type").toString(),
            query.value("file_path").toString(),
            query.value("upload_date").toString(),
            query.value("status").toString());
    }

    return Document();
}

int Database::getDocumentCountByStatus(int userId, const QString &status)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM documents WHERE user_id = ? AND status = ?");
    query.addBindValue(userId);
    query.addBindValue(status);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

bool Database::hasDocument(int userId, const QString &documentType)
{
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM documents WHERE user_id = ? AND document_type = ? "
        "AND status != 'Deleted'");
    query.addBindValue(userId);
    query.addBindValue(documentType);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt() > 0;
    }

    return false;
}

// Job management methods
std::vector<Job> Database::getJobsByCategory(const QString &category)
{
    std::vector<Job> jobs;
    QSqlQuery query;
    query.prepare("SELECT * FROM jobs WHERE category = ? ORDER BY title");
    query.addBindValue(category);

    if (query.exec())
    {
        while (query.next())
        {
            JobStatus status = static_cast<JobStatus>(query.value("status").toInt());
            jobs.emplace_back(
                query.value("id").toInt(),
                query.value("title").toString(),
                query.value("department").toString(),
                query.value("category").toString(),
                query.value("description").toString(),
                query.value("pay_rate_min").toDouble(),
                query.value("pay_rate_max").toDouble(),
                query.value("hours_per_week").toInt(),
                query.value("positions_available").toInt(),
                status,
                query.value("work_study_eligible").toBool(),
                query.value("international_eligible").toBool(),
                query.value("deadline").toString(),
                query.value("requirements").toString(),
                query.value("duties").toString(),
                query.value("skills").toString(),
                query.value("supervisor_info").toString(),
                query.value("contact_email").toString(),
                query.value("schedule").toString());
        }
    }
    return jobs;
}

std::vector<Job> Database::searchJobs(const QString &keyword, const QString &statusFilter,
                                      const QString &categoryFilter, bool workStudyOnly,
                                      int minHours, int maxHours, double minPay, double maxPay)
{
    std::vector<Job> jobs;
    QString queryStr = "SELECT * FROM jobs WHERE 1=1";

    if (!keyword.isEmpty())
    {
        queryStr += " AND (title LIKE '%" + keyword + "%' OR department LIKE '%" + keyword + "%' OR description LIKE '%" + keyword + "%')";
    }
    if (!statusFilter.isEmpty())
    {
        queryStr += " AND status = " + statusFilter;
    }
    if (!categoryFilter.isEmpty())
    {
        queryStr += " AND category = '" + categoryFilter + "'";
    }
    if (workStudyOnly)
    {
        queryStr += " AND work_study_eligible = 1";
    }
    if (minHours > 0 || maxHours < 40)
    {
        queryStr += QString(" AND hours_per_week >= %1 AND hours_per_week <= %2").arg(minHours).arg(maxHours);
    }
    if (minPay > 0 || maxPay < 100)
    {
        queryStr += QString(" AND pay_rate_min >= %1 AND pay_rate_max <= %2").arg(minPay).arg(maxPay);
    }

    queryStr += " ORDER BY status, title";

    QSqlQuery query;
    if (query.exec(queryStr))
    {
        while (query.next())
        {
            JobStatus status = static_cast<JobStatus>(query.value("status").toInt());
            jobs.emplace_back(
                query.value("id").toInt(),
                query.value("title").toString(),
                query.value("department").toString(),
                query.value("category").toString(),
                query.value("description").toString(),
                query.value("pay_rate_min").toDouble(),
                query.value("pay_rate_max").toDouble(),
                query.value("hours_per_week").toInt(),
                query.value("positions_available").toInt(),
                status,
                query.value("work_study_eligible").toBool(),
                query.value("international_eligible").toBool(),
                query.value("deadline").toString(),
                query.value("requirements").toString(),
                query.value("duties").toString(),
                query.value("skills").toString(),
                query.value("supervisor_info").toString(),
                query.value("contact_email").toString(),
                query.value("schedule").toString());
        }
    }
    return jobs;
}

Job Database::getJobById(int jobId)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM jobs WHERE id = ?");
    query.addBindValue(jobId);

    if (query.exec() && query.next())
    {
        JobStatus status = static_cast<JobStatus>(query.value("status").toInt());
        return Job(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("department").toString(),
            query.value("category").toString(),
            query.value("description").toString(),
            query.value("pay_rate_min").toDouble(),
            query.value("pay_rate_max").toDouble(),
            query.value("hours_per_week").toInt(),
            query.value("positions_available").toInt(),
            status,
            query.value("work_study_eligible").toBool(),
            query.value("international_eligible").toBool(),
            query.value("deadline").toString(),
            query.value("requirements").toString(),
            query.value("duties").toString(),
            query.value("skills").toString(),
            query.value("supervisor_info").toString(),
            query.value("contact_email").toString(),
            query.value("schedule").toString());
    }
    return Job();
}

int Database::getJobCountByCategory(const QString &category)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM jobs WHERE category = ?");
    query.addBindValue(category);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt();
    }
    return 0;
}

bool Database::applyForJob(int userId, int jobId, const QString &resumePath,
                           const QString &coverLetterPath, const QString &whyInterested,
                           const QString &availability, const QString &references)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO job_applications (user_id, job_id, resume_path, cover_letter_path, "
        "why_interested, availability, references, status, applied_date) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, 'Pending', datetime('now'))");
    query.addBindValue(userId);
    query.addBindValue(jobId);
    query.addBindValue(resumePath);
    query.addBindValue(coverLetterPath);
    query.addBindValue(whyInterested);
    query.addBindValue(availability);
    query.addBindValue(references);

    return query.exec();
}

bool Database::expressInterest(int userId, int jobId, const QString &message)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO job_interests (user_id, job_id, message, date) "
        "VALUES (?, ?, ?, datetime('now'))");
    query.addBindValue(userId);
    query.addBindValue(jobId);
    query.addBindValue(message);

    return query.exec();
}

bool Database::saveJob(int userId, int jobId)
{
    QSqlQuery query;
    query.prepare(
        "INSERT OR IGNORE INTO saved_jobs (user_id, job_id, saved_date) "
        "VALUES (?, ?, datetime('now'))");
    query.addBindValue(userId);
    query.addBindValue(jobId);

    return query.exec();
}

bool Database::unsaveJob(int userId, int jobId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM saved_jobs WHERE user_id = ? AND job_id = ?");
    query.addBindValue(userId);
    query.addBindValue(jobId);

    return query.exec();
}

std::vector<int> Database::getSavedJobs(int userId)
{
    std::vector<int> jobIds;
    QSqlQuery query;
    query.prepare("SELECT job_id FROM saved_jobs WHERE user_id = ?");
    query.addBindValue(userId);

    if (query.exec())
    {
        while (query.next())
        {
            jobIds.push_back(query.value(0).toInt());
        }
    }
    return jobIds;
}

bool Database::isJobSaved(int userId, int jobId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM saved_jobs WHERE user_id = ? AND job_id = ?");
    query.addBindValue(userId);
    query.addBindValue(jobId);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt() > 0;
    }
    return false;
}