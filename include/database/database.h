#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QDateTime>
#include <vector>
#include "models/job.h"
#include "models/user.h"

struct Document
{
    int id;
    int userId;
    QString documentType;
    QString filePath;
    QString uploadDate;
    QString status;

    Document() : id(-1), userId(-1) {}
    Document(int id, int userId, const QString &type, const QString &path,
             const QString &date, const QString &status)
        : id(id), userId(userId), documentType(type), filePath(path),
          uploadDate(date), status(status) {}
};

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool connectToDatabase();
    bool validateLogin(const QString &email, const QString &password);
    bool registerUser(const QString &fullName, const QString &password,
                      const QString &email, const QString &emplid,
                      const QString &major = "", const QString &gpa = "",
                      const QString &gradDate = "");

    bool updateProfile(const QString &email, const QString &major,
                       const QString &gpa, const QString &gradDate);

    // New methods for user profile by ID
    bool updateProfileById(int userId, const QString &major,
                           const QString &gpa, const QString &gradDate);
    User getUserDataById(int userId);
    int getUserIdByEmail(const QString &email);

    // Document management methods
    bool addDocument(int userId, const QString &documentType,
                     const QString &filePath, const QString &status = "Pending");
    bool updateDocumentStatus(int documentId, const QString &status);
    bool deleteDocument(int documentId);
    std::vector<Document> getUserDocuments(int userId);
    Document getLatestDocument(int userId, const QString &documentType);

    // Document statistics
    int getDocumentCountByStatus(int userId, const QString &status);
    bool hasDocument(int userId, const QString &documentType);

    User getUserData(const QString &email);
    std::vector<Job> getJobs();

private:
    QSqlDatabase db;
    bool createTables();
};