#include "ui/myprofilepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QScrollArea>

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

void MyProfilePage::setupUI()
{
    // Top-level layout contains a scroll area to prevent crowding
    auto rootLayout = new QVBoxLayout(this);
    rootLayout->setSpacing(0);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    rootLayout->addWidget(scrollArea);

    // Content widget inside the scroll area
    auto contentWidget = new QWidget(scrollArea);
    auto mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    scrollArea->setWidget(contentWidget);

    // SECTION 1: Personal Information
    auto personalGroup = new QGroupBox("Personal Information", this);
    auto personalLayout = new QVBoxLayout(personalGroup);

    nameLabel = new QLabel("", this);
    personalLayout->addWidget(nameLabel);

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

    personalLayout->addWidget(new QLabel("Phone Number:"));
    phoneEdit = new QLineEdit(this);
    personalLayout->addWidget(phoneEdit);

    personalLayout->addWidget(new QLabel("Address:"));
    addressEdit = new QLineEdit(this);
    personalLayout->addWidget(addressEdit);

    mainLayout->addWidget(personalGroup);

    // SECTION 2: Academic Information
    auto academicGroup = new QGroupBox("Academic Information", this);
    auto academicLayout = new QVBoxLayout(academicGroup);
    academicLayout->addWidget(new QLabel("Major:"));
    majorCombo = new QComboBox(this);
    majorCombo->addItems({"Computer Science", "Science", "Arts", "Business"});
    academicLayout->addWidget(majorCombo);

    academicLayout->addWidget(new QLabel("Current Year:"));
    currentYearCombo = new QComboBox(this);
    currentYearCombo->addItems({"Freshman", "Sophomore"});
    academicLayout->addWidget(currentYearCombo);

    academicLayout->addWidget(new QLabel("Expected Graduation Date:"));
    gradDateEdit = new QDateEdit(this);
    gradDateEdit->setDisplayFormat("MM/yyyy");
    gradDateEdit->setCalendarPopup(true);
    academicLayout->addWidget(gradDateEdit);

    academicLayout->addWidget(new QLabel("Current GPA:"));
    gpaEdit = new QLineEdit(this);
    academicLayout->addWidget(gpaEdit);

    academicLayout->addWidget(new QLabel("International Student:"));
    intlYesRadio = new QRadioButton("Yes", this);
    intlNoRadio = new QRadioButton("No", this);
    auto intlRow = new QHBoxLayout();
    intlRow->addWidget(intlYesRadio);
    intlRow->addWidget(intlNoRadio);
    academicLayout->addLayout(intlRow);

    academicLayout->addWidget(new QLabel("Country of Origin:"));
    countryOfOriginEdit = new QLineEdit(this);
    academicLayout->addWidget(countryOfOriginEdit);

    mainLayout->addWidget(academicGroup);

    // SECTION 3: Documents
    auto docsGroup = new QGroupBox("Documents", this);
    auto docsLayout = new QVBoxLayout(docsGroup);

    // Resume
    docsLayout->addWidget(new QLabel("Resume/CV:"));
    resumePathEdit = new QLineEdit(this);
    resumePathEdit->setReadOnly(true);
    auto resumeRow = new QHBoxLayout();
    viewResumeButton = new QPushButton("View", this);
    deleteResumeButton = new QPushButton("Delete", this);
    uploadResumeButton = new QPushButton("Upload Resume", this);
    resumeRow->addWidget(resumePathEdit, 1);
    resumeRow->addWidget(viewResumeButton);
    resumeRow->addWidget(deleteResumeButton);
    docsLayout->addLayout(resumeRow);
    docsLayout->addWidget(uploadResumeButton);

    // Transcript
    docsLayout->addWidget(new QLabel("Transcript:"));
    transcriptPathEdit = new QLineEdit(this);
    transcriptPathEdit->setReadOnly(true);
    auto transcriptRow = new QHBoxLayout();
    viewTranscriptButton = new QPushButton("View", this);
    deleteTranscriptButton = new QPushButton("Delete", this);
    uploadTranscriptButton = new QPushButton("Upload Transcript", this);
    transcriptRow->addWidget(transcriptPathEdit, 1);
    transcriptRow->addWidget(viewTranscriptButton);
    transcriptRow->addWidget(deleteTranscriptButton);
    docsLayout->addLayout(transcriptRow);
    docsLayout->addWidget(uploadTranscriptButton);

    mainLayout->addWidget(docsGroup);

    // SECTION 4: Work Preferences
    auto workGroup = new QGroupBox("Work Preferences", this);
    auto workLayout = new QVBoxLayout(workGroup);
    workLayout->addWidget(new QLabel("Work Study Eligible:"));
    workStudyYesRadio = new QRadioButton("Yes", this);
    workStudyNoRadio = new QRadioButton("No", this);
    auto workRow = new QHBoxLayout();
    workRow->addWidget(workStudyYesRadio);
    workRow->addWidget(workStudyNoRadio);
    workLayout->addLayout(workRow);
    mainLayout->addWidget(workGroup);

    // Bottom buttons
    auto btnRow = new QHBoxLayout();
    saveButton = new QPushButton("Save Changes", this);
    cancelButton = new QPushButton("Cancel", this);
    btnRow->addStretch();
    btnRow->addWidget(saveButton);
    btnRow->addWidget(cancelButton);
    mainLayout->addLayout(btnRow);

    // Simple connections
    connect(saveButton, &QPushButton::clicked, this, &MyProfilePage::handleSaveProfile);
    connect(cancelButton, &QPushButton::clicked, this, [this]()
            { loadUser(); });
    connect(viewResumeButton, &QPushButton::clicked, this, [this]()
            { QMessageBox::information(this, "Resume", "Viewing resume not implemented."); });
    connect(deleteResumeButton, &QPushButton::clicked, this, [this]()
            { resumePathEdit->clear(); });
    connect(uploadResumeButton, &QPushButton::clicked, this, [this]()
            { QMessageBox::information(this, "Resume", "Upload resume not implemented."); });
    connect(viewTranscriptButton, &QPushButton::clicked, this, [this]()
            { QMessageBox::information(this, "Transcript", "Viewing transcript not implemented."); });
    connect(deleteTranscriptButton, &QPushButton::clicked, this, [this]()
            { transcriptPathEdit->clear(); });
    connect(uploadTranscriptButton, &QPushButton::clicked, this, [this]()
            { QMessageBox::information(this, "Transcript", "Upload transcript not implemented."); });
}

