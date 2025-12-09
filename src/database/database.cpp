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
        "is_international_student INTEGER DEFAULT 0,"
        "survey_completed INTEGER DEFAULT 0,"
        "resume_path TEXT,"
        "parsed_resume_data TEXT,"
        "parsed_gpa TEXT,"
        "parsed_courses TEXT,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")");
    if (!success)
    {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    // Add new columns if upgrading from old schema
    {
        QSqlQuery pragma;
        QStringList existingColumns;
        if (pragma.exec("PRAGMA table_info(users)"))
        {
            while (pragma.next())
            {
                existingColumns << pragma.value(1).toString();
            }
        }

        if (!existingColumns.contains("is_international_student"))
        {
            query.exec("ALTER TABLE users ADD COLUMN is_international_student INTEGER DEFAULT 0");
        }
        if (!existingColumns.contains("survey_completed"))
        {
            query.exec("ALTER TABLE users ADD COLUMN survey_completed INTEGER DEFAULT 0");
        }
        if (!existingColumns.contains("resume_path"))
        {
            query.exec("ALTER TABLE users ADD COLUMN resume_path TEXT");
        }
        if (!existingColumns.contains("parsed_resume_data"))
        {
            query.exec("ALTER TABLE users ADD COLUMN parsed_resume_data TEXT");
        }
        if (!existingColumns.contains("parsed_gpa"))
        {
            query.exec("ALTER TABLE users ADD COLUMN parsed_gpa TEXT");
        }
        if (!existingColumns.contains("parsed_courses"))
        {
            query.exec("ALTER TABLE users ADD COLUMN parsed_courses TEXT");
        }
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

    // Insert test user if users table is empty
    query.exec("SELECT COUNT(*) FROM users");
    if (query.next() && query.value(0).toInt() == 0)
    {
        // SHA256 hash of "test123"
        success = query.exec(
            "INSERT INTO users (full_name, email, password, emplid, major, gpa, grad_date) VALUES "
            "('Test Student', 'test@stu.bmcc.cuny.edu', 'ecd71870d1963316a97e3ac3408c9835ad8cf0f3c1bc703527c30265534f75ae', '12345678', 'Computer Science', '3.8', '2026-05-15')");
        if (!success)
        {
            qDebug() << "Error inserting test user:" << query.lastError().text();
            return false;
        }
    }

    // Insert sample jobs if table is empty
    query.exec("SELECT COUNT(*) FROM jobs");
    if (query.next() && query.value(0).toInt() == 0)
    {
        success = query.exec(
            "INSERT INTO jobs (title, department, category, description, pay_rate_min, pay_rate_max, status, positions_available, hours_per_week, work_study_eligible, international_eligible, requirements, duties, skills, deadline) VALUES "
            "('Computer Lab Assistant', 'Computer Science', 'Academic Support', 'Assist students with computer lab activities, troubleshoot basic technical issues, and maintain lab equipment.', 15.00, 16.50, 0, 3, 12, 1, 1, 'Basic computer skills, good communication, reliable', 'Monitor lab usage, help students with software, maintain equipment', 'Technical support, customer service, problem solving', '2025-12-20'),"
            "('Library Circulation Desk', 'Library Services', 'Library', 'Work at circulation desk checking books in/out, helping students locate materials, and organizing library resources.', 16.00, 17.00, 0, 2, 10, 1, 1, 'Customer service skills, attention to detail', 'Check out materials, assist patrons, shelf reading, process returns', 'Library systems, organization, communication', '2025-12-25'),"
            "('IT Help Desk Support', 'IT Services', 'Technology', 'Provide first-level technical support to students and staff via phone, email, and in-person. Help with password resets and basic troubleshooting.', 17.00, 19.00, 0, 2, 15, 1, 0, 'Technical knowledge, patience, problem-solving skills', 'Answer support tickets, troubleshoot issues, document solutions', 'IT support, customer service, technical writing', '2025-12-30'),"
            "('Student Center Front Desk', 'Student Services', 'Student Services', 'Welcome visitors, answer questions, direct students to campus resources, and assist with event setup.', 15.50, 16.00, 0, 4, 12, 1, 1, 'Friendly demeanor, multitasking ability', 'Greet visitors, answer phones, provide information, assist with events', 'Communication, hospitality, multitasking', '2025-12-22'),"
            "('Tutoring Center Aide', 'Academic Affairs', 'Academic Support', 'Assist tutoring center coordinator, schedule appointments, maintain records, and help students connect with tutors.', 16.00, 17.50, 0, 2, 10, 1, 1, 'Organizational skills, proficiency with Microsoft Office', 'Schedule tutoring sessions, maintain databases, prepare materials', 'Organization, database management, student support', '2025-12-28')");
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
            query.value("grad_date").toString(),
            query.value("is_international_student").toBool(),
            query.value("survey_completed").toBool(),
            query.value("resume_path").toString(),
            query.value("transcript_path").toString(),
            query.value("parsed_resume_data").toString(),
            query.value("parsed_gpa").toString(),
            query.value("parsed_courses").toString());
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
            query.value("grad_date").toString(),
            query.value("is_international_student").toBool(),
            query.value("survey_completed").toBool(),
            query.value("resume_path").toString(),
            query.value("transcript_path").toString(),
            query.value("parsed_resume_data").toString(),
            query.value("parsed_gpa").toString(),
            query.value("parsed_courses").toString());
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

// Survey and document parsing methods
bool Database::updateSurveyData(int userId, bool isInternational, const QString &resumePath,
                                const QString &transcriptPath, const QString &parsedResumeData,
                                const QString &parsedGPA, const QString &parsedCourses)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET is_international_student = ?, resume_path = ?, "
                  "transcript_path = ?, parsed_resume_data = ?, parsed_gpa = ?, "
                  "parsed_courses = ?, survey_completed = 1 WHERE id = ?");
    query.addBindValue(isInternational ? 1 : 0);
    query.addBindValue(resumePath);
    query.addBindValue(transcriptPath);
    query.addBindValue(parsedResumeData);
    query.addBindValue(parsedGPA);
    query.addBindValue(parsedCourses);
    query.addBindValue(userId);

    bool success = query.exec();
    if (!success)
    {
        qDebug() << "Error updating survey data:" << query.lastError().text();
    }
    return success;
}

bool Database::markSurveyCompleted(int userId, bool completed)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET survey_completed = ? WHERE id = ?");
    query.addBindValue(completed ? 1 : 0);
    query.addBindValue(userId);

    return query.exec();
}

bool Database::updateInternationalStatus(int userId, bool isInternational)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET is_international_student = ? WHERE id = ?");
    query.addBindValue(isInternational ? 1 : 0);
    query.addBindValue(userId);

    return query.exec();
}