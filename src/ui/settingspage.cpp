#include "ui/settingspage.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QSqlQuery>
#include <QDateEdit>
#include <QComboBox>
#include <QRadioButton>

SettingsPage::SettingsPage(Database *db, QWidget *parent)
    : QWidget(parent), database(db), currentUserId(-1)
{
    setupUI();
}

void SettingsPage::setUserId(int userId)
{
    currentUserId = userId;
    loadUserProfile();
}

void SettingsPage::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create tab widget with two tabs
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // === TAB 1: MY PROFILE ===
    auto *profileTab = new QWidget(this);
    auto *profileLayout = new QVBoxLayout(profileTab);
    profileLayout->setSpacing(12);
    profileLayout->setContentsMargins(16, 16, 16, 16);

    // Section 1: Personal Information
    auto *personalGroup = new QGroupBox("Personal Information", this);
    auto *personalLayout = new QVBoxLayout(personalGroup);
    personalLayout->addWidget(new QLabel("Full Name:"));
    fullNameEdit = new QLineEdit(this);
    personalLayout->addWidget(fullNameEdit);
    personalLayout->addWidget(new QLabel("Student ID:"));
    studentIdEdit = new QLineEdit(this);
    studentIdEdit->setReadOnly(true);
    personalLayout->addWidget(studentIdEdit);
    personalLayout->addWidget(new QLabel("Email Address:"));
    emailEdit = new QLineEdit(this);
    personalLayout->addWidget(emailEdit);
    personalLayout->addWidget(new QLabel("Phone Number:"));
    phoneEdit = new QLineEdit(this);
    personalLayout->addWidget(phoneEdit);
    personalLayout->addWidget(new QLabel("Address:"));
    addressEdit = new QLineEdit(this);
    personalLayout->addWidget(addressEdit);
    profileLayout->addWidget(personalGroup);

    // Section 2: Academic Information
    auto *academicGroup = new QGroupBox("Academic Information", this);
    auto *academicLayout = new QVBoxLayout(academicGroup);
    academicLayout->addWidget(new QLabel("Major:"));
    majorCombo = new QComboBox(this);
    majorCombo->addItems({"Computer Science", "Science", "Arts", "Business"});
    academicLayout->addWidget(majorCombo);
    academicLayout->addWidget(new QLabel("Current Year:"));
    currentYearCombo = new QComboBox(this);
    currentYearCombo->addItems({"Freshman", "Sophomore", "Junior", "Senior"});
    academicLayout->addWidget(currentYearCombo);
    academicLayout->addWidget(new QLabel("Expected Graduation Date:"));
    expectedGradDateEdit = new QDateEdit(this);
    expectedGradDateEdit->setDisplayFormat("MM/yyyy");
    expectedGradDateEdit->setCalendarPopup(true);
    academicLayout->addWidget(expectedGradDateEdit);
    academicLayout->addWidget(new QLabel("Current GPA:"));
    gpaEdit = new QLineEdit(this);
    academicLayout->addWidget(gpaEdit);
    academicLayout->addWidget(new QLabel("International Student:"));
    intlYesRadio = new QRadioButton("Yes", this);
    intlNoRadio = new QRadioButton("No", this);
    auto *intlRow = new QHBoxLayout();
    intlRow->addWidget(intlYesRadio);
    intlRow->addWidget(intlNoRadio);
    academicLayout->addLayout(intlRow);
    academicLayout->addWidget(new QLabel("Country of Origin:"));
    countryOfOriginEdit = new QLineEdit(this);
    academicLayout->addWidget(countryOfOriginEdit);
    profileLayout->addWidget(academicGroup);

    // Section 3: Documents
    auto *docsGroup = new QGroupBox("Documents", this);
    auto *docsLayout = new QVBoxLayout(docsGroup);
    docsLayout->addWidget(new QLabel("Resume/CV:"));
    resumePathEdit = new QLineEdit(this);
    resumePathEdit->setReadOnly(true);
    uploadResumeButton = new QPushButton("Upload Resume", this);
    viewResumeButton = new QPushButton("View", this);
    deleteResumeButton = new QPushButton("Delete", this);
    auto *resumeRow = new QHBoxLayout();
    resumeRow->addWidget(resumePathEdit, 1);
    resumeRow->addWidget(viewResumeButton);
    resumeRow->addWidget(deleteResumeButton);
    docsLayout->addLayout(resumeRow);
    docsLayout->addWidget(uploadResumeButton);

    docsLayout->addSpacing(8);
    docsLayout->addWidget(new QLabel("Transcript:"));
    transcriptPathEdit = new QLineEdit(this);
    transcriptPathEdit->setReadOnly(true);
    uploadTranscriptButton = new QPushButton("Upload Transcript", this);
    viewTranscriptButton = new QPushButton("View", this);
    deleteTranscriptButton = new QPushButton("Delete", this);
    auto *transcriptRow = new QHBoxLayout();
    transcriptRow->addWidget(transcriptPathEdit, 1);
    transcriptRow->addWidget(viewTranscriptButton);
    transcriptRow->addWidget(deleteTranscriptButton);
    docsLayout->addLayout(transcriptRow);
    docsLayout->addWidget(uploadTranscriptButton);
    profileLayout->addWidget(docsGroup);

    // Section 4: Work Preferences
    auto *workGroup = new QGroupBox("Work Preferences", this);
    auto *workLayout = new QVBoxLayout(workGroup);
    workLayout->addWidget(new QLabel("Work Study Eligible:"));
    workStudyYesRadio = new QRadioButton("Yes", this);
    workStudyNoRadio = new QRadioButton("No", this);
    auto *workRow = new QHBoxLayout();
    workRow->addWidget(workStudyYesRadio);
    workRow->addWidget(workStudyNoRadio);
    workLayout->addLayout(workRow);
    profileLayout->addWidget(workGroup);

    // Bottom buttons
    auto *profileButtons = new QHBoxLayout();
    saveProfileButton = new QPushButton("Save Changes", this);
    cancelProfileButton = new QPushButton("Cancel", this);
    profileButtons->addStretch();
    profileButtons->addWidget(saveProfileButton);
    profileButtons->addWidget(cancelProfileButton);
    profileLayout->addLayout(profileButtons);

    tabWidget->addTab(profileTab, "My Profile");

    // === TAB 2: MY ACCOUNT ===
    auto *accountTab = new QWidget(this);
    auto *accountLayout = new QVBoxLayout(accountTab);
    accountLayout->setSpacing(12);
    accountLayout->setContentsMargins(16, 16, 16, 16);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username (email)");
    usernameEdit->setReadOnly(true);

    newPasswordEdit = new QLineEdit(this);
    newPasswordEdit->setPlaceholderText("New Password");
    newPasswordEdit->setEchoMode(QLineEdit::Password);

    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setPlaceholderText("Confirm Password");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    changePasswordButton = new QPushButton("Change Password", this);

    accountLayout->addWidget(new QLabel("Username:"));
    accountLayout->addWidget(usernameEdit);
    accountLayout->addSpacing(8);
    accountLayout->addWidget(new QLabel("Change Password:"));
    accountLayout->addWidget(newPasswordEdit);
    accountLayout->addWidget(confirmPasswordEdit);
    accountLayout->addWidget(changePasswordButton);

    // Notification Settings
    auto *notifyGroup = new QGroupBox("Notification Settings", this);
    auto *notifyLayout = new QVBoxLayout(notifyGroup);
    notifyJobsCheck = new QCheckBox("Email me about new job postings", this);
    notifyApplicationsCheck = new QCheckBox("Email me about application updates", this);
    notifyInterviewsCheck = new QCheckBox("Email me about interview schedules", this);
    notifyLayout->addWidget(notifyJobsCheck);
    notifyLayout->addWidget(notifyApplicationsCheck);
    notifyLayout->addWidget(notifyInterviewsCheck);
    accountLayout->addWidget(notifyGroup);

    // Account Status
    auto *statusGroup = new QGroupBox("Account Status", this);
    auto *statusLayout = new QVBoxLayout(statusGroup);
    accountTypeLabel = new QLabel("Account Type: Student", this);
    memberSinceLabel = new QLabel("Member Since: N/A", this);
    statusLabel = new QLabel("Status: Active", this);
    statusLayout->addWidget(accountTypeLabel);
    statusLayout->addWidget(memberSinceLabel);
    statusLayout->addWidget(statusLabel);
    accountLayout->addWidget(statusGroup);

    // Bottom buttons
    auto *accountButtons = new QHBoxLayout();
    saveAccountButton = new QPushButton("Save Changes", this);
    cancelAccountButton = new QPushButton("Cancel", this);
    accountButtons->addStretch();
    accountButtons->addWidget(saveAccountButton);
    accountButtons->addWidget(cancelAccountButton);
    accountLayout->addLayout(accountButtons);

    tabWidget->addTab(accountTab, "My Account");

    // Back button to return home (toolbar context still available)
    backButton = new QPushButton("Back to Home", this);
    mainLayout->addWidget(backButton);

    // Connect signals
    connect(backButton, &QPushButton::clicked, this, [this]()
            { emit backToHomeRequested(); });
    connect(saveProfileButton, &QPushButton::clicked, this, &SettingsPage::handleSaveProfile);
    connect(changePasswordButton, &QPushButton::clicked, this, &SettingsPage::handleChangePassword);
    connect(cancelProfileButton, &QPushButton::clicked, this, [this]()
            { loadUserProfile(); });
    connect(cancelAccountButton, &QPushButton::clicked, this, [this]()
            { loadUserProfile(); });
}

