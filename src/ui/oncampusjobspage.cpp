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

    // Get user's saved jobs for bookmarking feature
    savedJobIds = database->getSavedJobs(userId);

    // Load jobs with the selected filter (defaults to "All Students")
    loadJobs();
}

void OnCampusJobsPage::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // TOP PANEL - Compact Eligibility Filter
    auto filterPanel = new QWidget();
    filterPanel->setMaximumHeight(40);
    filterPanel->setStyleSheet("QWidget { background: #FFFFFF; border-bottom: 1px solid #E0E0E0; }");
    auto filterLayout = new QHBoxLayout(filterPanel);
    filterLayout->setContentsMargins(15, 0, 15, 0);
    filterLayout->setSpacing(12);

    auto filterLabel = new QLabel("Show:", this);
    filterLabel->setStyleSheet("font-size: 12px; color: #424242; font-weight: 500;");
    filterLayout->addWidget(filterLabel);

    // Create radio buttons for eligibility
    allStudentsBtn = new QRadioButton("All Jobs", this);
    workStudyBtn = new QRadioButton("Work Study", this);
    intlBtn = new QRadioButton("Open to International Students", this);
    allStudentsBtn->setChecked(true);

    filterLayout->addWidget(allStudentsBtn);
    filterLayout->addWidget(workStudyBtn);
    filterLayout->addWidget(intlBtn);
    filterLayout->addStretch();

    connect(allStudentsBtn, &QRadioButton::clicked, this, &OnCampusJobsPage::onFilterChanged);
    connect(workStudyBtn, &QRadioButton::clicked, this, &OnCampusJobsPage::onFilterChanged);
    connect(intlBtn, &QRadioButton::clicked, this, &OnCampusJobsPage::onFilterChanged);

    mainLayout->addWidget(filterPanel);

    // MAIN CONTENT - Job List and Details in Splitter
    auto splitter = new QSplitter(Qt::Horizontal, this);

    // LEFT PANEL - Job List
    auto leftPanel = new QWidget();
    auto leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(15, 15, 15, 15);
    leftLayout->setSpacing(10);

    // Results count
    resultsCountLabel = new QLabel("Showing 0 jobs");
    resultsCountLabel->setStyleSheet("color: #757575; font-size: 12px;");
    leftLayout->addWidget(resultsCountLabel);

    // Job list
    jobListWidget = new QListWidget(this);
    jobListWidget->setSpacing(8);
    connect(jobListWidget, &QListWidget::itemClicked, this, &OnCampusJobsPage::onJobCardClicked);
    leftLayout->addWidget(jobListWidget);

    leftPanel->setMinimumWidth(300);

    // RIGHT PANEL - Job Details
    detailPanel = new QWidget();
    auto detailLayout = new QVBoxLayout(detailPanel);
    detailLayout->setContentsMargins(15, 15, 15, 15);
    detailLayout->setSpacing(15);

    // Job title
    jobTitleLabel = new QLabel("Select a job to view details");
    jobTitleLabel->setStyleSheet("font-size: 18px; font-weight: 600; color: #1976D2;");
    jobTitleLabel->setWordWrap(true);
    detailLayout->addWidget(jobTitleLabel);

    // Department and status
    jobDepartmentLabel = new QLabel("");
    jobDepartmentLabel->setStyleSheet("font-size: 13px; color: #757575;");
    detailLayout->addWidget(jobDepartmentLabel);

    statusBadgeLabel = new QLabel("");
    detailLayout->addWidget(statusBadgeLabel);

    // Eligibility info for selected job
    eligibilityInfoLabel = new QLabel("");
    eligibilityInfoLabel->setVisible(false);
    eligibilityInfoLabel->setStyleSheet("font-size: 12px; color: #424242; margin-top: 8px;");
    eligibilityInfoLabel->setWordWrap(true);
    detailLayout->addWidget(eligibilityInfoLabel);

    detailLayout->addSpacing(10);

    // Apply button
    actionButton = new QPushButton("Apply Now", this);
    actionButton->setVisible(false);
    actionButton->setMinimumHeight(40);
    connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
    detailLayout->addWidget(actionButton);

    detailLayout->addSpacing(10);

    // Description
    jobDescriptionEdit = new QTextBrowser(this);
    jobDescriptionEdit->setVisible(false);
    jobDescriptionEdit->setMaximumHeight(150);
    detailLayout->addWidget(jobDescriptionEdit);

    // Requirements
    requirementsLabel = new QLabel("");
    requirementsLabel->setVisible(false);
    requirementsLabel->setWordWrap(true);
    detailLayout->addWidget(requirementsLabel);

    // Skills
    skillsLabel = new QLabel("");
    skillsLabel->setVisible(false);
    skillsLabel->setWordWrap(true);
    detailLayout->addWidget(skillsLabel);

    detailLayout->addStretch();

    // Onboarding Documents Box - Fixed at bottom
    QWidget *onboardingBox = new QWidget(this);
    onboardingBox->setStyleSheet("background: #FFF9C4; border: 2px solid #F57C00; border-radius: 6px; padding: 10px;");
    QVBoxLayout *onboardingLayout = new QVBoxLayout(onboardingBox);
    onboardingLayout->setSpacing(6);
    onboardingLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *onboardingTitle = new QLabel("📋 Prepare If Hired:", this);
    onboardingTitle->setStyleSheet("font-size: 13px; font-weight: 700; color: #E65100; padding: 0;");
    onboardingLayout->addWidget(onboardingTitle);

    QLabel *onboardingText = new QLabel(
        "• Social Security Card<br>"
        "• ID + Birth Certificate (or Passport)<br>"
        "• Bank Account Info<br>"
        "• W-4 Tax Form",
        this);
    onboardingText->setStyleSheet("font-size: 11px; color: #333; line-height: 1.4; padding: 0;");
    onboardingText->setWordWrap(true);
    onboardingLayout->addWidget(onboardingText);

    detailLayout->addWidget(onboardingBox);

    detailScrollArea = new QScrollArea(this);
    detailScrollArea->setWidget(detailPanel);
    detailScrollArea->setWidgetResizable(true);
    detailScrollArea->setMinimumWidth(300);

    // Add panels to splitter
    splitter->addWidget(leftPanel);
    splitter->addWidget(detailScrollArea);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes(QList<int>() << 400 << 500);

    mainLayout->addWidget(splitter);
}

