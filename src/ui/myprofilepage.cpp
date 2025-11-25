#include "ui/myprofilepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
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

void MyProfilePage::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Personal Information
    auto personalGroup = new QGroupBox("Personal Information", this);
    auto personalLayout = new QVBoxLayout(personalGroup);

    nameLabel = new QLabel("", this);
    nameLabel->setStyleSheet("font-size: 18px; font-weight: 600; color: #1976D2;");
    personalLayout->addWidget(nameLabel);

    fullNameEdit = new QLineEdit(this);
    emplidEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    majorEdit = new QLineEdit(this);
    gpaEdit = new QLineEdit(this);
    gradDateEdit = new QDateEdit(this);
    gradDateEdit->setDisplayFormat("MM/yyyy");
    gradDateEdit->setCalendarPopup(true);

    fullNameEdit->setPlaceholderText("Full Name");
    emplidEdit->setPlaceholderText("EMPLID");
    emailEdit->setPlaceholderText("Email");
    addressEdit->setPlaceholderText("Address (optional)");
    majorEdit->setPlaceholderText("Major");
    gpaEdit->setPlaceholderText("GPA 0.00-4.00");

    emailEdit->setReadOnly(true);
    emplidEdit->setReadOnly(true);

    personalLayout->addWidget(new QLabel("Full Name:"));
    personalLayout->addWidget(fullNameEdit);
    personalLayout->addWidget(new QLabel("EMPLID:"));
    personalLayout->addWidget(emplidEdit);
    personalLayout->addWidget(new QLabel("Email:"));
    personalLayout->addWidget(emailEdit);
    personalLayout->addWidget(new QLabel("Address:"));
    personalLayout->addWidget(addressEdit);
    personalLayout->addWidget(new QLabel("Major:"));
    personalLayout->addWidget(majorEdit);
    personalLayout->addWidget(new QLabel("GPA:"));
    personalLayout->addWidget(gpaEdit);
    personalLayout->addWidget(new QLabel("Expected Graduation:"));
    personalLayout->addWidget(gradDateEdit);

    mainLayout->addWidget(personalGroup);

    // Account Information
    auto accountGroup = new QGroupBox("Login / Account Information", this);
    auto accountLayout = new QVBoxLayout(accountGroup);

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
    saveButton = new QPushButton("Save Changes", this);

    accountLayout->addWidget(new QLabel("Username:"));
    accountLayout->addWidget(usernameEdit);
    accountLayout->addSpacing(8);
    accountLayout->addWidget(new QLabel("Change Password:"));
    accountLayout->addWidget(newPasswordEdit);
    accountLayout->addWidget(confirmPasswordEdit);
    auto btnRow = new QHBoxLayout();
    btnRow->addWidget(changePasswordButton);
    btnRow->addStretch();
    btnRow->addWidget(saveButton);
    accountLayout->addLayout(btnRow);

    mainLayout->addWidget(accountGroup);

    connect(saveButton, &QPushButton::clicked, this, &MyProfilePage::handleSaveProfile);
    connect(changePasswordButton, &QPushButton::clicked, this, &MyProfilePage::handleChangePassword);
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
        "QLineEdit, QDateEdit { padding: 12px; border: 2px solid #E0E0E0; border-radius: 8px; background: white; font-size: 14px; }"
        "QLineEdit:focus, QDateEdit:focus { border: 2px solid #2196F3; background: #f8fcff; }"
        "QPushButton { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #2196F3, stop:1 #1976D2); color:white;"
        "  padding: 12px 24px; border-radius: 8px; font-weight: 600; font-size: 14px; border: none; min-height: 40px; }"
        "QPushButton:hover { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #1976D2, stop:1 #0D47A1); }");
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
    usernameEdit->setText(u.getEmail());

    majorEdit->setText(u.getMajor());
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
    const QString major = majorEdit->text().trimmed();
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
    // No password update API yet; placeholder
    QMessageBox::information(this, "Password", "Password change is not implemented yet in the database layer.");
}
