#include "ui/myprofilepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QSqlQuery>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

MyProfilePage::MyProfilePage(Database *db, QWidget *parent)
    : QWidget(parent), database(db), currentUserId(-1)
{
    setupUI();
    setupStyles();

    // Smooth fade-in when shown
    auto *effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);
    auto *anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(350);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}

void MyProfilePage::setUserId(int userId)
{
    currentUserId = userId;
    loadUser();
}

void MyProfilePage::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create scrollable content area
    auto *scrollArea = new QScrollArea(this);
    auto *scrollWidget = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(scrollWidget);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(16, 16, 16, 16);

    // Section 1: Personal Information
    auto *personalGroup = new QGroupBox("Personal Information", this);
    auto *personalLayout = new QVBoxLayout(personalGroup);
    personalLayout->addWidget(new QLabel("Full Name:"));
    fullNameEdit = new QLineEdit(this);
    personalLayout->addWidget(fullNameEdit);
    personalLayout->addWidget(new QLabel("Student ID:"));
    emplidEdit = new QLineEdit(this);
    emplidEdit->setReadOnly(true);
    personalLayout->addWidget(emplidEdit);
    personalLayout->addWidget(new QLabel("Email Address:"));
    emailEdit = new QLineEdit(this);
    personalLayout->addWidget(emailEdit);

    personalLayout->addSpacing(10);
    personalLayout->addWidget(new QLabel("International Student:"));
    intlYesRadio = new QRadioButton("Yes", this);
    intlNoRadio = new QRadioButton("No", this);
    intlNoRadio->setChecked(true);
    auto *intlRow = new QHBoxLayout();
    intlRow->addWidget(intlYesRadio);
    intlRow->addWidget(intlNoRadio);
    intlRow->addStretch();
    personalLayout->addLayout(intlRow);

    personalLayout->addSpacing(10);
    personalLayout->addWidget(new QLabel("Work Study Eligible:"));
    workStudyYesRadio = new QRadioButton("Yes", this);
    workStudyNoRadio = new QRadioButton("No", this);
    workStudyNoRadio->setChecked(true);
    auto *workRow = new QHBoxLayout();
    workRow->addWidget(workStudyYesRadio);
    workRow->addWidget(workStudyNoRadio);
    workRow->addStretch();
    personalLayout->addLayout(workRow);

    contentLayout->addWidget(personalGroup);

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
    gradDateEdit = new QDateEdit(this);
    gradDateEdit->setDisplayFormat("MM/yyyy");
    gradDateEdit->setCalendarPopup(true);
    academicLayout->addWidget(gradDateEdit);
    academicLayout->addWidget(new QLabel("Current GPA:"));
    gpaEdit = new QLineEdit(this);
    academicLayout->addWidget(gpaEdit);
    academicLayout->addWidget(new QLabel("Degree/Major:"));
    degreeEdit = new QLineEdit(this);
    academicLayout->addWidget(degreeEdit);
    academicLayout->addWidget(new QLabel("Courses Completed:"));
    coursesEdit = new QTextEdit(this);
    coursesEdit->setMaximumHeight(80);
    academicLayout->addWidget(coursesEdit);
    academicLayout->addWidget(new QLabel("Credits Taken:"));
    creditsTakenEdit = new QLineEdit(this);
    creditsTakenEdit->setReadOnly(true);
    academicLayout->addWidget(creditsTakenEdit);
    contentLayout->addWidget(academicGroup);

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
    contentLayout->addWidget(docsGroup);

    // Bottom buttons
    auto *profileButtons = new QHBoxLayout();
    saveButton = new QPushButton("Save Changes", this);
    cancelButton = new QPushButton("Cancel", this);
    profileButtons->addStretch();
    profileButtons->addWidget(saveButton);
    profileButtons->addWidget(cancelButton);
    contentLayout->addLayout(profileButtons);

    contentLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    connect(saveButton, &QPushButton::clicked, this, &MyProfilePage::handleSaveProfile);
    connect(cancelButton, &QPushButton::clicked, this, [this]()
            { loadUser(); });
}

