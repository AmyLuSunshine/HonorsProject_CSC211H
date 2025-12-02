#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QTextEdit>
#include <QScrollArea>
#include <QComboBox>
#include <QDateEdit>
#include <QRadioButton>
#include "database/database.h"

// Settings page with My Profile and My Account sections
class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(Database *db, QWidget *parent = nullptr);
    void setUserId(int userId);

signals:
    void backToHomeRequested();
    void profileUpdated();

private slots:
    void handleSaveProfile();
    void handleChangePassword();

private:
    void setupUI();
    void loadUserProfile();

    Database *database;
    int currentUserId;

    // Tabs
    QTabWidget *tabWidget;

    // My Profile widgets
    QCheckBox *internationalCheckbox;
    QLineEdit *degreeEdit;
    QLineEdit *creditsTakenEdit;
    QLineEdit *gpaEdit;
    QTextEdit *coursesEdit;
    QLineEdit *resumePathEdit;
    QLineEdit *transcriptPathEdit;
    QPushButton *saveProfileButton;
    QPushButton *cancelProfileButton;

    // Personal information
    QLineEdit *fullNameEdit;
    QLineEdit *studentIdEdit;
    QLineEdit *emailEdit;
    QLineEdit *phoneEdit;
    QLineEdit *addressEdit;

    // Academic information
    QComboBox *majorCombo;
    QComboBox *currentYearCombo;
    QDateEdit *expectedGradDateEdit;
    QRadioButton *intlYesRadio;
    QRadioButton *intlNoRadio;
    QLineEdit *countryOfOriginEdit;

    // Documents
    QPushButton *uploadResumeButton;
    QPushButton *viewResumeButton;
    QPushButton *deleteResumeButton;
    QPushButton *uploadTranscriptButton;
    QPushButton *viewTranscriptButton;
    QPushButton *deleteTranscriptButton;

    // Work preferences
    QRadioButton *workStudyYesRadio;
    QRadioButton *workStudyNoRadio;

    // My Account widgets
    QLineEdit *usernameEdit;
    QLineEdit *newPasswordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *changePasswordButton;
    QCheckBox *notifyJobsCheck;
    QCheckBox *notifyApplicationsCheck;
    QCheckBox *notifyInterviewsCheck;
    QLabel *accountTypeLabel;
    QLabel *memberSinceLabel;
    QLabel *statusLabel;
    QPushButton *saveAccountButton;
    QPushButton *cancelAccountButton;

    QPushButton *backButton;
};

#endif // SETTINGSPAGE_H
