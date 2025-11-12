#include "auth/loginwindow.h"
#include "database/database.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

LoginWindow::LoginWindow(Database* db, QWidget *parent)
    : QWidget(parent), database(db) {
    qDebug() << "Creating login window...";
    setupUI();
    setupStyles();
    qDebug() << "Login window created and styled";
    
    // Make sure window appears in the center of the screen
    this->setWindowTitle("Campus Jobs Portal");
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            QSize(400, 500),
            QApplication::primaryScreen()->availableGeometry()
        )
    );
    qDebug() << "Window geometry set";
}

void LoginWindow::setupUI() {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Title
    titleLabel = new QLabel("Campus Jobs Portal", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Username
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");
    mainLayout->addWidget(usernameEdit);

    // Password
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    mainLayout->addWidget(passwordEdit);

    // Error label
    errorLabel = new QLabel(this);
    errorLabel->setStyleSheet("color: red;");
    errorLabel->hide();
    mainLayout->addWidget(errorLabel);

    // Login button
    loginButton = new QPushButton("Login", this);
    mainLayout->addWidget(loginButton);

    // Register button
    registerButton = new QPushButton("Create Account", this);
    mainLayout->addWidget(registerButton);

    // Connect signals
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);

    // Add the stuff needed for new version
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

    // Add input validators
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    usernameEdit->setValidator(new QRegularExpressionValidator(emailRegex, this));

    // Add return key handling
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::handleLogin);

    // Make window responsive
    setMinimumSize(400, 500);
    resize(450, 550);
}

void LoginWindow::setupStyles() {
    setStyleSheet(
        "QWidget { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #f5f7fa, stop:1 #c3cfe2);"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QLabel { font-size: 16px; color: #424242; }"
        "QLineEdit { "
        "   padding: 12px; "
        "   border: 2px solid #E0E0E0; "
        "   border-radius: 8px; "
        "   margin: 5px 0; "
        "   font-size: 14px; "
        "   background: white;"
        "   min-height: 40px;"
        "}"
        "QLineEdit:focus { "
        "   border: 2px solid #2196F3; "
        "   background: #f8fcff;"
        "}"
        "QPushButton { "
        "   padding: 12px; "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #2196F3, stop:1 #1976D2);"
        "   color: white; "
        "   border: none; "
        "   border-radius: 8px; "
        "   font-size: 14px; "
        "   font-weight: bold;"
        "   min-height: 44px;"
        "}"
        "QPushButton:hover { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #1976D2, stop:1 #0D47A1);"
        "}"
    );

    titleLabel->setStyleSheet(
        "font-size: 28px; "
        "font-weight: bold; "
        "color: #1976D2; "
        "margin: 20px 0;"
        "background: transparent;"
    );
    registerButton->setStyleSheet(
        "QPushButton { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #4CAF50, stop:1 #388E3C);"
        "   min-height: 44px;"
        "}"
        "QPushButton:hover { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #388E3C, stop:1 #2E7D32);"
        "}"
    );
}

void LoginWindow::handleLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Please fill in all fields");
        errorLabel->show();
        return;
    }

    if (database->validateLogin(username, password)) {
        emit loginSuccessful();
    } else {
        errorLabel->setText("Invalid username or password");
        errorLabel->show();
    }
}

void LoginWindow::onRegisterClicked() {
    emit switchToRegister();
}