void MyProfilePage::setupStyles()
{
    setStyleSheet(
        "QWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #f8f9fa);"
        "  font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QGroupBox { font-weight: 600; font-size: 14px; border: 2px solid #BDBDBD; border-radius: 10px;"
        "  margin-top: 14px; padding-top: 12px; background: white; }"
        "QGroupBox::title { left: 15px; padding: 0 8px; color: #1976D2; background: white; }"
        "QLineEdit, QDateEdit, QComboBox { padding: 12px; border: 2px solid #E0E0E0; border-radius: 8px; background: white; font-size: 14px; }"
        "QLineEdit:focus, QDateEdit:focus, QComboBox:focus { border: 2px solid #2196F3; background: #f8fcff; }"
        "QTextEdit { padding: 8px; border: 2px solid #E0E0E0; border-radius: 8px; background: white; font-size: 14px; }"
        "QTextEdit:focus { border: 2px solid #2196F3; background: #f8fcff; }"
        "QPushButton { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #2196F3, stop:1 #1976D2); color:white;"
        "  padding: 12px 24px; border-radius: 8px; font-weight: 600; font-size: 14px; border: none; min-height: 40px; }"
        "QPushButton:hover { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #1976D2, stop:1 #0D47A1); }");
}

void MyProfilePage::loadUser()
{
    if (!database || currentUserId < 0)
        return;

    User user = database->getUserDataById(currentUserId);
    // Personal
    fullNameEdit->setText(user.getFullName());
    emplidEdit->setText(user.getEmplid());
    emailEdit->setText(user.getEmail());

    // Academic
    int majorIndex = majorCombo->findText(user.getMajor(), Qt::MatchFixedString);
    if (majorIndex >= 0)
        majorCombo->setCurrentIndex(majorIndex);
    gpaEdit->setText(user.getGpa());
    QDate parsed = QDate::fromString(user.getGradDate(), "yyyy-MM-dd");
    if (!parsed.isValid())
        parsed = QDate::fromString(user.getGradDate(), "MM/yyyy");
    if (parsed.isValid())
        gradDateEdit->setDate(parsed);

    // Load profile data from survey
    QSqlQuery query;
    query.prepare("SELECT is_international_student, resume_path, transcript_path, "
                  "parsed_gpa, parsed_courses, major FROM users WHERE id = ?");
    query.addBindValue(currentUserId);

    if (query.exec() && query.next())
    {
        bool isInternational = query.value(0).toBool();
        QString resumePath = query.value(1).toString();
        QString transcriptPath = query.value(2).toString();
        QString parsedGPA = query.value(3).toString();
        QString parsedCourses = query.value(4).toString();
        QString major = query.value(5).toString();

        // International and Work Study status
        intlYesRadio->setChecked(isInternational);
        intlNoRadio->setChecked(!isInternational);
        resumePathEdit->setText(resumePath.isEmpty() ? "No resume uploaded" : resumePath);
        transcriptPathEdit->setText(transcriptPath.isEmpty() ? "No transcript uploaded" : transcriptPath);
        gpaEdit->setText(parsedGPA);
        coursesEdit->setPlainText(parsedCourses);
        degreeEdit->setText(major);

        // Parse credits from courses if available
        if (!parsedCourses.isEmpty())
        {
            int courseCount = parsedCourses.split(",").count();
            creditsTakenEdit->setText(QString::number(courseCount * 3));
        }
    }
}

void MyProfilePage::handleSaveProfile()
{
    if (!database || currentUserId < 0)
        return;

    // Update profile fields
    bool isInternational = intlYesRadio->isChecked();
    bool isWorkStudy = workStudyYesRadio->isChecked();
    QString degree = degreeEdit->text().trimmed();
    QString gpa = gpaEdit->text().trimmed();
    QString courses = coursesEdit->toPlainText().trimmed();
    QString fullName = fullNameEdit->text().trimmed();

    // Update international status
    database->updateInternationalStatus(currentUserId, isInternational);

    // Update basic info
    QSqlQuery query;
    query.prepare("UPDATE users SET full_name = ?, parsed_gpa = ? WHERE id = ?");
    query.addBindValue(fullName);
    query.addBindValue(gpa);
    query.addBindValue(currentUserId);

    if (query.exec())
    {
        QMessageBox::information(this, "Profile Updated",
                                 "Your profile information has been saved successfully!");
        emit profileSaved();
    }
    else
    {
        QMessageBox::warning(this, "Update Failed",
                             "Failed to save profile changes. Please try again.");
    }
}
