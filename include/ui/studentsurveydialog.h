#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include "database/database.h"

class StudentSurveyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StudentSurveyDialog(Database *db, int userId, QWidget *parent = nullptr);

signals:
    void surveyCompleted();

private slots:
    void browseResume();
    void browseTranscript();
    void submitSurvey();
    void skipSurvey();

private:
    void setupUI();
    void setupStyles();
    // Parsing features removed

    Database *database;
    int currentUserId;

    // UI elements
    QLabel *titleLabel;
    QLabel *descriptionLabel;

    // International student status
    QCheckBox *internationalCheckbox;
    QLabel *internationalHelpLabel;

    // Resume upload
    QLineEdit *resumePathEdit;
    QPushButton *resumeBrowseBtn;
    QLabel *resumeStatusLabel;

    // Transcript upload
    QLineEdit *transcriptPathEdit;
    QPushButton *transcriptBrowseBtn;
    QLabel *transcriptStatusLabel;

    // Parsed data preview removed

    // Progress
    QProgressBar *progressBar;

    // Buttons
    QPushButton *submitBtn;
    QPushButton *skipBtn;

    // Parsed data storage removed
};
