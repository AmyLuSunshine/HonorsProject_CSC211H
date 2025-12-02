#include "ui/myaccountpage.h"
#include <QMessageBox>

MyAccountPage::MyAccountPage(Database *db, QWidget *parent)
    : QWidget(parent), database(db), currentUserId(-1)
{
    setupUI();
}

void MyAccountPage::setUserId(int userId)
{
    currentUserId = userId;
    loadAccountData();
}

void MyAccountPage::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // Title
    auto *title = new QLabel("My Account", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    mainLayout->addWidget(title);

    // Section 1: Login Information
    auto *loginGroup = new QGroupBox("Login Information", this);
    auto *loginLayout = new QVBoxLayout(loginGroup);
    loginLayout->addWidget(new QLabel("Username:"));
    usernameEdit = new QLineEdit(this);
    usernameEdit->setReadOnly(false);
    loginLayout->addWidget(usernameEdit);
    loginLayout->addWidget(new QLabel("Email:"));
    emailEdit = new QLineEdit(this);
    emailEdit->setReadOnly(true);
    loginLayout->addWidget(emailEdit);

    changePasswordButton = new QPushButton("Change Password", this);
    loginLayout->addWidget(changePasswordButton);

    // Simple inline password change area (hidden until clicked)
    currentPasswordEdit = new QLineEdit(this);
    currentPasswordEdit->setPlaceholderText("Current Password");
    currentPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit = new QLineEdit(this);
    newPasswordEdit->setPlaceholderText("New Password");
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setPlaceholderText("Confirm Password");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    updatePasswordButton = new QPushButton("Update", this);
    cancelPasswordButton = new QPushButton("Cancel", this);

    auto *pwRow = new QHBoxLayout();
    pwRow->addWidget(updatePasswordButton);
    pwRow->addWidget(cancelPasswordButton);

    loginLayout->addWidget(currentPasswordEdit);
    loginLayout->addWidget(newPasswordEdit);
    loginLayout->addWidget(confirmPasswordEdit);
    loginLayout->addLayout(pwRow);

    // Initially hide password fields
    currentPasswordEdit->hide();
    newPasswordEdit->hide();
    confirmPasswordEdit->hide();
    updatePasswordButton->hide();
    cancelPasswordButton->hide();

    mainLayout->addWidget(loginGroup);

    // Section 2: Notification Settings
    auto *notifyGroup = new QGroupBox("Notification Settings", this);
    auto *notifyLayout = new QVBoxLayout(notifyGroup);
    notifyJobsCheck = new QCheckBox("Email me about new job postings", this);
    notifyApplicationsCheck = new QCheckBox("Email me about application updates", this);
    notifyInterviewsCheck = new QCheckBox("Email me about interview schedules", this);
    notifyLayout->addWidget(notifyJobsCheck);
    notifyLayout->addWidget(notifyApplicationsCheck);
    notifyLayout->addWidget(notifyInterviewsCheck);
    mainLayout->addWidget(notifyGroup);

    // Section 3: Account Status
    auto *statusGroup = new QGroupBox("Account Status", this);
    auto *statusLayout = new QVBoxLayout(statusGroup);
    accountTypeLabel = new QLabel("Account Type: Student", this);
    memberSinceLabel = new QLabel("Member Since: N/A", this);
    statusLabel = new QLabel("Status: Active", this);
    statusLayout->addWidget(accountTypeLabel);
    statusLayout->addWidget(memberSinceLabel);
    statusLayout->addWidget(statusLabel);
    mainLayout->addWidget(statusGroup);

    // Bottom Buttons
    auto *btnRow = new QHBoxLayout();
    saveButton = new QPushButton("Save Changes", this);
    cancelButton = new QPushButton("Cancel", this);
    backButton = new QPushButton("Back", this);
    btnRow->addStretch();
    btnRow->addWidget(saveButton);
    btnRow->addWidget(cancelButton);
    btnRow->addWidget(backButton);
    mainLayout->addLayout(btnRow);

    // Connections
    connect(changePasswordButton, &QPushButton::clicked, this, [this]()
            {
        currentPasswordEdit->show();
        newPasswordEdit->show();
        confirmPasswordEdit->show();
        updatePasswordButton->show();
        cancelPasswordButton->show(); });
    connect(cancelPasswordButton, &QPushButton::clicked, this, [this]()
            {
        currentPasswordEdit->hide();
        newPasswordEdit->hide();
        confirmPasswordEdit->hide();
        updatePasswordButton->hide();
        cancelPasswordButton->hide(); });

    connect(updatePasswordButton, &QPushButton::clicked, this, &MyAccountPage::handleChangePassword);
    connect(saveButton, &QPushButton::clicked, this, &MyAccountPage::handleSaveAccount);
    connect(backButton, &QPushButton::clicked, this, [this]()
            { emit backToHomeRequested(); });
}

void MyAccountPage::loadAccountData()
{
    if (!database || currentUserId < 0)
        return;
    User user = database->getUserDataById(currentUserId);
    usernameEdit->setText(user.getEmail());
    emailEdit->setText(user.getEmail());
    // TODO: Load notifications from DB when fields are available
}

void MyAccountPage::handleChangePassword()
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
    QMessageBox::information(this, "Password", "Password update to be implemented.");
    currentPasswordEdit->clear();
    newPasswordEdit->clear();
    confirmPasswordEdit->clear();
}

void MyAccountPage::handleSaveAccount()
{
    // Placeholder save; wire to DB when preferences exist
    QMessageBox::information(this, "Account", "Account settings saved.");
    emit accountSaved();
}
