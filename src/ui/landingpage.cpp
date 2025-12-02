#include "ui/landingpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

LandingPage::LandingPage(Database *db, int userId, QWidget *parent)
    : QWidget(parent), database(db), currentUserId(userId)
{
    setupUI();
    setupStyles();
}

void LandingPage::setupUI()
{
    // Create a scroll area so users can scroll if needed
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Create the main content widget
    auto pageWidget = new QWidget();
    scrollArea->setWidget(pageWidget);

    // Main vertical layout - stacks everything from top to bottom
    auto mainLayout = new QVBoxLayout(pageWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // TITLE
    titleLabel = new QLabel("Welcome to Campus Hire", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #1976D2;");
    mainLayout->addWidget(titleLabel);

    // SUBTITLE
    subtitleLabel = new QLabel("Your Gateway to Campus Employment Success", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 16px; color: #666666;");
    mainLayout->addWidget(subtitleLabel);

    // DESCRIPTION
    descriptionLabel = new QLabel(
        "Campus Hire streamlines your on-campus employment journey — from discovery to hire.<br><br>"
        "We help you find the perfect position, prepare for success, and manage your application process.",
        this);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("font-size: 14px; color: #424242; padding: 0 40px;");
    mainLayout->addWidget(descriptionLabel);

    mainLayout->addSpacing(20);

    // FEATURE BOXES - side by side
    featuresLayout = new QHBoxLayout();
    featuresLayout->setSpacing(10);

    // Feature 1
    auto feature1Box = createFeatureBox(
        "💼",
        "On-Campus Jobs",
        "Browse positions tailored to your skills. Filter by department and eligibility.");
    featuresLayout->addWidget(feature1Box);

    // Feature 2
    auto feature2Box = createFeatureBox(
        "📋",
        "Smart On-boarding",
        "Upload documents once and auto-fill applications. Track paperwork seamlessly.");
    featuresLayout->addWidget(feature2Box);

    // Feature 3
    auto feature3Box = createFeatureBox(
        "🎯",
        "Interview Prep",
        "Get personalized tips and practice resources to land your dream campus job.");
    featuresLayout->addWidget(feature3Box);

    mainLayout->addLayout(featuresLayout);

    mainLayout->addSpacing(10);

    // BUTTONS
    auto buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    getStartedBtn = new QPushButton("Let's Get Started", this);
    getStartedBtn->setMinimumSize(180, 45);
    getStartedBtn->setCursor(Qt::PointingHandCursor);
    connect(getStartedBtn, &QPushButton::clicked, this, &LandingPage::startSurvey);

    skipBtn = new QPushButton("Skip for Now", this);
    skipBtn->setMinimumSize(150, 45);
    skipBtn->setCursor(Qt::PointingHandCursor);
    connect(skipBtn, &QPushButton::clicked, this, &LandingPage::skipToJobs);

    buttonLayout->addStretch();
    buttonLayout->addWidget(getStartedBtn);
    buttonLayout->addWidget(skipBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(20);

    // Put scroll area in the main widget
    auto outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);
}

// Simple function to create one feature box
// Takes: icon (emoji), title (bold text), description (normal text)
// Returns: a simple white box with the content
QWidget* LandingPage::createFeatureBox(const QString &icon,
                                       const QString &title,
                                       const QString &description)
{
    // Create the box container
    auto box = new QWidget(this);
    box->setFixedSize(300, 250);  // Fixed size 

    // Vertical layout - stacks icon, title, description from top to bottom
    auto layout = new QVBoxLayout(box);
    layout->setSpacing(30);
    layout->setContentsMargins(10, 10, 10, 10);

    // ICON
    auto iconLabel = new QLabel(icon, this);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 30px;");
    layout->addWidget(iconLabel);

    // TITLE
    auto titleLabel = new QLabel(title, this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setWordWrap(true);
    titleLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: #1976D2;");
    layout->addWidget(titleLabel);

    // DESCRIPTION
    auto descLabel = new QLabel(description, this);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("font-size: 15px; color: #424242;");
    layout->addWidget(descLabel);

    // Push everything to the top
    layout->addStretch();

    // Simple white box with border
    box->setStyleSheet(
        "QWidget { "
        "    background-color: white; "
        "    border-radius: 8px; "
        "}"
    );

    return box;
}

void LandingPage::setupStyles()
{
    setStyleSheet(
        // Page background
        "QWidget {"
        "    background-color: #F5F5F5;"
        "    font-family: 'Segoe UI', Arial, sans-serif;"
        "}"

        // Primary button (Let's Get Started)
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 12px 24px;"
        "}"

        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"

        "QPushButton:pressed {"
        "    background-color: #1565C0;"
        "}"

        // Skip button (outlined style)
        "QPushButton#skipBtn {"
        "    background-color: white;"
        "    color: #2196F3;"
        "    border: 2px solid #2196F3;"
        "}"

        "QPushButton#skipBtn:hover {"
        "    background-color: #E3F2FD;"
        "}"

        // Scroll area
        "QScrollArea {"
        "    border: none;"
        "    background-color: transparent;"
        "}"

        // Scrollbar
        "QScrollBar:vertical {"
        "    background: #E0E0E0;"
        "    width: 10px;"
        "    border-radius: 5px;"
        "}"

        "QScrollBar::handle:vertical {"
        "    background: #BDBDBD;"
        "    border-radius: 5px;"
        "}"

        "QScrollBar::handle:vertical:hover {"
        "    background: #9E9E9E;"
        "}"
    );

    skipBtn->setObjectName("skipBtn");
}