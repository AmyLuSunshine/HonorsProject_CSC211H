#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../database/database.h"

// ========== C++ CONCEPT #2: INHERITANCE ==========
// LoginWindow INHERITS from QWidget (parent class)
// Syntax: "class LoginWindow : public QWidget"
// This means:
// - LoginWindow IS-A QWidget (specialized type)
// - Gets all QWidget features: display, events, resizing, etc.
// - We only add login-specific features on top
// This demonstrates CODE REUSE - don't rewrite what Qt provides!
class LoginWindow : public QWidget
{
Q_OBJECT // Qt macro needed for signals/slots
    public : explicit LoginWindow(Database *db, QWidget *parent = nullptr);
    QString getCurrentUsername() const { return usernameEdit->text(); }
    bool isAdminLogin() const { return adminLoginFlag; }

signals:
    // Qt signals - part of inherited QObject functionality
    void loginSuccessful();
    void adminLoginSuccessful();
    void switchToRegister();

private slots:
    void handleLogin();
    void onRegisterClicked();

private:
    void setupUI();
    void setupStyles();

    Database *database;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *titleLabel;
    QLabel *errorLabel;
    bool adminLoginFlag; // Track if current login is admin
};
