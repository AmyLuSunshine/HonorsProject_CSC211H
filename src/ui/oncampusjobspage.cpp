#include "ui/oncampusjobspage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFormLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTextBrowser>

// JobApplicationDialog Implementation
JobApplicationDialog::JobApplicationDialog(Database *db, int userId, const Job &job, QWidget *parent)
    : QDialog(parent), database(db), userId(userId), currentJob(job)
{
    setupUI();
    setWindowTitle("Apply for " + job.getTitle());
    setMinimumSize(600, 700);
}

void JobApplicationDialog::setupUI()
{
    auto layout = new QVBoxLayout(this);

    auto formLayout = new QFormLayout();

    // Resume upload
    auto resumeLayout = new QHBoxLayout();
    resumePathEdit = new QLineEdit(this);
    resumePathEdit->setPlaceholderText("No file selected");
    resumePathEdit->setReadOnly(true);
    auto resumeBtn = new QPushButton("Browse...", this);
    connect(resumeBtn, &QPushButton::clicked, this, &JobApplicationDialog::browseResume);
    resumeLayout->addWidget(resumePathEdit);
    resumeLayout->addWidget(resumeBtn);
    formLayout->addRow("Resume (PDF):", resumeLayout);

    // Cover letter upload
    auto coverLayout = new QHBoxLayout();
    coverLetterPathEdit = new QLineEdit(this);
    coverLetterPathEdit->setPlaceholderText("No file selected (optional)");
    coverLetterPathEdit->setReadOnly(true);
    auto coverBtn = new QPushButton("Browse...", this);
    connect(coverBtn, &QPushButton::clicked, this, &JobApplicationDialog::browseCoverLetter);
    coverLayout->addWidget(coverLetterPathEdit);
    coverLayout->addWidget(coverBtn);
    formLayout->addRow("Cover Letter:", coverLayout);

    layout->addLayout(formLayout);

    // Why interested
    layout->addWidget(new QLabel("Why are you interested in this position?"));
    whyInterestedEdit = new QTextEdit(this);
    whyInterestedEdit->setPlaceholderText("Describe your interest and qualifications (max 1000 characters)");
    whyInterestedEdit->setMaximumHeight(120);
    layout->addWidget(whyInterestedEdit);

    // Availability
    layout->addWidget(new QLabel("Your Availability:"));
    availabilityEdit = new QTextEdit(this);
    availabilityEdit->setPlaceholderText("e.g., Monday-Friday 2pm-6pm, flexible on weekends");
    availabilityEdit->setMaximumHeight(80);
    layout->addWidget(availabilityEdit);

    // References
    layout->addWidget(new QLabel("References (2 required):"));
    referencesEdit = new QTextEdit(this);
    referencesEdit->setPlaceholderText("List names and contact info for 2 references");
    referencesEdit->setMaximumHeight(80);
    layout->addWidget(referencesEdit);

    // Confirmation
    confirmCheckbox = new QCheckBox("I confirm all information is accurate", this);
    layout->addWidget(confirmCheckbox);

    // Buttons
    auto btnLayout = new QHBoxLayout();
    auto saveBtn = new QPushButton("Save Draft", this);
    connect(saveBtn, &QPushButton::clicked, this, &JobApplicationDialog::saveDraft);
    auto submitBtn = new QPushButton("Submit Application", this);
    connect(submitBtn, &QPushButton::clicked, this, &JobApplicationDialog::submitApplication);
    btnLayout->addWidget(saveBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(submitBtn);
    layout->addLayout(btnLayout);
}

void JobApplicationDialog::browseResume()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Resume", "", "PDF Files (*.pdf);;All Files (*)");
    if (!file.isEmpty())
    {
        resumePathEdit->setText(file);
    }
}

void JobApplicationDialog::browseCoverLetter()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Cover Letter", "", "PDF Files (*.pdf);;All Files (*)");
    if (!file.isEmpty())
    {
        coverLetterPathEdit->setText(file);
    }
}

