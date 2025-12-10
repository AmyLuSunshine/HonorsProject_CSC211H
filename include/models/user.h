#pragma once
#include <QString>
#include <QDateTime>

class User
{
public:
    User() : id(-1), isInternationalStudent(false), surveyCompleted(false) {}
    User(int id, const QString &email, const QString &fullName,
         const QString &emplid, const QString &major = "",
         const QString &gpa = "", const QString &gradDate = "",
         bool isInternational = false, bool surveyed = false,
         const QString &resumePath = "", const QString &transcriptPath = "",
         const QString &parsedResumeData = "", const QString &parsedGPA = "",
         const QString &parsedCourses = "")
        : id(id), email(email), fullName(fullName), emplid(emplid),
          major(major), gpa(gpa), gradDate(gradDate),
          isInternationalStudent(isInternational), surveyCompleted(surveyed),
          resumePath(resumePath), transcriptPath(transcriptPath),
          parsedResumeData(parsedResumeData), parsedGPA(parsedGPA),
          parsedCourses(parsedCourses) {}

    // Basic getters
    int getId() const { return id; }
    QString getEmail() const { return email; }
    QString getFullName() const { return fullName; }
    QString getEmplid() const { return emplid; }
    QString getMajor() const { return major; }
    QString getGpa() const { return gpa; }
    QString getGradDate() const { return gradDate; }

    // New getters
    bool getIsInternationalStudent() const { return isInternationalStudent; }
    bool getSurveyCompleted() const { return surveyCompleted; }
    QString getResumePath() const { return resumePath; }
    QString getTranscriptPath() const { return transcriptPath; }
    QString getParsedResumeData() const { return parsedResumeData; }
    QString getParsedGPA() const { return parsedGPA; }
    QString getParsedCourses() const { return parsedCourses; }

    // Setters
    void setIsInternationalStudent(bool value) { isInternationalStudent = value; }
    void setSurveyCompleted(bool value) { surveyCompleted = value; }
    void setResumePath(const QString &path) { resumePath = path; }
    void setTranscriptPath(const QString &path) { transcriptPath = path; }
    void setParsedResumeData(const QString &data) { parsedResumeData = data; }
    void setParsedGPA(const QString &gpaData) { parsedGPA = gpaData; }
    void setParsedCourses(const QString &courses) { parsedCourses = courses; }

private:
    int id;
    QString email;
    QString fullName;
    QString emplid;
    QString major;
    QString gpa;
    QString gradDate;

    // New fields
    bool isInternationalStudent;
    bool surveyCompleted;
    QString resumePath;
    QString transcriptPath;
    QString parsedResumeData; // JSON or structured text from resume
    QString parsedGPA;        // Parsed from transcript
    QString parsedCourses;    // Parsed course list from transcript
};