void SettingsPage::loadUserProfile()
{
    if (!database || currentUserId < 0)
        return;

    User user = database->getUserDataById(currentUserId);
    // Personal
    fullNameEdit->setText(user.getFullName());
    studentIdEdit->setText(user.getEmplid());
    emailEdit->setText(user.getEmail());
    usernameEdit->setText(user.getEmail());
    addressEdit->setText("");
    phoneEdit->setText("");

    // Academic
    int majorIndex = majorCombo->findText(user.getMajor(), Qt::MatchFixedString);
    if (majorIndex >= 0)
        majorCombo->setCurrentIndex(majorIndex);
    gpaEdit->setText(user.getGpa());
    // expectedGradDate from user.getGradDate()
    QDate parsed = QDate::fromString(user.getGradDate(), "yyyy-MM-dd");
    if (!parsed.isValid())
        parsed = QDate::fromString(user.getGradDate(), "MM/yyyy");
    if (parsed.isValid())
        expectedGradDateEdit->setDate(parsed);

    // Load profile data from survey
    QSqlQuery query;
    query.prepare("SELECT is_international_student, resume_path, transcript_path, "
                  "parsed_resume_data, parsed_gpa, parsed_courses, major FROM users WHERE id = ?");
    query.addBindValue(currentUserId);

    if (query.exec() && query.next())
    {
        bool isInternational = query.value(0).toBool();
        QString resumePath = query.value(1).toString();
        QString transcriptPath = query.value(2).toString();
        QString parsedGPA = query.value(4).toString();
        QString parsedCourses = query.value(5).toString();
        QString major = query.value(6).toString();

        // International radio
        intlYesRadio->setChecked(isInternational);
        intlNoRadio->setChecked(!isInternational);
        internationalCheckbox->setChecked(isInternational);
        resumePathEdit->setText(resumePath.isEmpty() ? "No resume uploaded" : resumePath);
        transcriptPathEdit->setText(transcriptPath.isEmpty() ? "No transcript uploaded" : transcriptPath);
        gpaEdit->setText(parsedGPA);
        coursesEdit->setPlainText(parsedCourses);
        degreeEdit->setText(major);

        // Parse credits from courses if available (count courses as rough estimate)
        if (!parsedCourses.isEmpty())
        {
            int courseCount = parsedCourses.split(",").count();
            creditsTakenEdit->setText(QString::number(courseCount * 3)); // Assume 3 credits per course
        }
    }
}

