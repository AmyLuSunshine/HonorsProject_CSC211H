#include "ui/mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(Database* db, const QString& username, QWidget *parent)
    : QMainWindow(parent), database(db) {
    setupUI();
    setupStyles();
    loadUserData(username);

    setWindowTitle("BMCC Campus Jobs Portal");
    setMinimumSize(800, 600);
}

void MainWindow::setupUI() {
    // Create central widget
    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create toolbar
    toolbar = new QToolBar(this);
    toolbar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, toolbar);

    // Add toolbar actions
    auto jobsAction = toolbar->addAction("Available Jobs");
    auto profileAction = toolbar->addAction("My Profile");
    auto interviewAction = toolbar->addAction("Interview");
    toolbar->addSeparator();
    auto logoutAction = toolbar->addAction("Logout");

    // Create stacked widget
    stackedWidget = new QStackedWidget(centralWidget);

    // Create pages
    jobListWidget = new JobListWidget(database, this);
    profileWidget = new ProfileWidget(database, this);
    interviewWidget = new InterviewWidget(database, &currentUser, this);

    stackedWidget->addWidget(jobListWidget);
    stackedWidget->addWidget(profileWidget);
    stackedWidget->addWidget(interviewWidget);

    // Set layout
    auto layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(stackedWidget);

    // Connect signals
    connect(jobsAction, &QAction::triggered, this, &MainWindow::switchToJobs);
    connect(profileAction, &QAction::triggered, this, &MainWindow::switchToProfile);
    connect(interviewAction, &QAction::triggered, this, &MainWindow::switchToInterview);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::handleLogout);

    // Start with jobs page
    switchToJobs();
}

void MainWindow::setupStyles() {
    setStyleSheet(
        "QMainWindow { background-color: #F5F5F5; }"
        "QToolBar { background-color: #2196F3; spacing: 10px; padding: 5px; }"
        "QToolBar QToolButton { color: white; background-color: transparent; "
        "                       padding: 5px 15px; border: none; }"
        "QToolBar QToolButton:hover { background-color: #1976D2; }"
        );
}

void MainWindow::switchToJobs() {
    stackedWidget->setCurrentWidget(jobListWidget);
}

void MainWindow::switchToProfile() {
    stackedWidget->setCurrentWidget(profileWidget);
}

void MainWindow::switchToInterview() {
    stackedWidget->setCurrentWidget(interviewWidget);
}

void MainWindow::handleLogout() {
    if (QMessageBox::question(this, "Logout", "Are you sure you want to logout?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::loadUserData(const QString& email) {
    currentUser = database->getUserData(email);
    profileWidget->loadUserProfile(email);
    // Optionally refresh interviewWidget if needed
}
