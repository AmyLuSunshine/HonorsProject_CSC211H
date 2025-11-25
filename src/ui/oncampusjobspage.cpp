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
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create splitter for resizable panels
    auto splitter = new QSplitter(Qt::Horizontal, this);

    // LEFT PANEL - Categories (25%)
    auto leftPanel = new QWidget();
    auto leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(10, 10, 10, 10);

    auto categoryLabel = new QLabel("Job Categories");
    categoryLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: #1976D2; padding: 10px;");
    leftLayout->addWidget(categoryLabel);

    categoryTree = new QTreeWidget(this);
    categoryTree->setHeaderHidden(true);
    categoryTree->setAnimated(true);
    connect(categoryTree, &QTreeWidget::itemClicked, this, &OnCampusJobsPage::onCategorySelected);
    leftLayout->addWidget(categoryTree);

    leftPanel->setMinimumWidth(250);
    leftPanel->setMaximumWidth(400);

    // CENTER PANEL - Search & Job List (50%)
    auto centerPanel = new QWidget();
    auto centerLayout = new QVBoxLayout(centerPanel);
    centerLayout->setContentsMargins(10, 10, 10, 10);

    // Search bar
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Search jobs by title, department, or keyword...");
    searchEdit->setMinimumHeight(40);
    connect(searchEdit, &QLineEdit::textChanged, this, &OnCampusJobsPage::onSearchTextChanged);
    centerLayout->addWidget(searchEdit);

    // Filters
    auto filterLayout = new QHBoxLayout();

    statusFilter = new QComboBox(this);
    statusFilter->addItem("All Status", "");
    statusFilter->addItem("Available", "0");
    statusFilter->addItem("Closed", "2");
    statusFilter->addItem("Upcoming", "3");
    connect(statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OnCampusJobsPage::onFilterChanged);

    auto eligibilityLabel = new QLabel("Eligibility:", this);
    eligibilityFilter = new QComboBox(this);
    eligibilityFilter->addItem("All Students", 0);
    eligibilityFilter->addItem("Work Study Only", 1);
    eligibilityFilter->addItem("International students available", 2);
    connect(eligibilityFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OnCampusJobsPage::onFilterChanged);

    clearFiltersBtn = new QPushButton("Clear Filters", this);
    connect(clearFiltersBtn, &QPushButton::clicked, this, &OnCampusJobsPage::clearFilters);

    filterLayout->addWidget(statusFilter);
    filterLayout->addWidget(eligibilityLabel);
    filterLayout->addWidget(eligibilityFilter);
    filterLayout->addStretch();
    filterLayout->addWidget(clearFiltersBtn);
    centerLayout->addLayout(filterLayout);

    // Results count
    resultsCountLabel = new QLabel("Showing 0 jobs");
    resultsCountLabel->setStyleSheet("color: #757575; padding: 5px;");
    centerLayout->addWidget(resultsCountLabel);

    // Job list
    jobListWidget = new QListWidget(this);
    jobListWidget->setSpacing(8);
    connect(jobListWidget, &QListWidget::itemClicked, this, &OnCampusJobsPage::onJobCardClicked);
    centerLayout->addWidget(jobListWidget);

    centerPanel->setMinimumWidth(400);

    // RIGHT PANEL - Job Details (25%)
    detailScrollArea = new QScrollArea(this);
    detailPanel = new QWidget();
    auto detailLayout = new QVBoxLayout(detailPanel);
    detailLayout->setContentsMargins(15, 15, 15, 15);

    // Job header
    jobTitleLabel = new QLabel("Select a job to view details");
    jobTitleLabel->setStyleSheet("font-size: 18px; font-weight: 600; color: #1976D2;");
    jobTitleLabel->setWordWrap(true);
    detailLayout->addWidget(jobTitleLabel);

    jobDepartmentLabel = new QLabel("");
    jobDepartmentLabel->setStyleSheet("font-size: 13px; color: #757575;");
    detailLayout->addWidget(jobDepartmentLabel);

    statusBadgeLabel = new QLabel("");
    detailLayout->addWidget(statusBadgeLabel);

    // Action buttons
    auto actionLayout = new QHBoxLayout();
    saveJobButton = new QPushButton("💾 Save Job", this);
    saveJobButton->setVisible(false);
    connect(saveJobButton, &QPushButton::clicked, this, &OnCampusJobsPage::toggleSaveJob);

    actionButton = new QPushButton("Apply Now", this);
    actionButton->setVisible(false);
    connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
    actionLayout->addWidget(saveJobButton);
    actionLayout->addStretch();
    actionLayout->addWidget(actionButton);
    detailLayout->addLayout(actionLayout);

    detailLayout->addSpacing(10);

    // Quick facts
    quickFactsWidget = new QWidget();
    quickFactsWidget->setVisible(false);
    auto factsLayout = new QVBoxLayout(quickFactsWidget);
    factsLayout->setContentsMargins(0, 0, 0, 0);
    detailLayout->addWidget(quickFactsWidget);

    // Description
    jobDescriptionEdit = new QTextBrowser(this);
    jobDescriptionEdit->setVisible(false);
    jobDescriptionEdit->setOpenExternalLinks(false);
    detailLayout->addWidget(jobDescriptionEdit);

    // Requirements
    requirementsEdit = new QTextBrowser(this);
    requirementsEdit->setVisible(false);
    detailLayout->addWidget(requirementsEdit);

    // Duties
    dutiesEdit = new QTextBrowser(this);
    dutiesEdit->setVisible(false);
    detailLayout->addWidget(dutiesEdit);

    // Skills
    skillsLabel = new QLabel("");
    skillsLabel->setVisible(false);
    skillsLabel->setWordWrap(true);
    detailLayout->addWidget(skillsLabel);

    // Supervisor
    supervisorLabel = new QLabel("");
    supervisorLabel->setVisible(false);
    supervisorLabel->setWordWrap(true);
    detailLayout->addWidget(supervisorLabel);

    detailLayout->addStretch();

    detailPanel->setLayout(detailLayout);
    detailScrollArea->setWidget(detailPanel);
    detailScrollArea->setWidgetResizable(true);
    detailScrollArea->setMinimumWidth(300);

    // Add panels to splitter
    splitter->addWidget(leftPanel);
    splitter->addWidget(centerPanel);
    splitter->addWidget(detailScrollArea);
    splitter->setStretchFactor(0, 1); // 25%
    splitter->setStretchFactor(1, 2); // 50%
    splitter->setStretchFactor(2, 1); // 25%

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
        "QListWidget { background: white; border: 1px solid #E0E0E0; border-radius: 8px; }"
        "QListWidget::item { border-left: 3px solid transparent; padding: 10px; margin: 2px; background: white; border-radius: 6px; }"
        "QListWidget::item:hover { border-left: 3px solid #2196F3; background: #F5F5F5; }"
        "QListWidget::item:selected { border-left: 3px solid #2196F3; background: #E3F2FD; }"
        "QTreeWidget { background: white; border: 1px solid #E0E0E0; border-radius: 8px; }"
        "QTextBrowser { background: white; border: 1px solid #E0E0E0; border-radius: 6px; padding: 10px; }");
}