void JobApplicationDialog::submitApplication()
{
    if (resumePathEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Missing Information", "Please upload your resume.");
        return;
    }
    if (whyInterestedEdit->toPlainText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Missing Information", "Please explain why you're interested in this position.");
        return;
    }
    if (!confirmCheckbox->isChecked())
    {
        QMessageBox::warning(this, "Confirmation Required", "Please confirm that all information is accurate.");
        return;
    }

    bool success = database->applyForJob(
        userId,
        currentJob.getId(),
        resumePathEdit->text(),
        coverLetterPathEdit->text(),
        whyInterestedEdit->toPlainText(),
        availabilityEdit->toPlainText(),
        referencesEdit->toPlainText());

    if (success)
    {
        QMessageBox::information(this, "Application Submitted",
                                 "Your application has been submitted successfully! You will receive an email confirmation shortly.");
        accept();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to submit application. Please try again.");
    }
}

void JobApplicationDialog::saveDraft()
{
    QMessageBox::information(this, "Draft Saved", "Your application draft has been saved. You can continue later.");
    accept();
}

// ExpressInterestDialog Implementation
ExpressInterestDialog::ExpressInterestDialog(Database *db, int userId, const Job &job, QWidget *parent)
    : QDialog(parent), database(db), userId(userId), currentJob(job)
{
    setupUI();
    setWindowTitle("Express Interest - " + job.getTitle());
    setMinimumSize(500, 350);
}

void ExpressInterestDialog::setupUI()
{
    auto layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("This position is not currently accepting applications."));
    layout->addWidget(new QLabel("Tell us why you're interested, and we'll notify you when it opens!"));

    notifyCheckbox = new QCheckBox("Notify me when this position opens", this);
    notifyCheckbox->setChecked(true);
    layout->addWidget(notifyCheckbox);

    layout->addWidget(new QLabel("Why are you interested in this role? (500 characters max)"));
    messageEdit = new QTextEdit(this);
    messageEdit->setPlaceholderText("Share your interest and relevant background...");
    layout->addWidget(messageEdit);

    auto btnLayout = new QHBoxLayout();
    auto cancelBtn = new QPushButton("Cancel", this);
    auto submitBtn = new QPushButton("Submit Interest", this);
    submitBtn->setStyleSheet("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #2196F3, stop:1 #1976D2); color:white; padding:10px; border-radius:6px; font-weight:600;");
    connect(cancelBtn, &QPushButton::clicked, this, &ExpressInterestDialog::reject);
    connect(submitBtn, &QPushButton::clicked, this, &ExpressInterestDialog::submitInterest);
    btnLayout->addStretch();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(submitBtn);
    layout->addLayout(btnLayout);
}

void ExpressInterestDialog::submitInterest()
{
    QString message = messageEdit->toPlainText().trimmed();
    if (message.length() > 500)
    {
        QMessageBox::warning(this, "Too Long", "Please limit your message to 500 characters.");
        return;
    }

    bool success = database->expressInterest(userId, currentJob.getId(), message);
    if (success)
    {
        QMessageBox::information(this, "Interest Recorded",
                                 "Thank you! We've recorded your interest and will notify you when the position opens.");
        accept();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to record interest. Please try again.");
    }
}

// OnCampusJobsPage Implementation
OnCampusJobsPage::OnCampusJobsPage(Database *db, int userId, QWidget *parent)
    : QWidget(parent), database(db), currentUserId(userId)
{
    setupUI();
    setupStyles();
    loadCategories();
    loadJobs();

    savedJobIds = database->getSavedJobs(userId);
}

