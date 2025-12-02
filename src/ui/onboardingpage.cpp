#include "onboardingpage.h"
#include <QMessageBox>

// Constructor
OnboardingPage::OnboardingPage(bool isInternational, QWidget *parent)
    : QWidget(parent), isInternational(isInternational)
{
    setupUI();
    loadContent();
}

// Create all the widgets
void OnboardingPage::setupUI()
{
    // Main layout - everything stacks vertically
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    titleLabel = new QLabel("Student Onboarding", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Student type label
    studentTypeLabel = new QLabel(this);
    studentTypeLabel->setStyleSheet("font-size: 16px;");
    studentTypeLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(studentTypeLabel);

    // Buttons at top
    QHBoxLayout *topButtons = new QHBoxLayout();

    backButton = new QPushButton("Back", this);
    backButton->setMinimumHeight(40);
    connect(backButton, &QPushButton::clicked, this, &OnboardingPage::onBackClicked);
    topButtons->addWidget(backButton);

    QPushButton *viewAppBtn = new QPushButton("View My Application", this);
    viewAppBtn->setMinimumHeight(40);
    connect(viewAppBtn, &QPushButton::clicked, this, [this]()
            { emit viewApplicationClicked(); });
    topButtons->addWidget(viewAppBtn);

    mainLayout->addLayout(topButtons);

    mainLayout->addSpacing(10);

    // Documents section
    QLabel *docTitle = new QLabel("Required Documents:", this);
    docTitle->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(docTitle);

    documentList = new QListWidget(this);
    documentList->setMinimumHeight(150);
    mainLayout->addWidget(documentList);

    mainLayout->addSpacing(10);

    // Steps section
    QLabel *stepsTitle = new QLabel("Onboarding Steps:", this);
    stepsTitle->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(stepsTitle);

    stepsList = new QListWidget(this);
    stepsList->setMinimumHeight(200);
    mainLayout->addWidget(stepsList);

    mainLayout->addStretch();
}

// Load content based on student type
void OnboardingPage::loadContent()
{
    documentList->clear();
    stepsList->clear();

    if (isInternational)
    {
        // International student
        studentTypeLabel->setText("International Student (F-1 Visa)");

        // Documents
        documentList->addItem("1. Valid I-20 Form");
        documentList->addItem("2. Passport (valid 6+ months)");
        documentList->addItem("3. F-1 Visa");
        documentList->addItem("4. I-94 Record");
        documentList->addItem("5. Work Authorization Letter");
        documentList->addItem("6. Social Security Card");

        // Steps
        stepsList->addItem("Step 1: Verify F-1 status with International Office");
        stepsList->addItem("Step 2: Get work authorization letter");
        stepsList->addItem("Step 3: Apply for Social Security Number (if needed)");
        stepsList->addItem("Step 4: Submit I-9 documents to HR (Room S-701)");
        stepsList->addItem("Step 5: Complete W-4 and Direct Deposit forms");
        stepsList->addItem("Step 6: Start working (max 20 hours/week)");
    }
    else
    {
        // Work-study student
        studentTypeLabel->setText("Work-Study Student");

        // Documents
        documentList->addItem("1. I-9 Documentation (bring to HR S-701)");
        documentList->addItem("2. Social Security Card (bring to Payroll S-702)");
        documentList->addItem("3. W-4 Tax Form (submit online)");
        documentList->addItem("4. Direct Deposit Form (submit online)");
        documentList->addItem("5. Transcript (upload online)");
        documentList->addItem("6. Resume (upload online)");

        // Steps
        stepsList->addItem("Step 1: Apply to Work-Study Program");
        stepsList->addItem("Step 2: Apply for campus jobs");
        stepsList->addItem("Step 3: Submit documents to HR and Payroll");
        stepsList->addItem("Step 4: Complete online forms (W-4, Direct Deposit)");
        stepsList->addItem("Step 5: Attend training session");
        stepsList->addItem("Step 6: Start working (up to 20 hours/week)");
    }
}

// Back button clicked
void OnboardingPage::onBackClicked()
{
    emit backButtonClicked();
}

// View guide button clicked (placeholder for future)
void OnboardingPage::onViewGuideClicked()
{
    // This will be connected to a button in the future
    // For now, it's just a placeholder
}