void OnCampusJobsPage::loadCategories()
{
    categoryTree->clear();

    // All jobs shortcut
    auto allItem = new QTreeWidgetItem(categoryTree);
    allItem->setText(0, "📋 All Jobs");
    allItem->setData(0, Qt::UserRole, "");

    // Academic Support group with children
    int academicCount = database->getJobCountByCategory("Academic Support");
    auto academicItem = new QTreeWidgetItem(categoryTree);
    academicItem->setText(0, QString("%1 Academic Support (%2)").arg(getCategoryIcon("Academic Support")).arg(academicCount));
    academicItem->setData(0, Qt::UserRole, "Academic Support");
    QStringList academicRoles = {"Tutor", "SI - Supplemental Instructor", "Peer Mentor", "Research Assistant"};
    for (const auto &role : academicRoles)
    {
        auto child = new QTreeWidgetItem(academicItem);
        child->setText(0, role);
        child->setData(0, Qt::UserRole, "Academic Support"); // category filter
        child->setData(0, Qt::UserRole + 1, role);           // keyword filter
    }

    // Administrative group with children (merging Library, Food, Fitness)
    int adminCount = database->getJobCountByCategory("Administrative") + database->getJobCountByCategory("Library Services") + database->getJobCountByCategory("Food Services") + database->getJobCountByCategory("Recreation & Fitness");
    auto adminItem = new QTreeWidgetItem(categoryTree);
    adminItem->setText(0, QString("%1 Administrative (%2)").arg(getCategoryIcon("Administrative")).arg(adminCount));
    adminItem->setData(0, Qt::UserRole, "Administrative_GROUP"); // special marker

    QStringList adminRoles = {"College Assistant", "Data Entry Assistant", "Student Ambassador"};
    for (const auto &role : adminRoles)
    {
        auto child = new QTreeWidgetItem(adminItem);
        child->setText(0, role);
        child->setData(0, Qt::UserRole, "Administrative");
        child->setData(0, Qt::UserRole + 1, role);
    }
    // Moved categories as children under Administrative
    for (const QString &movedCat : {QString("Library Services"), QString("Food Services"), QString("Recreation & Fitness")})
    {
        auto child = new QTreeWidgetItem(adminItem);
        int count = database->getJobCountByCategory(movedCat);
        child->setText(0, QString("%1 (%2)").arg(movedCat).arg(count));
        child->setData(0, Qt::UserRole, movedCat);
        child->setData(0, Qt::UserRole + 1, "");
    }

    // Keep Technology and Student Services as top-level
    for (const QString &top : {QString("Technology"), QString("Student Services")})
    {
        int count = database->getJobCountByCategory(top);
        auto item = new QTreeWidgetItem(categoryTree);
        item->setText(0, QString("%1 %2 (%3)").arg(getCategoryIcon(top)).arg(top).arg(count));
        item->setData(0, Qt::UserRole, top);
    }
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

    resultsCountLabel->setText(QString("Showing %1 jobs").arg(jobs.size()));

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
                               "<div style='padding:5px;'>"
                               "<div style='font-size:15px; font-weight:600; color:#212121;'>%1</div>"
                               "<div style='font-size:12px; color:#757575; margin-top:2px;'>%2</div>"
                               "<div style='margin-top:5px;'>%3</div>"
                               "<div style='font-size:13px; color:#424242; margin-top:5px;'>"
                               "💰 %4 | ⏰ %5%6%7</div>"
                               "<div style='font-size:12px; color:#616161; margin-top:6px;'>%8</div>"
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

    jobTitleLabel->setText(job.getTitle());
    jobTitleLabel->setVisible(true);

    jobDepartmentLabel->setText(job.getDepartment());
    jobDepartmentLabel->setVisible(true);

    statusBadgeLabel->setText(getStatusBadge(job.getStatus(), job.getPositionsAvailable()));
    statusBadgeLabel->setVisible(true);

    // Update action button
    if (job.getStatus() == JobStatus::Open)
    {
        actionButton->setText("Apply Now");
        actionButton->setStyleSheet("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #4CAF50, stop:1 #388E3C); color:white; padding:10px; border-radius:6px; font-weight:600;");
        disconnect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::expressInterest);
        connect(actionButton, &QPushButton::clicked, this, &OnCampusJobsPage::applyForJob);
    }
    else
    {
        actionButton->setText("Express Interest");
        actionButton->setStyleSheet("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #2196F3, stop:1 #1976D2); color:white; padding:10px; border-radius:6px; font-weight:600;");
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
    QString keyword = searchEdit->text();
    QString statusFilterVal = statusFilter->currentData().toString();
    int eligibility = eligibilityFilter->currentData().toInt();
    bool workStudyOnly = (eligibility == 1);
    bool internationalOnly = (eligibility == 2);

    // With no hours filter, pass broad defaults
    int minHours = 0;
    int maxHours = 100;
    auto jobs = database->searchJobs(keyword, statusFilterVal, "", workStudyOnly, minHours, maxHours, 0, 100);
    if (internationalOnly)
    {
        std::vector<Job> filtered;
        for (const auto &j : jobs)
        {
            if (isInternationalEligible(j))
                filtered.push_back(j);
        }
        updateJobList(filtered);
    }
    else
    {
        updateJobList(jobs);
    }
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
