#pragma once
#include <QString>
#include <QDateTime>

class User
{
public:
    User() : id(-1) {}
    User(int id, const QString &email, const QString &fullName,
         const QString &emplid, const QString &major = "",
         const QString &gpa = "", const QString &gradDate = "")
        : id(id), email(email), fullName(fullName), emplid(emplid),
          major(major), gpa(gpa), gradDate(gradDate) {}

    int getId() const { return id; }
    QString getEmail() const { return email; }
    QString getFullName() const { return fullName; }
    QString getEmplid() const { return emplid; }
    QString getMajor() const { return major; }
    QString getGpa() const { return gpa; }
    QString getGradDate() const { return gradDate; }

private:
    int id;
    QString email;
    QString fullName;
    QString emplid;
    QString major;
    QString gpa;
    QString gradDate;
};