void OnCampusJobsPage::setupUI()
{
    // Simple two-column layout: job list on left, details on right
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // LEFT COLUMN - Job List
    auto leftPanel = new QWidget(this);
    auto leftLayout = new QVBoxLayout(leftPanel);

    auto titleLabel = new QLabel("On-Campus Jobs", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #1976D2;");
    leftLayout->addWidget(titleLabel);

    // Simple search bar
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Search jobs...");
    connect(searchEdit, &QLineEdit::textChanged, this, &OnCampusJobsPage::onSearchTextChanged);
    leftLayout->addWidget(searchEdit);

    // Results count
    resultsCountLabel = new QLabel("Showing 0 jobs");
    resultsCountLabel->setStyleSheet("color: #666; font-size: 12px;");
    leftLayout->addWidget(resultsCountLabel);

    // Job list widget
    jobListWidget = new QListWidget(this);
    connect(jobListWidget, &QListWidget::itemClicked, this, &OnCampusJobsPage::onJobCardClicked);
    leftLayout->addWidget(jobListWidget);

    leftPanel->setMinimumWidth(350);
    mainLayout->addWidget(leftPanel, 1);

    // RIGHT COLUMN - Job Details
    detailScrollArea = new QScrollArea(this);
    detailPanel = new QWidget();
    auto detailLayout = new QVBoxLayout(detailPanel);

    jobTitleLabel = new QLabel("Select a job to view details");
    jobTitleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1976D2;");
    jobTitleLabel->setWordWrap(true);
    detailLayout->addWidget(jobTitleLabel);

    jobDepartmentLabel = new QLabel("");
    jobDepartmentLabel->setStyleSheet("color: #666;");
    detailLayout->addWidget(jobDepartmentLabel);

    statusBadgeLabel = new QLabel("");
    detailLayout->addWidget(statusBadgeLabel);

    // Action buttons
    auto btnLayout = new QHBoxLayout();
    saveJobButton = new QPushButton("Save Job", this);
    saveJobButton->setVisible(false);
    connect(saveJobButton, &QPushButton::clicked, this, &OnCampusJobsPage::toggleSaveJob);

    actionButton = new QPushButton("Apply Now", this);
    actionButton->setVisible(false);
    connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
    btnLayout->addWidget(saveJobButton);
    btnLayout->addStretch();
    btnLayout->addWidget(actionButton);
    detailLayout->addLayout(btnLayout);

    // Description
    jobDescriptionEdit = new QTextBrowser(this);
    jobDescriptionEdit->setVisible(false);
    detailLayout->addWidget(jobDescriptionEdit);

    // Requirements
    requirementsEdit = new QTextBrowser(this);
    requirementsEdit->setVisible(false);
    detailLayout->addWidget(requirementsEdit);

    // Duties
    dutiesEdit = new QTextBrowser(this);
    dutiesEdit->setVisible(false);
    detailLayout->addWidget(dutiesEdit);

    detailLayout->addStretch();

    detailPanel->setLayout(detailLayout);
    detailScrollArea->setWidget(detailPanel);
    detailScrollArea->setWidgetResizable(true);
    mainLayout->addWidget(detailScrollArea, 1);

    // Hidden widgets for removed features (avoid crashes)
    categoryTree = new QTreeWidget(this);
    categoryTree->setVisible(false);
    statusFilter = new QComboBox(this);
    statusFilter->setVisible(false);
    eligibilityFilter = new QComboBox(this);
    eligibilityFilter->setVisible(false);
    clearFiltersBtn = new QPushButton(this);
    clearFiltersBtn->setVisible(false);
    payRateSlider = new QSlider(this);
    payRateSlider->setVisible(false);
    payRangeLabel = new QLabel(this);
    payRangeLabel->setVisible(false);
    departmentFilter = new QComboBox(this);
    departmentFilter->setVisible(false);
    quickFactsWidget = new QWidget(this);
    quickFactsWidget->setVisible(false);
    skillsLabel = new QLabel(this);
    skillsLabel->setVisible(false);
    supervisorLabel = new QLabel(this);
    supervisorLabel->setVisible(false);
    applicationProcessEdit = new QTextBrowser(this);
    applicationProcessEdit->setVisible(false);
}

void OnCampusJobsPage::setupStyles()
{
    setStyleSheet(
        "QWidget { background: white; font-family: Arial, sans-serif; }"
        "QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 4px; font-size: 14px; }"
        "QPushButton { background: #2196F3; color: white; border: none; padding: 8px 16px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background: #1976D2; }"
        "QListWidget { background: #f9f9f9; border: 1px solid #ddd; }"
        "QListWidget::item { padding: 10px; margin: 4px; background: white; border-radius: 4px; }"
        "QListWidget::item:hover { background: #e3f2fd; }"
        "QListWidget::item:selected { background: #bbdefb; }"
        "QTextBrowser { background: white; border: 1px solid #ddd; border-radius: 4px; padding: 8px; }");
}

void OnCampusJobsPage::loadCategories()
{
    // Category tree removed for simpler UI; kept as stub to avoid crashes
}

void OnCampusJobsPage::loadJobs()
{
    currentJobs = database->getJobs();
    updateJobList(currentJobs);
}

void OnCampusJobsPage::updateJobList(const std::vector<Job> &jobs)
{
    jobListWidget->clear();
    currentJobs = jobs;

    resultsCountLabel->setText(QString("%1 jobs available").arg(jobs.size()));

    for (const auto &job : jobs)
    {
        auto item = new QListWidgetItem();

        // Simple card text
        QString payText = job.getPayRateString();
        QString hoursText = QString("%1 hrs/week").arg(job.getHoursPerWeek());

        QString cardText = QString("%1\n%2 | %3 | %4")
                               .arg(job.getTitle())
                               .arg(job.getDepartment())
                               .arg(payText)
                               .arg(hoursText);

        item->setText(cardText);
        item->setData(Qt::UserRole, job.getId());
        item->setSizeHint(QSize(0, 60));

        jobListWidget->addItem(item);
    }
}

void OnCampusJobsPage::showJobDetails(const Job &job)
{
    selectedJob = job;

    jobTitleLabel->setText(job.getTitle());
    jobTitleLabel->setVisible(true);

    jobDepartmentLabel->setText("Department: " + job.getDepartment());
    jobDepartmentLabel->setVisible(true);

    QString statusText;
    if (job.getStatus() == JobStatus::Open)
        statusText = "Status: OPEN";
    else if (job.getStatus() == JobStatus::Closed)
        statusText = "Status: CLOSED";
    else
        statusText = "Status: UPCOMING";
    statusBadgeLabel->setText(statusText);
    statusBadgeLabel->setVisible(true);

    // Update action button
    if (job.getStatus() == JobStatus::Open)
    {
        actionButton->setText("Apply Now");
        actionButton->setStyleSheet("background: #4CAF50; color: white; padding: 10px; border-radius: 4px; font-weight: bold;");
        disconnect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::expressInterest);
        connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
    }
    else
    {
        actionButton->setText("Express Interest");
        actionButton->setStyleSheet("background: #2196F3; color: white; padding: 10px; border-radius: 4px; font-weight: bold;");
        disconnect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
        connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::expressInterest);
    }
    actionButton->setVisible(true);

    // Update save button
    bool isSaved = database->isJobSaved(currentUserId, job.getId());
    saveJobButton->setText(isSaved ? "💙 Saved" : "💾 Save Job");
    saveJobButton->setVisible(true);

    // Quick facts
    QString factsHtml = QString(
                            "<div style='background:#E3F2FD; padding:15px; border-radius:8px; border-left:4px solid #2196F3;'>"
                            "<div style='font-weight:600; color:#1976D2; margin-bottom:10px;'>Quick Facts</div>"
                            "<div style='font-size:13px; line-height:1.8;'>"
                            "💰 <b>Pay Rate:</b> %1<br>"
                            "⏰ <b>Hours:</b> %2 hours/week<br>"
                            "📅 <b>Schedule:</b> %3<br>"
                            "👥 <b>Positions:</b> %4 available<br>"
                            "🎓 <b>Work Study:</b> %5<br>"
                            "🌍 <b>Open to International:</b> %7<br>"
                            "📆 <b>Deadline:</b> %6"
                            "</div></div>")
                            .arg(job.getPayRateString())
                            .arg(job.getHoursPerWeek())
                            .arg(job.getSchedule().isEmpty() ? "Flexible" : job.getSchedule())
                            .arg(job.getPositionsAvailable())
                            .arg(job.isWorkStudyEligible() ? "Yes" : "No")
                            .arg(job.getDeadline().isEmpty() ? "Rolling" : job.getDeadline())
                            .arg(isInternationalEligible(job) ? "Yes" : "No");

    auto factsLabel = quickFactsWidget->findChild<QLabel *>();
    if (!factsLabel)
    {
        factsLabel = new QLabel(quickFactsWidget);
        factsLabel->setWordWrap(true);
        auto layout = qobject_cast<QVBoxLayout *>(quickFactsWidget->layout());
        if (layout)
            layout->addWidget(factsLabel);
    }
    factsLabel->setText(factsHtml);
    quickFactsWidget->setVisible(true);

    // Description
    jobDescriptionEdit->setHtml(QString("<h3>Job Description</h3><p>%1</p>").arg(job.getDescription()));
    jobDescriptionEdit->setVisible(true);

    // Requirements
    requirementsEdit->setHtml(QString("<h3>Qualifications & Requirements</h3><p>%1</p>").arg(job.getRequirements().isEmpty() ? "No specific requirements listed." : job.getRequirements()));
    requirementsEdit->setVisible(true);

    // Duties
    dutiesEdit->setHtml(QString("<h3>Duties & Responsibilities</h3><p>%1</p>").arg(job.getDuties().isEmpty() ? "Details to be discussed during interview." : job.getDuties()));
    dutiesEdit->setVisible(true);

    // Skills
    if (!job.getSkills().isEmpty())
    {
        skillsLabel->setText(QString("<h3>Skills You'll Gain</h3><p style='color:#2196F3;'>%1</p>").arg(job.getSkills()));
        skillsLabel->setVisible(true);
    }
    else
    {
        skillsLabel->setVisible(false);
    }

    // Supervisor
    if (!job.getSupervisorInfo().isEmpty())
    {
        supervisorLabel->setText(QString("<h3>Supervisor Information</h3><p>%1</p>").arg(job.getSupervisorInfo()));
        supervisorLabel->setVisible(true);
    }
    else
    {
        supervisorLabel->setVisible(false);
    }

    // Application Process
    if (!applicationProcessEdit)
    {
        applicationProcessEdit = new QTextBrowser(this);
        auto layout = qobject_cast<QVBoxLayout *>(detailPanel->layout());
        if (layout)
            layout->insertWidget(layout->count() - 1, applicationProcessEdit);
    }
    QString processHtml = QString(
        "<h3>Application Process</h3>"
        "<p>To apply for this position, please prepare the following:</p>"
        "<ul>"
        "<li>Resume (PDF)</li>"
        "<li>Optional Cover Letter</li>"
        "<li>Unofficial Transcript (if requested)</li>"
        "<li>References (2-3)</li>"
        "</ul>"
        "<p>Typical timeline:</p>"
        "<ol>"
        "<li>Submit application</li>"
        "<li>Screening (1-3 days)</li>"
        "<li>Interview invitation (if selected)</li>"
        "<li>Final decision</li>"
        "</ol>");
    applicationProcessEdit->setHtml(processHtml);
    applicationProcessEdit->setVisible(true);
}

