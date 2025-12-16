#pragma once
#include <QString>
#include <QDateTime>
#include "person.h"

// ========== C++ CONCEPT #1: CLASSES ==========
// ========== C++ CONCEPT #2: INHERITANCE ==========
// User class INHERITS from Person (base class)
// Syntax: "class User : public Person"
// This means User IS-A Person with additional student-specific features
class User : public Person
{
public:
    // Default constructor
    User() : Person(), emplid(""), isInternationalStudent(false), surveyCompleted(false) {}

    // Parameterized constructor - calls Person constructor first
    User(int id, const QString &email, const QString &fullName,
         const QString &emplid, const QString &major = "",
         const QString &gpa = "", const QString &gradDate = "",
         bool isInternational = false, bool surveyed = false,
         const QString &resumePath = "", const QString &transcriptPath = "",
         const QString &parsedResumeData = "", const QString &parsedGPA = "",
         const QString &parsedCourses = "")
        : Person(id, email, fullName), // Call parent constructor
          emplid(emplid), major(major), gpa(gpa), gradDate(gradDate),
          isInternationalStudent(isInternational), surveyCompleted(surveyed),
          resumePath(resumePath), transcriptPath(transcriptPath),
          parsedResumeData(parsedResumeData), parsedGPA(parsedGPA),
          parsedCourses(parsedCourses)
    {
    }

    // INHERITED from Person: getId(), getEmail(), getFullName()
    // We don't need to rewrite these - we get them for free!

    // Override virtual function from Person
    QString getRole() const override { return "Student"; }

    // Student-specific getters
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

    // SETTER METHODS - provide CONTROLLED WRITE access to private data
    // We can add validation here if needed
    void setIsInternationalStudent(bool value) { isInternationalStudent = value; }
    void setSurveyCompleted(bool value) { surveyCompleted = value; }
    void setResumePath(const QString &path) { resumePath = path; }
    void setTranscriptPath(const QString &path) { transcriptPath = path; }
    void setParsedResumeData(const QString &data) { parsedResumeData = data; }
    void setParsedGPA(const QString &gpaData) { parsedGPA = gpaData; }
    void setParsedCourses(const QString &courses) { parsedCourses = courses; }

private:
    // Note: id, email, fullName are inherited from Person (protected members)
    // Student-specific private members
    QString emplid;
    QString major;
    QString gpa;
    QString gradDate;
    bool isInternationalStudent;
    bool surveyCompleted;
    QString resumePath;
    QString transcriptPath;
    QString parsedResumeData;
    QString parsedGPA;
    QString parsedCourses;
};
