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
    mainLayout->setSpacing(30);                     // Consistent spacing like interview page
    mainLayout->setContentsMargins(40, 40, 40, 40); // Balanced margins like interview page

    // TITLE
    titleLabel = new QLabel("Campus Hire", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #000000;");
    mainLayout->addWidget(titleLabel);

    // SUBTITLE
    subtitleLabel = new QLabel("Streamline on-campus employment journey — from discovery to on-board", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #666666;");
    mainLayout->addWidget(subtitleLabel);

    // Gap before feature boxes
    mainLayout->addSpacing(30);

    // FEATURE BOXES - side by side (clickable)
    featuresLayout = new QHBoxLayout();
    featuresLayout->setSpacing(20); // Consistent with interview page spacing
    featuresLayout->setContentsMargins(0, 0, 0, 0);

    // Feature 1 - Jobs Box (clickable)
    auto jobsBtn = createFeatureBox(
        "💼 On-Campus Jobs",
        "Find the perfect position.\nFilter by eligibility.",
        "#ffffff");
    connect(jobsBtn, &QPushButton::clicked, this, &LandingPage::jobsBoxClicked);
    featuresLayout->addWidget(jobsBtn);

    // Feature 2 - Onboarding Box (clickable)
    auto onboardingBtn = createFeatureBox(
        "📋 Smart On-boarding",
        "Manage application process.\nTrack paperwork seamlessly.",
        "#ffffff");
    connect(onboardingBtn, &QPushButton::clicked, this, &LandingPage::onboardingBoxClicked);
    featuresLayout->addWidget(onboardingBtn);

    // Feature 3 - Interview Box (clickable)
    auto interviewBtn = createFeatureBox(
        "🎯 Interview Prep",
        "Prepare for success\nto land your dream campus job.",
        "#ffffff");
    connect(interviewBtn, &QPushButton::clicked, this, &LandingPage::interviewBoxClicked);
    featuresLayout->addWidget(interviewBtn);

    mainLayout->addLayout(featuresLayout);

    // BUTTONS (using same spacing pattern)
    auto buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    getStartedBtn = new QPushButton("Complete Profile", this);
    getStartedBtn->setMinimumSize(150, 40);
    getStartedBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #ffffff;"
        "  color: #000000;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  border: 2px solid #cccccc;"
        "  border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f0f0f0;"
        "  border: 2px solid #999999;"
        "}");
    getStartedBtn->setCursor(Qt::PointingHandCursor);
    connect(getStartedBtn, &QPushButton::clicked, this, &LandingPage::startSurvey);

    skipBtn = new QPushButton("Browse Jobs", this);
    skipBtn->setMinimumSize(150, 40);
    skipBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #f5f5f5;"
        "  color: #000000;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  border: 2px solid #cccccc;"
        "  border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #eeeeee;"
        "  border: 2px solid #999999;"
        "}");
    skipBtn->setCursor(Qt::PointingHandCursor);
    connect(skipBtn, &QPushButton::clicked, this, &LandingPage::skipToJobs);

    buttonLayout->addStretch();
    buttonLayout->addWidget(getStartedBtn);
    buttonLayout->addWidget(skipBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Add stretch at the bottom to push everything up
    mainLayout->addStretch();

    // Put scroll area in the main widget
    auto outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);
}

// Helper function to create feature boxes - keeps code clean and simple
QPushButton *LandingPage::createFeatureBox(const QString &title, const QString &description, const QString &bgColor)
{
    // Create the content widget that goes inside the button
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(15, 20, 15, 20); // Padding inside the box
    contentLayout->setSpacing(8);                      // Space between title and description
    contentLayout->setAlignment(Qt::AlignCenter);

    // Title label
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #000000;");
    contentLayout->addWidget(titleLabel);

    // Description label
    QLabel *descLabel = new QLabel(description);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setStyleSheet("font-size: 11px; color: #666666; line-height: 1.5;");
    contentLayout->addWidget(descLabel);

    // Create the clickable button
    auto button = new QPushButton();
    button->setLayout(contentLayout);
    button->setMinimumSize(250, 130); // Slightly smaller for better proportions
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Determine hover color based on background
    QString hoverColor = (bgColor == "#ffffff") ? "#f5f5f5" : "#eeeeee";

    button->setStyleSheet(
        QString("QPushButton {"
                "  background-color: %1;"
                "  border: 2px solid #cccccc;"
                "  border-radius: 8px;"
                "  padding: 0px;"
                "}"
                "QPushButton:hover {"
                "  background-color: %2;"
                "  border: 2px solid #999999;"
                "}")
            .arg(bgColor, hoverColor));
    button->setCursor(Qt::PointingHandCursor);

    return button;
}

void LandingPage::setupStyles()
{
    setStyleSheet(
        // Page background
        "QWidget {"
        "    background-color: #ffffff;"
        "    font-family: 'Segoe UI', Arial, sans-serif;"
        "}"

        // Scroll area
        "QScrollArea {"
        "    border: none;"
        "    background-color: #ffffff;"
        "}"

        // Scrollbar
        "QScrollBar:vertical {"
        "    background: #f5f5f5;"
        "    width: 10px;"
        "}"

        "QScrollBar::handle:vertical {"
        "    background: #cccccc;"
        "    border-radius: 5px;"
        "}"

        "QScrollBar::handle:vertical:hover {"
        "    background: #999999;"
        "}");
}