QString OnCampusJobsPage::getCategoryIcon(const QString &category)
{
    if (category.contains("Academic"))
        return "📚";
    if (category.contains("Residential"))
        return "🏠";
    if (category.contains("Administrative"))
        return "📋";
    if (category.contains("Technology"))
        return "💻";
    if (category.contains("Library"))
        return "📖";
    if (category.contains("Food"))
        return "🍽️";
    if (category.contains("Recreation"))
        return "⚽";
    if (category.contains("Student"))
        return "👥";
    return "💼";
}

bool OnCampusJobsPage::isInternationalEligible(const Job &job) const
{
    return job.isInternationalEligible();
}

QString OnCampusJobsPage::getStatusBadge(JobStatus status, int positions)
{
    switch (status)
    {
    case JobStatus::Open:
        return QString("<span style='background:#4CAF50; color:white; padding:4px 12px; border-radius:12px; font-size:12px; font-weight:600;'>✓ OPEN - %1 position%2</span>")
            .arg(positions)
            .arg(positions == 1 ? "" : "s");
    case JobStatus::ClosingSoon:
        return "<span style='background:#FF9800; color:white; padding:4px 12px; border-radius:12px; font-size:12px; font-weight:600;'>⚠ CLOSING SOON</span>";
    case JobStatus::Closed:
        return "<span style='background:#9E9E9E; color:white; padding:4px 12px; border-radius:12px; font-size:12px; font-weight:600;'>✕ CLOSED</span>";
    case JobStatus::Upcoming:
        return "<span style='background:#2196F3; color:white; padding:4px 12px; border-radius:12px; font-size:12px; font-weight:600;'>🕐 OPENING SPRING 2025</span>";
    default:
        return "";
    }
}

