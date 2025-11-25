#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QFileDialog>
#include <QDate>
#include "database/database.h"

class MyApplicationPage : public QWidget
{
    Q_OBJECT
public:
    explicit MyApplicationPage(Database *db, QWidget *parent = nullptr);
    void setUserId(int userId);

signals:
    void documentsUpdated();

private slots:
    void uploadTranscript();
    void uploadCV();
    void showOnboardingStatus();

private:
    void setupUI();
    void setupStyles();
    void loadDocuments();
    void updateDocumentCounts();

    Database *database;
    int currentUserId;

    // Upload widgets
    QPushButton *uploadTranscriptBtn;
    QPushButton *uploadCVBtn;
    QLabel *transcriptLabel;
    QLabel *cvLabel;

    // Status labels
    QLabel *pendingDocsLabel;
    QLabel *completedDocsLabel;
    QLabel *processingDocsLabel;

    // History
    QTableWidget *documentsTable;

    // Actions
    QPushButton *saveButton;
    QPushButton *onboardingButton;
};
