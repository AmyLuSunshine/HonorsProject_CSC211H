#include "ui/mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QApplication>
#include <QScreen>
#include <QToolButton>
#include <QMenu>

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

    // Add toolbar actions (removed My Profile from main toolbar)
    auto onCampusJobsAction = toolbar->addAction("On-Campus Jobs");
    auto applicationAction = toolbar->addAction("My Application");
    auto interviewAction = toolbar->addAction("Interview");

    // Add an expanding spacer to push Settings to the right
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    // Settings dropdown with icons
    auto settingsButton = new QToolButton(this);
    settingsButton->setText("⚙ Settings");
    settingsButton->setPopupMode(QToolButton::InstantPopup);
    auto settingsMenu = new QMenu(settingsButton);

    QAction *settingsProfileAction = settingsMenu->addAction("👤 My Profile");
    QAction *settingsLogoutAction = settingsMenu->addAction("⏻ Logout");

    settingsButton->setMenu(settingsMenu);
    toolbar->addWidget(settingsButton);

    // Create stacked widget
    stackedWidget = new QStackedWidget(centralWidget);

    // Create pages
    profilePage = new MyProfilePage(database, this);
    applicationPage = new MyApplicationPage(database, this);
    onCampusJobsPage = new OnCampusJobsPage(database, -1, this); // userId set later
    interviewWidget = new InterviewWidget(currentUser.getMajor(), this);

    stackedWidget->addWidget(onCampusJobsPage);
    stackedWidget->addWidget(profilePage);
    stackedWidget->addWidget(applicationPage);
    stackedWidget->addWidget(interviewWidget);

    // Set layout
    auto layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(stackedWidget);

    // Connect signals
    connect(onCampusJobsAction, &QAction::triggered, this, &MainWindow::switchToOnCampusJobs);
    connect(applicationAction, &QAction::triggered, this, &MainWindow::switchToApplication);
    connect(interviewAction, &QAction::triggered, this, &MainWindow::switchToInterview);
    connect(settingsProfileAction, &QAction::triggered, this, &MainWindow::switchToProfile);
    connect(settingsLogoutAction, &QAction::triggered, this, &MainWindow::handleLogout);

    // Start with On-Campus Jobs page
    switchToOnCampusJobs();
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
        "   spacing: 12px; "
        "   padding: 10px 16px; "
        "   border-bottom: 2px solid #1565C0;"
        "}"
        "QToolBar QToolButton { "
        "   color: white; "
        "   background-color: transparent; "
        "   padding: 8px 18px; "
        "   border: none; "
        "   border-radius: 6px;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "}"
        "QToolBar QToolButton:hover { "
        "   background-color: rgba(255, 255, 255, 0.2); "
        "}"
        "QToolBar QToolButton:pressed { "
        "   background-color: rgba(255, 255, 255, 0.3); "
        "}"
        "QToolButton::menu-indicator { width: 10px; height: 10px; }"
        "QMenu {"
        "   background: white;"
        "   border: 1px solid #BDBDBD;"
        "   border-radius: 6px;"
        "   padding: 4px 0;"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QMenu::item {"
        "   padding: 10px 30px 10px 20px;"
        "   font-size: 14px;"
        "   color: #424242;"
        "}"
        "QMenu::item:selected {"
        "   background: #E3F2FD;"
        "   color: #1976D2;"
        "}");
}

void MainWindow::applyFadeTransition(QWidget *widget)
{
    // Smooth fade-in effect
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::switchToOnCampusJobs()
{
    applyFadeTransition(onCampusJobsPage);
    stackedWidget->setCurrentWidget(onCampusJobsPage);
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
    // Modern styled confirmation dialog
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Logout Confirmation");
    msgBox.setText("Are you sure you want to logout?");
    msgBox.setInformativeText("You will need to login again to access the portal.");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    // Style the message box
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background: white;"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QLabel { font-size: 14px; color: #424242; }"
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2196F3, stop:1 #1976D2);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 8px 20px;"
        "   font-size: 14px;"
        "   font-weight: 600;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1976D2, stop:1 #0D47A1);"
        "}");

    if (msgBox.exec() == QMessageBox::Yes)
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

    // Update onCampusJobsPage with new userId
    delete onCampusJobsPage;
    onCampusJobsPage = new OnCampusJobsPage(database, uid, this);
    stackedWidget->insertWidget(1, onCampusJobsPage);

    // Optionally refresh interviewWidget if needed
}