void OnCampusJobsPage::onCategorySelected(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QString category = item->data(0, Qt::UserRole).toString();
    QString keyword = item->data(0, Qt::UserRole + 1).toString();

    if (category.isEmpty())
    {
        loadJobs();
        return;
    }
    // Administrative group aggregates multiple categories
    if (category == "Administrative_GROUP")
    {
        std::vector<Job> agg;
        for (const QString &cat : {QString("Administrative"), QString("Library Services"), QString("Food Services"), QString("Recreation & Fitness")})
        {
            auto vec = database->getJobsByCategory(cat);
            agg.insert(agg.end(), vec.begin(), vec.end());
        }
        updateJobList(agg);
        return;
    }
    if (!keyword.isEmpty())
    {
        auto jobs = database->searchJobs(keyword, "", category, false);
        updateJobList(jobs);
        return;
    }
    auto jobs = database->getJobsByCategory(category);
    updateJobList(jobs);
}

void OnCampusJobsPage::onJobCardClicked(QListWidgetItem *item)
{
    int jobId = item->data(Qt::UserRole).toInt();
    Job job = database->getJobById(jobId);
    showJobDetails(job);
}

void OnCampusJobsPage::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text);
    applyFilters();
}

void OnCampusJobsPage::onFilterChanged()
{
    applyFilters();
}