void OnCampusJobsPage::setupStyles()
{
    setStyleSheet(
        "QWidget { background: #FAFAFA; font-family: 'Segoe UI', Arial, sans-serif; }"
        "QLineEdit { padding: 10px; border: 2px solid #E0E0E0; border-radius: 8px; background: white; font-size: 14px; }"
        "QLineEdit:focus { border: 2px solid #2196F3; }"
        "QComboBox, QPushButton { padding: 8px 16px; border-radius: 6px; font-size: 14px; }"
        "QPushButton { background: #2196F3; color: white; border: none; font-weight: 600; }"
        "QPushButton:hover { background: #1976D2; }"
        "QListWidget { background: white; border: 1px solid #E0E0E0; border-radius: 8px; padding: 4px; }"
        "QListWidget::item { border-left: 4px solid transparent; padding: 0px; margin: 4px; background: white; border-radius: 6px; border: 1px solid #E0E0E0; }"
        "QListWidget::item:hover { border-left: 4px solid #2196F3; background: #F5F5F5; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }"
        "QListWidget::item:selected { border-left: 4px solid #1976D2; background: #E3F2FD; border: 1px solid #2196F3; }"
        "QRadioButton { font-size: 12px; color: #424242; spacing: 6px; }"
        "QRadioButton::indicator { width: 14px; height: 14px; }"
        "QRadioButton:hover { color: #2196F3; }"
        "QTextBrowser { background: white; border: 1px solid #E0E0E0; border-radius: 6px; padding: 12px; line-height: 1.5; }");
}

