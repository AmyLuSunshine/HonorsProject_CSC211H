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

    // Job management methods
    std::vector<Job> getJobs();
    std::vector<Job> getJobsByCategory(const QString &category);
    std::vector<Job> searchJobs(const QString &keyword, const QString &statusFilter = "",
                                const QString &categoryFilter = "", bool workStudyOnly = false,
                                int minHours = 0, int maxHours = 40,
                                double minPay = 0, double maxPay = 100);
    Job getJobById(int jobId);
    int getJobCountByCategory(const QString &category);

    // Job application methods
    bool applyForJob(int userId, int jobId, const QString &resumePath,
                     const QString &coverLetterPath, const QString &whyInterested,
                     const QString &availability, const QString &references);
    bool expressInterest(int userId, int jobId, const QString &message);
    bool saveJob(int userId, int jobId);
    bool unsaveJob(int userId, int jobId);
    std::vector<int> getSavedJobs(int userId);
    bool isJobSaved(int userId, int jobId);

private:
    QSqlDatabase db;
    bool createTables();
};