void OnCampusJobsPage::applyFilters()
{
    QString keyword = searchEdit->text().trimmed();
    if (keyword.isEmpty())
    {
        loadJobs();
        return;
    }
    // Simple search: filter jobs by keyword in title/department/description
    auto jobs = database->searchJobs(keyword, "", "", false, 0, 100, 0, 100);
    updateJobList(jobs);
}

void OnCampusJobsPage::clearFilters()
{
    searchEdit->clear();
    statusFilter->setCurrentIndex(0);
    eligibilityFilter->setCurrentIndex(0);
    loadJobs();
}

void OnCampusJobsPage::applyForJob()
{
    if (selectedJob.getId() < 0)
        return;

    JobApplicationDialog dialog(database, currentUserId, selectedJob, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, "Success", "Application submitted successfully!");
        emit jobApplied();
    }
}

void OnCampusJobsPage::expressInterest()
{
    if (selectedJob.getId() < 0)
        return;

    ExpressInterestDialog dialog(database, currentUserId, selectedJob, this);
    dialog.exec();
}

void OnCampusJobsPage::toggleSaveJob()
{
    if (selectedJob.getId() < 0)
        return;

    bool isSaved = database->isJobSaved(currentUserId, selectedJob.getId());

    if (isSaved)
    {
        database->unsaveJob(currentUserId, selectedJob.getId());
        saveJobButton->setText("💾 Save Job");
        QMessageBox::information(this, "Job Unsaved", "Job removed from your saved list.");
    }
    else
    {
        database->saveJob(currentUserId, selectedJob.getId());
        saveJobButton->setText("💙 Saved");
        QMessageBox::information(this, "Job Saved", "Job added to your saved list!");
    }

    savedJobIds = database->getSavedJobs(currentUserId);
}
