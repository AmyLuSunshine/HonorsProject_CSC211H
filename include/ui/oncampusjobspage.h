#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QSlider>
#include <QTreeWidget>
#include <QDialog>
#include <QFileDialog>
#include <QScrollArea>
#include <QSplitter>
#include "database/database.h"
#include "models/job.h"

class JobApplicationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit JobApplicationDialog(Database *db, int userId, const Job &job, QWidget *parent = nullptr);

private slots:
    void browseResume();
    void browseCoverLetter();
    void submitApplication();
    void saveDraft();

private:
    void setupUI();
    Database *database;
    int userId;
    Job currentJob;

    QLineEdit *resumePathEdit;
    QLineEdit *coverLetterPathEdit;
    QTextEdit *whyInterestedEdit;
    QTextEdit *availabilityEdit;
    QTextEdit *referencesEdit;
    QCheckBox *confirmCheckbox;
};

class ExpressInterestDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExpressInterestDialog(Database *db, int userId, const Job &job, QWidget *parent = nullptr);

private slots:
    void submitInterest();

private:
    void setupUI();
    Database *database;
    int userId;
    Job currentJob;

    QTextEdit *messageEdit;
    QCheckBox *notifyCheckbox;
};

class OnCampusJobsPage : public QWidget
{
    Q_OBJECT
public:
    explicit OnCampusJobsPage(Database *db, int userId, QWidget *parent = nullptr);

signals:
    void jobApplied();

private slots:
    void onJobCardClicked(QListWidgetItem *item);
    void onSearchTextChanged(const QString &text);
    void onFilterChanged();
    void applyFilters();
    void clearFilters();
    void applyForJob();
    void expressInterest();

private:
    void setupUI();
    void setupStyles();
    void loadCategories();
    void loadJobs();
    void updateJobList(const std::vector<Job> &jobs);
    void showJobDetails(const Job &job);
    QString getCategoryIcon(const QString &category);
    QString getStatusBadge(JobStatus status, int positions = 0);

    Database *database;
    int currentUserId;
    Job selectedJob;
    std::vector<Job> currentJobs;
    std::vector<int> savedJobIds;

    // Left panel - Search & Job List
    QLineEdit *searchEdit;
    QComboBox *eligibilityFilter;
    QPushButton *clearFiltersBtn;
    QRadioButton *allStudentsBtn;
    QRadioButton *workStudyBtn;
    QRadioButton *intlBtn;
    QLabel *resultsCountLabel;
    QListWidget *jobListWidget;

    // Right panel - Job Details
    QWidget *detailPanel;
    QScrollArea *detailScrollArea;
    QLabel *jobTitleLabel;
    QLabel *jobDepartmentLabel;
    QLabel *statusBadgeLabel;
    QLabel *eligibilityInfoLabel;
    QPushButton *actionButton;
    QTextBrowser *jobDescriptionEdit;
    QLabel *requirementsLabel;
    QLabel *skillsLabel;

    // Helpers
    bool isInternationalEligible(const Job &job) const;
};
