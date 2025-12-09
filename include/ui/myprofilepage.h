#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QTextEdit>
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

private:
    void setupUI();
    void setupStyles();
    void loadUser();

    Database *database;
    int currentUserId;

    // Personal info widgets
    QLabel *nameLabel;
    QLineEdit *fullNameEdit;
    QLineEdit *emplidEdit;
    QLineEdit *emailEdit;
    QLineEdit *addressEdit;
    QLineEdit *phoneEdit;

    // Academic information
    QComboBox *majorCombo;
    QComboBox *currentYearCombo;
    QLineEdit *gpaEdit;
    QDateEdit *gradDateEdit;
    QLineEdit *degreeEdit;
    QLineEdit *creditsTakenEdit;
    QTextEdit *coursesEdit;

    // International student
    QRadioButton *intlYesRadio;
    QRadioButton *intlNoRadio;
    QCheckBox *internationalCheckbox;
    QLineEdit *countryOfOriginEdit;

    // Documents
    QLineEdit *resumePathEdit;
    QLineEdit *transcriptPathEdit;
    QPushButton *uploadResumeButton;
    QPushButton *viewResumeButton;
    QPushButton *deleteResumeButton;
    QPushButton *uploadTranscriptButton;
    QPushButton *viewTranscriptButton;
    QPushButton *deleteTranscriptButton;

    // Work preferences
    QRadioButton *workStudyYesRadio;
    QRadioButton *workStudyNoRadio;

    // Save button for profile only
    QPushButton *saveButton;
    QPushButton *cancelButton;
};
