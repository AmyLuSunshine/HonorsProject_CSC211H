#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include "database/database.h"

class MyProfilePage : public QWidget
{
    Q_OBJECT
public:
    explicit MyProfilePage(Database *db, QWidget *parent = nullptr);
    void setUserId(int userId);

signals:
    void profileSaved();

private slots:
    void handleSaveProfile();
    void handleChangePassword();

private:
    void setupUI();
    void setupStyles();
    void loadUser();

    Database *database;
    int currentUserId;

    // SECTION 1: Personal
    QLabel *nameLabel;
    QLineEdit *fullNameEdit;
    QLineEdit *emplidEdit;
    QLineEdit *emailEdit;
    QLineEdit *phoneEdit;
    QLineEdit *addressEdit;

    // SECTION 2: Academic
    QComboBox *majorCombo;
    QComboBox *currentYearCombo;
    QDateEdit *gradDateEdit;
    QLineEdit *gpaEdit;
    QRadioButton *intlYesRadio;
    QRadioButton *intlNoRadio;
    QLineEdit *countryOfOriginEdit;

    // SECTION 3: Documents
    QLineEdit *resumePathEdit;
    QPushButton *uploadResumeButton;
    QPushButton *viewResumeButton;
    QPushButton *deleteResumeButton;
    QLineEdit *transcriptPathEdit;
    QPushButton *uploadTranscriptButton;
    QPushButton *viewTranscriptButton;
    QPushButton *deleteTranscriptButton;

    // SECTION 4: Work Preferences
    QRadioButton *workStudyYesRadio;
    QRadioButton *workStudyNoRadio;

    // Bottom buttons
    QPushButton *saveButton;
    QPushButton *cancelButton;
};