void OnCampusJobsPage::loadJobs()
{
    // Load all jobs from database and filter to show only open positions
    auto allJobs = database->getJobs();
    std::vector<Job> openJobs;

    for (const auto &job : allJobs)
    {
        // Only show jobs with Open status
        if (job.getStatus() == JobStatus::Open)
        {
            openJobs.push_back(job);
        }
    }

    currentJobs = openJobs;

    // Apply the currently selected eligibility filter
    applyFilters();
}

void OnCampusJobsPage::updateJobList(const std::vector<Job> &jobs)
{
    jobListWidget->clear();

    resultsCountLabel->setText(QString("Showing %1 job%2").arg(jobs.size()).arg(jobs.size() == 1 ? "" : "s"));

    // Show helpful message if no jobs match the filter
    if (jobs.empty())
    {
        auto item = new QListWidgetItem();
        QString emptyMsg = "<div style='padding:40px; text-align:center; color:#757575;'>"
                           "<div style='font-size:48px; margin-bottom:10px;'>🔍</div>"
                           "<div style='font-size:16px; font-weight:600; margin-bottom:8px;'>No jobs found</div>"
                           "<div style='font-size:13px;'>Try selecting a different filter option</div>"
                           "</div>";
        auto label = new QLabel(emptyMsg);
        label->setAlignment(Qt::AlignCenter);
        item->setSizeHint(QSize(0, 200));
        jobListWidget->addItem(item);
        jobListWidget->setItemWidget(item, label);
        return;
    }

    for (const auto &job : jobs)
    {
        auto item = new QListWidgetItem();

        QString payText = (job.getPayRateMin() == 0 && job.getPayRateMax() == 0)
                              ? "TBD"
                              : job.getPayRateString();
        QString hoursText = (job.getHoursPerWeek() <= 0)
                                ? "10-15 hours/week"
                                : QString("%1 hours/week").arg(job.getHoursPerWeek());
        QString oneLineDesc = job.getDescription();
        if (oneLineDesc.length() > 120)
            oneLineDesc = oneLineDesc.left(117) + "...";
        QString intlIcon = isInternationalEligible(job) ? " | 🌍 International" : "";
        QString wsIcon = job.isWorkStudyEligible() ? " | 🎓 Work Study" : "";

        QString cardHtml = QString(
                               "<div style='padding:8px;'>"
                               "<div style='font-size:16px; font-weight:600; color:#1976D2; margin-bottom:4px;'>%1</div>"
                               "<div style='font-size:12px; color:#757575; margin-bottom:6px;'>📍 %2</div>"
                               "<div style='margin-bottom:6px;'>%3</div>"
                               "<div style='font-size:12px; color:#424242; background:#f9f9f9; padding:6px; border-radius:4px; margin-bottom:6px;'>"
                               "💰 <b>%4</b> • ⏰ %5%6%7</div>"
                               "<div style='font-size:12px; color:#616161; line-height:1.4;'>%8</div>"
                               "</div>")
                               .arg(job.getTitle())
                               .arg(job.getDepartment())
                               .arg(getStatusBadge(job.getStatus(), job.getPositionsAvailable()))
                               .arg(payText)
                               .arg(hoursText)
                               .arg(wsIcon)
                               .arg(intlIcon)
                               .arg(oneLineDesc);
        item->setData(Qt::UserRole, job.getId());
        item->setSizeHint(QSize(0, 100));
        item->setToolTip(job.getDescription());

        jobListWidget->addItem(item);

        auto label = new QLabel(cardHtml);
        label->setWordWrap(true);
        label->setStyleSheet("padding: 8px;");
        jobListWidget->setItemWidget(item, label);
    }
}

