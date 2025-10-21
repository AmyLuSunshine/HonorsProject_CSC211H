#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setupConnections();
    setupStyling();

    setWindowTitle("Campus Hire Portal - Login");
    setFixedSize(420, 350);

    // Set placeholder text
    ui->usernameLineEdit->setPlaceholderText("Username");
    ui->passwordLineEdit->setPlaceholderText("Password");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::setupConnections()
{
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &LoginWindow::onCancelClicked);
    connect(ui->createaccountButton, &QPushButton::clicked, this, &LoginWindow::onCreateAccountClicked);

    connect(ui->usernameLineEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Please enter both username and password.");
        return;
    }

    // Demo validation
    if (username == "admin" && password == "password") {
        accept(); // Close dialog with success
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid credentials.\nDemo: admin/password");
        ui->passwordLineEdit->clear();
        ui->passwordLineEdit->setFocus();
    }
}

void LoginWindow::onCancelClicked()
{
    reject(); // Close dialog with cancel
}

void LoginWindow::onCreateAccountClicked()
{
    QMessageBox::information(this, "Create Account",
                             "Registration feature coming soon!\nUse demo: admin/password");
}

QString LoginWindow::getUsername() const
{
    return ui->usernameLineEdit->text().trimmed();
}

QString LoginWindow::getPassword() const
{
    return ui->passwordLineEdit->text();
}

void LoginWindow::setupStyling()
{
    // Ad styling
}
