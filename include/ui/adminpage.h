#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include "database/database.h"
#include "models/admin.h"

// Very basic admin page - foundation for future expansion
class AdminPage : public QWidget
{
    Q_OBJECT
public:
    explicit AdminPage(Database *db, const Admin &adminData, QWidget *parent = nullptr);

private slots:
    void refreshUserList();
    void handleLogout();

private:
    void setupUI();
    void loadUserData();

    // ========== POINTER MEMBER VARIABLES ==========
    Database *database; // Pointer to database object
    Admin currentAdmin; // Admin data (using custom inheritance class)

    // UI pointers
    QLabel *welcomeLabel;
    QLabel *statsLabel;
    QTableWidget *userTable;
    QPushButton *refreshButton;
    QPushButton *logoutButton;

signals:
    void logoutRequested();
};
