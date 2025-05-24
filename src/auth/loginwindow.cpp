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


    setFixedSize(400, 500);
}

void LoginWindow::setupStyles() {
    setStyleSheet(
        "QWidget { background-color: #FFFFFF; }"
        "QLabel { font-size: 16px; }"
        "QLineEdit { padding: 10px; border: 2px solid #E0E0E0; border-radius: 5px; margin: 5px 0; font-size: 14px; }"
        "QPushButton { padding: 10px; background-color: #2196F3; color: white; border: none; border-radius: 5px; font-size: 14px; }"
        "QPushButton:hover { background-color: #1976D2; }"
    );

    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2196F3; margin: 20px 0;");
    registerButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; }"
        "QPushButton:hover { background-color: #388E3C; }"
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
