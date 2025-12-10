#include <QApplication>
#include <QDebug>
#include <QSharedPointer>
#include "auth/loginwindow.h"
#include "auth/registerwindow.h"
#include "ui/mainwindow.h"
#include "database/database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize database with shared pointer for better memory management
    QSharedPointer<Database> db(new Database());
    if (!db->connectToDatabase())
    {
        qDebug() << "Failed to connect to database";
        return -1;
    }

    // Create windows using shared pointers
    QSharedPointer<LoginWindow> loginWindow(new LoginWindow(db.data()));
    QSharedPointer<RegisterWindow> registerWindow(new RegisterWindow(db.data()));
    QSharedPointer<MainWindow> mainWindow;

    // Connect login/register windows
    QObject::connect(loginWindow.data(), &LoginWindow::switchToRegister, [=]()
                     {
        loginWindow->hide();
        registerWindow->show(); });

    QObject::connect(registerWindow.data(), &RegisterWindow::switchToLogin, [=]()
                     {
        registerWindow->hide();
        loginWindow->show(); });

    QObject::connect(loginWindow.data(), &LoginWindow::loginSuccessful,
                     [&mainWindow, db, loginWindow]()
                     {
                         loginWindow->hide();
                         mainWindow.reset(new MainWindow(db.data(), loginWindow->getCurrentUsername()));
                         mainWindow->show();
                     });

    loginWindow->show();

    return a.exec();
}