void SettingsPage::handleSaveProfile()
{
    if (!database || currentUserId < 0)
        return;

    // Update profile fields
    bool isInternational = intlYesRadio->isChecked();
    QString degree = degreeEdit->text().trimmed();
    QString credits = creditsTakenEdit->text().trimmed();
    QString gpa = gpaEdit->text().trimmed();
    QString courses = coursesEdit->toPlainText().trimmed();
    QString fullName = fullNameEdit->text().trimmed();
    QString address = addressEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();

    // Update international status
    database->updateInternationalStatus(currentUserId, isInternational);

    // Update major (degree)
    QSqlQuery query;
    query.prepare("UPDATE users SET full_name = ?, major = ?, parsed_gpa = ?, parsed_courses = ?, address = ?, phone = ? WHERE id = ?");
    query.addBindValue(fullName);
    query.addBindValue(degree);
    query.addBindValue(gpa);
    query.addBindValue(courses);
    query.addBindValue(address);
    query.addBindValue(phone);
    query.addBindValue(currentUserId);

    if (query.exec())
    {
        QMessageBox::information(this, "Profile Updated",
                                 "Your profile information has been saved successfully!");
        emit profileUpdated();
    }
    else
    {
        QMessageBox::warning(this, "Update Failed",
                             "Failed to save profile changes. Please try again.");
    }
}

void SettingsPage::handleChangePassword()
{
    if (newPasswordEdit->text().isEmpty() || confirmPasswordEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Password", "Please enter and confirm your new password.");
        return;
    }

    if (newPasswordEdit->text() != confirmPasswordEdit->text())
    {
        QMessageBox::warning(this, "Password", "Passwords do not match.");
        return;
    }

    // Password change functionality to be implemented in database
    QMessageBox::information(this, "Password",
                             "Password change functionality will be implemented soon.");

    // Clear password fields
    newPasswordEdit->clear();
    confirmPasswordEdit->clear();
}
