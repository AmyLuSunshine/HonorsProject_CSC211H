#pragma once
#include <QString>

// ========== C++ CONCEPT #2: INHERITANCE ==========
// BASE CLASS (Parent): Person
// This is the parent class that holds common properties for all people
class Person
{
public:
    // Constructor
    Person(int id = -1, const QString &email = "", const QString &fullName = "")
        : id(id), email(email), fullName(fullName) {}

    // Virtual destructor - good practice for base classes
    virtual ~Person() {}

    // Getters - common to all people
    int getId() const { return id; }
    QString getEmail() const { return email; }
    QString getFullName() const { return fullName; }

    // Virtual function - can be overridden by child classes
    virtual QString getRole() const { return "Person"; }

protected:
    // Protected - accessible by child classes
    int id;
    QString email;
    QString fullName;
};