void OnCampusJobsPage::showJobDetails(const Job &job)
{
    selectedJob = job;

    // Header Section
    jobTitleLabel->setText(job.getTitle());
    jobDepartmentLabel->setText("Department: " + job.getDepartment());
    statusBadgeLabel->setText(getStatusBadge(job.getStatus(), job.getPositionsAvailable()));

    // Build eligibility info string
    QStringList eligibilityList;
    if (job.isWorkStudyEligible())
        eligibilityList << "🎓 Work Study Eligible";
    if (isInternationalEligible(job))
        eligibilityList << "🌍 Open to International Students";

    if (!eligibilityList.isEmpty())
    {
        eligibilityInfoLabel->setText("Eligibility: " + eligibilityList.join(" | "));
        eligibilityInfoLabel->setVisible(true);
    }
    else
    {
        eligibilityInfoLabel->setVisible(false);
    }

    // Update action button
    if (job.getStatus() == JobStatus::Open)
    {
        actionButton->setText("Apply Now");
        actionButton->setStyleSheet("background: #4CAF50; color:white; padding:10px; border-radius:6px; font-weight:600;");
        disconnect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::expressInterest);
        connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
    }
    else
    {
        actionButton->setText("Express Interest");
        actionButton->setStyleSheet("background: #2196F3; color:white; padding:10px; border-radius:6px; font-weight:600;");
        disconnect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
        connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::expressInterest);
    }
    actionButton->setVisible(true);

    // Build comprehensive job details HTML
    QString htmlContent;

    // Key Job Info Section
    htmlContent += "<div style='background: #e3f2fd; padding: 12px; border-radius: 6px; margin-bottom: 14px; border-left: 4px solid #2196F3;'>";
    htmlContent += "<b style='color: #1565C0; font-size: 13px;'>Quick Overview</b>";
    htmlContent += "<table width='100%' style='margin-top:8px;'>";
    htmlContent += QString("<tr><td style='padding:4px 0; color:#424242;'><b>Pay Rate:</b></td><td style='padding:4px 0;'>%1</td></tr>").arg(job.getPayRateString());
    htmlContent += QString("<tr><td style='padding:4px 0; color:#424242;'><b>Hours/Week:</b></td><td style='padding:4px 0;'>%1</td></tr>").arg(job.getHoursPerWeek());
    htmlContent += QString("<tr><td style='padding:4px 0; color:#424242;'><b>Positions:</b></td><td style='padding:4px 0;'>%1</td></tr>").arg(job.getPositionsAvailable());

    // Add category if available
    if (!job.getCategory().isEmpty())
    {
        htmlContent += QString("<tr><td style='padding:4px 0; color:#424242;'><b>📂 Category:</b></td><td style='padding:4px 0;'>%1</td></tr>").arg(job.getCategory());
    }

    // Add deadline if available
    if (!job.getDeadline().isEmpty())
    {
        htmlContent += QString("<tr><td><b>Application Deadline:</b></td><td>%1</td></tr>").arg(job.getDeadline());
    }

    // Add schedule if available
    if (!job.getSchedule().isEmpty())
    {
        htmlContent += QString("<tr><td><b>Work Schedule:</b></td><td>%1</td></tr>").arg(job.getSchedule());
    }

    htmlContent += "</table></div>";

    // Job Description Section
    if (!job.getDescription().isEmpty())
    {
        htmlContent += "<div style='margin-bottom: 14px;'>";
        htmlContent += "<div style='font-size: 14px; font-weight: 600; color: #1976D2; margin-bottom: 6px; padding-bottom: 4px; border-bottom: 2px solid #E3F2FD;'>About This Position</div>";
        htmlContent += QString("<p style='margin: 8px 0; line-height: 1.6; color: #424242;'>%1</p>").arg(job.getDescription());
        htmlContent += "</div>";
    }

    // Duties Section
    if (!job.getDuties().isEmpty())
    {
        htmlContent += "<div style='margin-bottom: 14px;'>";
        htmlContent += "<div style='font-size: 14px; font-weight: 600; color: #1976D2; margin-bottom: 6px; padding-bottom: 4px; border-bottom: 2px solid #E3F2FD;'>Your Responsibilities</div>";
        htmlContent += QString("<p style='margin: 8px 0; line-height: 1.6; color: #424242;'>%1</p>").arg(job.getDuties());
        htmlContent += "</div>";
    }

    // Requirements Section
    if (!job.getRequirements().isEmpty())
    {
        htmlContent += "<div style='margin-bottom: 14px;'>";
        htmlContent += "<div style='font-size: 14px; font-weight: 600; color: #1976D2; margin-bottom: 6px; padding-bottom: 4px; border-bottom: 2px solid #E3F2FD;'>What We're Looking For</div>";
        htmlContent += QString("<p style='margin: 8px 0; line-height: 1.6; color: #424242;'>%1</p>").arg(job.getRequirements());
        htmlContent += "</div>";
    }

    // Skills Section
    if (!job.getSkills().isEmpty())
    {
        htmlContent += "<div style='margin-bottom: 14px;'>";
        htmlContent += "<div style='font-size: 14px; font-weight: 600; color: #1976D2; margin-bottom: 6px; padding-bottom: 4px; border-bottom: 2px solid #E3F2FD;'>Skills You'll Gain</div>";
        htmlContent += QString("<p style='margin: 8px 0; line-height: 1.6; color: #424242;'>%1</p>").arg(job.getSkills());
        htmlContent += "</div>";
    }

    // Required Documents Section
    if (!job.getRequiredDocuments().isEmpty())
    {
        htmlContent += "<div style='margin-bottom: 14px;'>";
        htmlContent += "<div style='font-size: 14px; font-weight: 600; color: #D32F2F; margin-bottom: 6px; padding-bottom: 4px; border-bottom: 2px solid #FFEBEE;'>📄 Required Application Documents</div>";
        htmlContent += QString("<p style='margin: 8px 0; line-height: 1.6; color: #424242;'>%1</p>").arg(job.getRequiredDocuments());
        htmlContent += "</div>";
    }

    // Contact & Supervisor Section
    if (!job.getContactEmail().isEmpty() || !job.getSupervisorInfo().isEmpty())
    {
        htmlContent += "<div style='background: #e8f5e9; padding: 12px; border-radius: 6px; border-left: 4px solid #4CAF50; margin-top: 16px;'>";
        htmlContent += "<b style='color: #2e7d32; font-size: 13px;'>Contact Information</b><br>";
        if (!job.getSupervisorInfo().isEmpty())
        {
            htmlContent += QString("<p style='margin: 4px 0;'><b>Supervisor:</b> %1</p>").arg(job.getSupervisorInfo());
        }
        if (!job.getContactEmail().isEmpty())
        {
            htmlContent += QString("<p style='margin: 4px 0;'><b>Email:</b> <a href='mailto:%1'>%1</a></p>").arg(job.getContactEmail());
        }
        htmlContent += "</div>";
    }

    jobDescriptionEdit->setHtml(htmlContent);
    jobDescriptionEdit->setVisible(true);
    jobDescriptionEdit->setMaximumHeight(400);

    // Hide the old individual labels since we're using the comprehensive HTML
    requirementsLabel->setVisible(false);
    skillsLabel->setVisible(false);
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
    std::vector<Job> filtered;

    // Filter jobs based on the selected radio button
    if (workStudyBtn->isChecked())
    {
        // Work Study Only - show jobs eligible for work study students
        for (const auto &job : currentJobs)
        {
            if (job.isWorkStudyEligible())
            {
                filtered.push_back(job);
            }
        }
    }
    else if (intlBtn->isChecked())
    {
        // International Available - show jobs open to international students
        for (const auto &job : currentJobs)
        {
            if (isInternationalEligible(job))
            {
                filtered.push_back(job);
            }
        }
    }
    else
    {
        // All Students - show all available jobs (default filter)
        filtered = currentJobs;
    }

    // Update the display with filtered results
    updateJobList(filtered);
}

void OnCampusJobsPage::clearFilters()
{
    // Reset to default "All Students" filter
    allStudentsBtn->setChecked(true);
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
