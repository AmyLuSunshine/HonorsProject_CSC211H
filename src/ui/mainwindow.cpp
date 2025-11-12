#include "ui/mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QApplication>
#include <QScreen>

MainWindow::MainWindow(Database *db, const QString &username, QWidget *parent)
    : QMainWindow(parent), database(db)
{
    setupUI();
    setupStyles();
    loadUserData(username);

    setWindowTitle("BMCC Campus Jobs Portal");
    
    // Make window responsive with proper sizing
    setMinimumSize(1024, 768);
    resize(1280, 800);
    
    // Center window on screen
    const QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void MainWindow::setupUI()
{
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
    auto applicationAction = toolbar->addAction("My Application");
    auto interviewAction = toolbar->addAction("Interview");
    toolbar->addSeparator();
    auto logoutAction = toolbar->addAction("Logout");

    // Create stacked widget
    stackedWidget = new QStackedWidget(centralWidget);

    // Create pages
    jobListWidget = new JobListWidget(database, this);
    profilePage = new MyProfilePage(database, this);
    applicationPage = new MyApplicationPage(database, this);
    interviewWidget = new InterviewWidget(currentUser.getMajor(), this);

    stackedWidget->addWidget(jobListWidget);
    stackedWidget->addWidget(profilePage);
    stackedWidget->addWidget(applicationPage);
    stackedWidget->addWidget(interviewWidget);

    // Set layout
    auto layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(stackedWidget);

    // Connect signals
    connect(jobsAction, &QAction::triggered, this, &MainWindow::switchToJobs);
    connect(profileAction, &QAction::triggered, this, &MainWindow::switchToProfile);
    connect(applicationAction, &QAction::triggered, this, &MainWindow::switchToApplication);
    connect(interviewAction, &QAction::triggered, this, &MainWindow::switchToInterview);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::handleLogout);

    // Start with jobs page
    switchToJobs();
}

void MainWindow::setupStyles()
{
    // Modern gradient background and styling
    setStyleSheet(
        "QMainWindow {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #f5f7fa, stop:1 #e8eef5);"
        "}"
        "QWidget {"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QToolBar { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #2196F3, stop:1 #1976D2);"
        "   spacing: 10px; "
        "   padding: 8px; "
        "   border-bottom: 2px solid #1565C0;"
        "}"
        "QToolBar QToolButton { "
        "   color: white; "
        "   background-color: transparent; "
        "   padding: 8px 20px; "
        "   border: none; "
        "   border-radius: 4px;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "}"
        "QToolBar QToolButton:hover { "
        "   background-color: rgba(255, 255, 255, 0.2); "
        "}"
        "QToolBar QToolButton:pressed { "
        "   background-color: rgba(255, 255, 255, 0.3); "
        "}"
    );
}

void MainWindow::applyFadeTransition(QWidget *widget)
{
    // Smooth fade-in effect
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::switchToJobs()
{
    applyFadeTransition(jobListWidget);
    stackedWidget->setCurrentWidget(jobListWidget);
}

void MainWindow::switchToProfile()
{
    applyFadeTransition(profilePage);
    stackedWidget->setCurrentWidget(profilePage);
}

void MainWindow::switchToApplication()
{
    applyFadeTransition(applicationPage);
    stackedWidget->setCurrentWidget(applicationPage);
}

void MainWindow::switchToInterview()
{
    applyFadeTransition(interviewWidget);
    stackedWidget->setCurrentWidget(interviewWidget);
}

void MainWindow::handleLogout()
{
    if (QMessageBox::question(this, "Logout", "Are you sure you want to logout?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        close();
    }
}

void MainWindow::loadUserData(const QString &email)
{
    currentUser = database->getUserData(email);
    int uid = currentUser.getId();
    profilePage->setUserId(uid);
    applicationPage->setUserId(uid);
    // Optionally refresh interviewWidget if needed
}
