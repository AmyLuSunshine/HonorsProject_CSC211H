#include "ui/adminpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

// ========== C++ CONCEPT #3: POINTERS ==========
// Constructor with pointer parameter
AdminPage::AdminPage(Database *db, const Admin &adminData, QWidget *parent)
    : QWidget(parent), database(db), currentAdmin(adminData)
{
    setupUI();
    loadUserData();
}

void AdminPage::setupUI()
{
    // Create main layout using pointer
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // ========== C++ CONCEPT #2: INHERITANCE ==========
    // currentAdmin.getRole() uses virtual function override
    // Returns "Admin" because Admin class overrides Person::getRole()
    welcomeLabel = new QLabel(QString("Welcome, %1 (%2)")
                                  .arg(currentAdmin.getFullName())
                                  .arg(currentAdmin.getRole()),
                              this);
    welcomeLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #1976D2;");
    mainLayout->addWidget(welcomeLabel);

    // Display admin department (admin-specific field, not inherited)
    auto deptLabel = new QLabel(QString("Department: %1 | Access Level: %2")
                                    .arg(currentAdmin.getDepartment())
                                    .arg(currentAdmin.getAccessLevel()),
                                this);
    deptLabel->setStyleSheet("font-size: 14px; color: #666;");
    mainLayout->addWidget(deptLabel);

    // Stats label
    statsLabel = new QLabel(this);
    statsLabel->setStyleSheet("font-size: 16px; color: #333; padding: 10px;");
    mainLayout->addWidget(statsLabel);

    // Button layout
    auto buttonLayout = new QHBoxLayout();

    refreshButton = new QPushButton("Refresh User List", this);
    refreshButton->setMinimumHeight(40);
    refreshButton->setStyleSheet(
        "QPushButton { background-color: #1976D2; color: white; border-radius: 5px; font-size: 14px; }"
        "QPushButton:hover { background-color: #1565C0; }");
    connect(refreshButton, &QPushButton::clicked, this, &AdminPage::refreshUserList);
    buttonLayout->addWidget(refreshButton);

    logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumHeight(40);
    logoutButton->setStyleSheet(
        "QPushButton { background-color: #D32F2F; color: white; border-radius: 5px; font-size: 14px; }"
        "QPushButton:hover { background-color: #C62828; }");
    connect(logoutButton, &QPushButton::clicked, this, &AdminPage::handleLogout);
    buttonLayout->addWidget(logoutButton);

    mainLayout->addLayout(buttonLayout);

    // User table - pointer to table widget
    userTable = new QTableWidget(this);
    userTable->setColumnCount(6);
    userTable->setHorizontalHeaderLabels({"ID", "Name", "Email", "EMPLID", "Major", "Status"});
    userTable->horizontalHeader()->setStretchLastSection(true);
    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTable->setAlternatingRowColors(true);
    mainLayout->addWidget(userTable);

    setMinimumSize(900, 600);
}

void AdminPage::loadUserData()
{
    // ========== POINTER USAGE: -> operator ==========
    // database is a pointer, so we use -> to access its methods
    std::vector<User> users = database->getAllUsers();
    int totalUsers = database->getTotalUserCount();

    // Update stats using arrow operator on pointer
    statsLabel->setText(QString("Total Users: %1").arg(totalUsers));

    // Clear and populate table
    userTable->setRowCount(0);

    for (size_t i = 0; i < users.size(); ++i)
    {
        // ========== C++ CONCEPT #2: INHERITANCE IN ACTION ==========
        // users[i] is a User object that inherits from Person
        // We can call getEmail(), getFullName() (inherited from Person)
        // And getEmplid(), getMajor() (specific to User class)

        const User &user = users[i];

        userTable->insertRow(i);
        userTable->setItem(i, 0, new QTableWidgetItem(QString::number(user.getId())));
        userTable->setItem(i, 1, new QTableWidgetItem(user.getFullName()));
        userTable->setItem(i, 2, new QTableWidgetItem(user.getEmail()));
        userTable->setItem(i, 3, new QTableWidgetItem(user.getEmplid()));
        userTable->setItem(i, 4, new QTableWidgetItem(user.getMajor()));

        QString status = user.getSurveyCompleted() ? "Survey Done" : "Pending Survey";
        userTable->setItem(i, 5, new QTableWidgetItem(status));
    }
}

void AdminPage::refreshUserList()
{
    loadUserData();
    QMessageBox::information(this, "Refreshed", "User list has been refreshed.");
}

void AdminPage::handleLogout()
{
    emit logoutRequested();
}
