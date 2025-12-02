#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include "database/database.h"

class MyAccountPage : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountPage(Database *db, QWidget *parent = nullptr);
    void setUserId(int userId);

signals:
    void backToHomeRequested();
    void accountSaved();

private slots:
    void handleChangePassword();
    void handleSaveAccount();

private:
    void setupUI();
    void loadAccountData();

    Database *database;
    int currentUserId;

    // Login info
    QLineEdit *usernameEdit;
    QLineEdit *emailEdit;
    QPushButton *changePasswordButton;

    // Change password dialog fields (simple inline for now)
    QLineEdit *currentPasswordEdit;
    QLineEdit *newPasswordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *updatePasswordButton;
    QPushButton *cancelPasswordButton;

    // Notifications
    QCheckBox *notifyJobsCheck;
    QCheckBox *notifyApplicationsCheck;
    QCheckBox *notifyInterviewsCheck;

    // Status
    QLabel *accountTypeLabel;
    QLabel *memberSinceLabel;
    QLabel *statusLabel;

    // Bottom
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *backButton;
};