void MyProfilePage::setupStyles()
{
    setStyleSheet(
        "QWidget {"
        "  background: #ffffff;"
        "  font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QGroupBox { font-weight: 600; font-size: 14px; border: 1px solid #BDBDBD; border-radius: 6px;"
        "  margin-top: 10px; padding-top: 8px; background: white; }"
        "QGroupBox::title { left: 12px; padding: 0 6px; color: #1976D2; background: white; }"
        "QLineEdit, QDateEdit, QComboBox { padding: 8px; border: 1px solid #E0E0E0; border-radius: 4px; background: white; font-size: 14px; }"
        "QLineEdit:focus, QDateEdit:focus, QComboBox:focus { border: 1px solid #2196F3; background: #f8fcff; }"
        "QPushButton { background: #1976D2; color:white; padding: 8px 16px; border-radius: 4px; font-weight: 600; font-size: 14px; border: none; min-height: 36px; }"
        "QPushButton:hover { background: #1565C0; }");
}

void MyProfilePage::setUserId(int userId)
{
    currentUserId = userId;
    loadUser();
}

void MyProfilePage::loadUser()
{
    if (!database || currentUserId < 0)
        return;
    User u = database->getUserDataById(currentUserId);
    nameLabel->setText(u.getFullName());
    fullNameEdit->setText(u.getFullName());
    emplidEdit->setText(u.getEmplid());
    emailEdit->setText(u.getEmail());
    addressEdit->setText("");
    phoneEdit->setText("");

    // Academic
    int majorIndex = majorCombo->findText(u.getMajor(), Qt::MatchFixedString);
    if (majorIndex >= 0)
        majorCombo->setCurrentIndex(majorIndex);
    gpaEdit->setText(u.getGpa());
    // gradDate stored as yyyy-MM-dd; show month/year if parseable
    QDate parsed = QDate::fromString(u.getGradDate(), "yyyy-MM-dd");
    if (!parsed.isValid())
        parsed = QDate::fromString(u.getGradDate(), "MM/yyyy");
    if (parsed.isValid())
        gradDateEdit->setDate(parsed);
}

void MyProfilePage::handleSaveProfile()
{
    if (!database || currentUserId < 0)
        return;
    // Gather edits
    const QString major = majorCombo->currentText().trimmed();
    const QString gpa = gpaEdit->text().trimmed();
    const QString grad = gradDateEdit->date().toString("yyyy-MM-dd");
    bool ok = database->updateProfileById(currentUserId, major, gpa, grad);
    if (ok)
    {
        QMessageBox::information(this, "Profile", "Profile updated successfully.");
        emit profileSaved();
    }
    else
    {
        QMessageBox::warning(this, "Profile", "Failed to update profile.");
    }
}

void MyProfilePage::handleChangePassword()
{
    QMessageBox::information(this, "My Account", "Login and password settings moved to Settings > My Account.");
}
