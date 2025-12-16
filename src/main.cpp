#include <QApplication>
#include <QDebug>
#include <QSharedPointer>
#include "auth/loginwindow.h"
#include "auth/registerwindow.h"
#include "ui/mainwindow.h"
#include "ui/adminpage.h"
#include "database/database.h"

// ========== C++ CONCEPT #3: POINTERS ==========
// char *argv[] is an array of pointers to character strings (command line arguments)
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ========== SMART POINTERS (Advanced) ==========
    // QSharedPointer is a "smart pointer" that automatically manages memory
    // "new Database()" creates object and returns raw pointer
    // QSharedPointer wraps it and deletes it automatically when no longer needed
    QSharedPointer<Database> db(new Database());
    // -> operator accesses members through pointer (even smart pointers)
    if (!db->connectToDatabase())
    {
        qDebug() << "Failed to connect to database";
        return -1;
    }

    // Create windows using shared pointers
    // db.data() converts QSharedPointer to raw pointer (Database*)
    // This is needed because LoginWindow expects a raw pointer parameter
    QSharedPointer<LoginWindow> loginWindow(new LoginWindow(db.data()));
    QSharedPointer<RegisterWindow> registerWindow(new RegisterWindow(db.data()));
    QSharedPointer<MainWindow> mainWindow;
    QSharedPointer<AdminPage> adminPage;

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
                         loginWindow->hide(); // -> accesses method through pointer
                         // Create new MainWindow and store its pointer in mainWindow
                         mainWindow.reset(new MainWindow(db.data(), loginWindow->getCurrentUsername()));
                         mainWindow->show();
                     });

    // ========== C++ CONCEPT #2: INHERITANCE ==========
    // Handle admin login - demonstrates using Admin class (inherits from Person)
    QObject::connect(loginWindow.data(), &LoginWindow::adminLoginSuccessful,
                     [&adminPage, &loginWindow, db]()
                     {
                         loginWindow->hide();
                         // Get admin data from database (returns Admin object)
                         Admin adminData = db->getAdminData(loginWindow->getCurrentUsername());
                         // Create AdminPage with Admin object
                         adminPage.reset(new AdminPage(db.data(), adminData));

                         // Connect logout after creating adminPage
                         QObject::connect(adminPage.data(), &AdminPage::logoutRequested,
                                          [&adminPage, &loginWindow]()
                                          {
                                              if (adminPage)
                                              {
                                                  adminPage->hide();
                                                  adminPage.reset();
                                              }
                                              loginWindow->show();
                                          });

                         adminPage->show();
                     });

    loginWindow->show();

    return a.exec();
}
