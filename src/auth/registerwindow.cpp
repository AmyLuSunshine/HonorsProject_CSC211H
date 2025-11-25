// #include <QVBoxLayout>
// #include <QLabel>
// #include <QLineEdit>
// #include <QPushButton>
// #include <QRegularExpression>

#include "auth/registerwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

RegisterWindow::RegisterWindow(Database *db, QWidget *parent)
    : QWidget(parent), database(db)
{
    setupUI();
    setupStyles();
}

void RegisterWindow::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    titleLabel = new QLabel("Register", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Full Name
    fullNameEdit = new QLineEdit(this);
    fullNameEdit->setPlaceholderText("Full Name");
    fullNameEdit->setMinimumHeight(40);
    mainLayout->addWidget(fullNameEdit);

    // Email
    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("Email (@stu.bmcc.cuny.edu)");
    emailEdit->setMinimumHeight(40);
    mainLayout->addWidget(emailEdit);
    QLabel *emailHint = new QLabel("Use your BMCC email (@stu.bmcc.cuny.edu)", this);
    emailHint->setStyleSheet("color: #666666; font-size: 12px;");
    mainLayout->addWidget(emailHint);

    // EMPLID
    emplidEdit = new QLineEdit(this);
    emplidEdit->setPlaceholderText("EMPLID");
    emplidEdit->setMinimumHeight(40);
    mainLayout->addWidget(emplidEdit);
    QLabel *emplidHint = new QLabel("Your 8-digit BMCC Student ID", this);
    emplidHint->setStyleSheet("color: #666666; font-size: 12px;");
    mainLayout->addWidget(emplidHint);

    // Password
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(40);
    mainLayout->addWidget(passwordEdit);

    // Confirm Password
    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setPlaceholderText("Confirm Password");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setMinimumHeight(40);
    mainLayout->addWidget(confirmPasswordEdit);

    // Error label
    errorLabel = new QLabel(this);
    errorLabel->setStyleSheet("color: #D32F2F;");
    errorLabel->setWordWrap(true);
    errorLabel->hide();
    mainLayout->addWidget(errorLabel);

    // Register button
    registerButton = new QPushButton("Register", this);
    registerButton->setMinimumHeight(40);
    mainLayout->addWidget(registerButton);

    // Login link
    loginButton = new QPushButton("Already have an account? Login", this);
    mainLayout->addWidget(loginButton);

    // Connect signals
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::handleRegistration);
    connect(loginButton, &QPushButton::clicked, this, &RegisterWindow::onLoginClicked);
    connect(emailEdit, &QLineEdit::textChanged, this, &RegisterWindow::validateEmail);

    // Make window responsive
    setMinimumSize(450, 650);
    resize(500, 720);
}

void RegisterWindow::setupStyles()
{
    setStyleSheet(
        "QWidget { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #f5f7fa, stop:1 #c3cfe2);"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QLabel { color: #424242; font-size: 14px; }"
        "QLineEdit { "
        "   padding: 12px; "
        "   border: 2px solid #E0E0E0; "
        "   border-radius: 8px; "
        "   background: white; "
        "   color: #333333; "
        "   font-size: 14px;"
        "   min-height: 40px;"
        "}"
        "QLineEdit:focus { "
        "   border: 2px solid #2196F3; "
        "   background: #f8fcff;"
        "}"
        "QPushButton { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #0D47A1, stop:1 #1565C0);"
        "   color: white; "
        "   border: none; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   font-size: 14px;"
        "   min-height: 44px;"
        "   padding: 12px;"
        "}"
        "QPushButton:hover { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #1565C0, stop:1 #0D47A1);"
        "}");

    titleLabel->setStyleSheet(
        "font-size: 28px; "
        "font-weight: bold; "
        "color: #0D47A1; "
        "margin: 20px 0;"
        "background: transparent;");
    loginButton->setStyleSheet(
        "QPushButton { "
        "   background: transparent; "
        "   color: #2196F3; "
        "   font-weight: bold;"
        "   min-height: 40px;"
        "}"
        "QPushButton:hover { "
        "   color: #1565C0; "
        "   text-decoration: underline; "
        "}");
}

void RegisterWindow::handleRegistration()
{
    QString fullName = fullNameEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString emplid = emplidEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString confirmPass = confirmPasswordEdit->text();

    // Validation
    if (fullName.isEmpty() || email.isEmpty() || emplid.isEmpty() || password.isEmpty())
    {
        errorLabel->setText("All fields are required");
        errorLabel->show();
        return;
    }

    if (!isValidBMCCEmail(email))
    {
        errorLabel->setText("Please use your BMCC email (@stu.bmcc.cuny.edu)");
        errorLabel->show();
        return;
    }

    if (emplid.length() != 8 || !emplid.toInt())
    {
        errorLabel->setText("Please enter a valid 8-digit EMPLID");
        errorLabel->show();
        return;
    }

    if (password != confirmPass)
    {
        errorLabel->setText("Passwords do not match");
        errorLabel->show();
        return;
    }

    if (password.length() < 6)
    {
        errorLabel->setText("Password must be at least 6 characters");
        errorLabel->show();
        return;
    }

    // Try to register
    if (database->registerUser(email, password, email, fullName, emplid))
    {
        QMessageBox::information(this, "Success", "Registration successful!");
        emit registrationSuccessful();
        emit switchToLogin();
    }
    else
    {
        errorLabel->setText("Registration failed. Email might already exist.");
        errorLabel->show();
    }
}

void RegisterWindow::onLoginClicked()
{
    emit switchToLogin();
}

void RegisterWindow::validateEmail(const QString &email)
{
    if (!email.isEmpty() && !isValidBMCCEmail(email))
    {
        errorLabel->setText("Please use your BMCC email (@stu.bmcc.cuny.edu)");
        errorLabel->show();
    }
    else
    {
        errorLabel->hide();
    }
}

bool RegisterWindow::isValidBMCCEmail(const QString &email)
{
    return email.endsWith("@stu.bmcc.cuny.edu");
}
