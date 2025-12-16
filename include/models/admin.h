#pragma once
#include "person.h"

// ========== C++ CONCEPT #2: INHERITANCE ==========
// DERIVED CLASS (Child): Admin
// Admin INHERITS from Person (same parent as User)
// This shows we can have multiple child classes from one parent
class Admin : public Person
{
public:
    // Constructor
    Admin(int id = -1, const QString &email = "", const QString &fullName = "",
          const QString &department = "", const QString &accessLevel = "Standard")
        : Person(id, email, fullName), // Call parent constructor
          department(department), accessLevel(accessLevel)
    {
    }

    // Override virtual function from Person
    QString getRole() const override { return "Admin"; }

    // Admin-specific getters
    QString getDepartment() const { return department; }
    QString getAccessLevel() const { return accessLevel; }

    // Admin-specific setters
    void setDepartment(const QString &dept) { department = dept; }
    void setAccessLevel(const QString &level) { accessLevel = level; }

private:
    // Admin-specific members
    QString department;
    QString accessLevel; // e.g., "Standard", "Super", "ReadOnly"
};
