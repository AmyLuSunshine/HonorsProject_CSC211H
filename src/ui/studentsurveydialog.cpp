#include "ui/studentsurveydialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QScrollArea>

StudentSurveyDialog::StudentSurveyDialog(Database *db, int userId, QWidget *parent)
    : QDialog(parent), database(db), currentUserId(userId)
{
    setupUI();
    setupStyles();
    setWindowTitle("Student Profile Survey");
    setMinimumSize(750, 700);
    resize(800, 750);
}

void StudentSurveyDialog::setupUI()
{
    // Main dialog layout
    auto dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->setSpacing(0);

    // Create scroll area
    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Content widget inside scroll area
    auto contentWidget = new QWidget();
    auto mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Title
    titleLabel = new QLabel("Complete Your Profile", this);
    titleLabel->setStyleSheet("font-size: 26px; font-weight: 600; color: #1976D2;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Description
    descriptionLabel = new QLabel(
        "Help us personalize your experience by providing a few details. "
        "Upload your resume and transcript to auto-fill future applications and verify eligibility.",
        this);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("font-size: 14px; color: #616161; line-height: 1.5;");
    descriptionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(descriptionLabel);

    mainLayout->addSpacing(20);

    // International student section
    auto internationalGroup = new QGroupBox("Student Status", this);
    auto internationalLayout = new QVBoxLayout(internationalGroup);

    internationalCheckbox = new QCheckBox("I am an international student", this);
    internationalCheckbox->setStyleSheet("font-size: 14px; font-weight: 500;");
    internationalLayout->addWidget(internationalCheckbox);

    internationalHelpLabel = new QLabel(
        "💡 This helps us show you jobs that accept international students and guide you "
        "through visa/work authorization requirements.",
        this);
    internationalHelpLabel->setWordWrap(true);
    internationalHelpLabel->setStyleSheet("font-size: 12px; color: #757575; margin-left: 25px;");
    internationalLayout->addWidget(internationalHelpLabel);

    mainLayout->addWidget(internationalGroup);

    // Resume upload section
    auto resumeGroup = new QGroupBox("Resume Upload", this);
    auto resumeLayout = new QVBoxLayout(resumeGroup);

    auto resumeRowLayout = new QHBoxLayout();
    resumePathEdit = new QLineEdit(this);
    resumePathEdit->setPlaceholderText("No file selected");
    resumePathEdit->setReadOnly(true);
    resumeBrowseBtn = new QPushButton("Browse...", this);
    connect(resumeBrowseBtn, &QPushButton::clicked, this, &StudentSurveyDialog::browseResume);

    resumeRowLayout->addWidget(new QLabel("Resume (PDF):", this));
    resumeRowLayout->addWidget(resumePathEdit, 1);
    resumeRowLayout->addWidget(resumeBrowseBtn);
    resumeLayout->addLayout(resumeRowLayout);

    resumeStatusLabel = new QLabel("", this);
    resumeStatusLabel->setStyleSheet("font-size: 12px; color: #4CAF50;");
    resumeLayout->addWidget(resumeStatusLabel);

    auto resumeHelpLabel = new QLabel(
        "📄 We'll extract your contact info and experience to pre-fill applications.",
        this);
    resumeHelpLabel->setStyleSheet("font-size: 12px; color: #757575;");
    resumeLayout->addWidget(resumeHelpLabel);

    mainLayout->addWidget(resumeGroup);

    // Transcript upload section
    auto transcriptGroup = new QGroupBox("Transcript Upload", this);
    auto transcriptLayout = new QVBoxLayout(transcriptGroup);

    auto transcriptRowLayout = new QHBoxLayout();
    transcriptPathEdit = new QLineEdit(this);
    transcriptPathEdit->setPlaceholderText("No file selected");
    transcriptPathEdit->setReadOnly(true);
    transcriptBrowseBtn = new QPushButton("Browse...", this);
    connect(transcriptBrowseBtn, &QPushButton::clicked, this, &StudentSurveyDialog::browseTranscript);

    transcriptRowLayout->addWidget(new QLabel("Transcript (PDF):", this));
    transcriptRowLayout->addWidget(transcriptPathEdit, 1);
    transcriptRowLayout->addWidget(transcriptBrowseBtn);
    transcriptLayout->addLayout(transcriptRowLayout);

    transcriptStatusLabel = new QLabel("", this);
    transcriptStatusLabel->setStyleSheet("font-size: 12px; color: #4CAF50;");
    transcriptLayout->addWidget(transcriptStatusLabel);

    auto transcriptHelpLabel = new QLabel(
        "📊 We'll automatically verify your GPA and coursework for job eligibility.",
        this);
    transcriptHelpLabel->setStyleSheet("font-size: 12px; color: #757575;");
    transcriptLayout->addWidget(transcriptHelpLabel);

    mainLayout->addWidget(transcriptGroup);

    // Parsed data preview
    auto previewGroup = new QGroupBox("Parsed Information", this);
    auto previewLayout = new QVBoxLayout(previewGroup);

    parsedDataPreview = new QTextEdit(this);
    parsedDataPreview->setReadOnly(true);
    parsedDataPreview->setPlaceholderText("Upload documents to see parsed data preview...");
    parsedDataPreview->setMaximumHeight(150);
    previewLayout->addWidget(parsedDataPreview);

    mainLayout->addWidget(previewGroup);

    // Progress bar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);

    mainLayout->addSpacing(20);

    // Buttons
    auto buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 20, 0, 0);

    skipBtn = new QPushButton("Skip for Now", this);
    skipBtn->setMinimumSize(140, 45);
    connect(skipBtn, &QPushButton::clicked, this, &StudentSurveyDialog::skipSurvey);

    submitBtn = new QPushButton("Complete Profile", this);
    submitBtn->setMinimumSize(170, 45);
    connect(submitBtn, &QPushButton::clicked, this, &StudentSurveyDialog::submitSurvey);

    buttonLayout->addWidget(skipBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(submitBtn);

    mainLayout->addLayout(buttonLayout);

    // Set the content widget to scroll area
    scrollArea->setWidget(contentWidget);

    // Add scroll area to dialog
    dialogLayout->addWidget(scrollArea);
}

void StudentSurveyDialog::setupStyles()
{
    setStyleSheet(
        "QDialog {"
        "    background: #FAFAFA;"
        "    font-family: 'Segoe UI', Arial, sans-serif;"
        "}"
        "QGroupBox {"
        "    font-size: 15px;"
        "    font-weight: 600;"
        "    color: #424242;"
        "    border: 2px solid #E0E0E0;"
        "    border-radius: 8px;"
        "    margin-top: 10px;"
        "    padding-top: 15px;"
        "    background: white;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px;"
        "}"
        "QLineEdit {"
        "    padding: 8px;"
        "    border: 2px solid #E0E0E0;"
        "    border-radius: 6px;"
        "    background: white;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #2196F3;"
        "}"
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "        stop:0 #2196F3, stop:1 #1976D2);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-size: 14px;"
        "    font-weight: 600;"
        "    padding: 10px 20px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "        stop:0 #1976D2, stop:1 #1565C0);"
        "}"
        "QPushButton#skipBtn {"
        "    background: transparent;"
        "    color: #757575;"
        "    border: 2px solid #BDBDBD;"
        "}"
        "QPushButton#skipBtn:hover {"
        "    background: #F5F5F5;"
        "    border: 2px solid #9E9E9E;"
        "}"
        "QTextEdit {"
        "    border: 2px solid #E0E0E0;"
        "    border-radius: 6px;"
        "    background: white;"
        "    font-size: 13px;"
        "    padding: 10px;"
        "}"
        "QCheckBox {"
        "    spacing: 12px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 22px;"
        "    height: 22px;"
        "}"
        "QScrollArea {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #F5F5F5;"
        "    width: 14px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #BDBDBD;"
        "    min-height: 30px;"
        "    border-radius: 7px;"
        "    margin: 2px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #9E9E9E;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}");

    skipBtn->setObjectName("skipBtn");
}

void StudentSurveyDialog::browseResume()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Resume",
        "",
        "PDF Files (*.pdf);;Word Documents (*.doc *.docx);;All Files (*)");

    if (!fileName.isEmpty())
    {
        resumePathEdit->setText(fileName);
        resumeStatusLabel->setText("✓ Resume uploaded successfully!");

        // Parse resume
        progressBar->setVisible(true);
        progressBar->setValue(30);

        parsedResumeData = parseResumeData(fileName);

        progressBar->setValue(60);

        // Update preview
        QString preview = parsedDataPreview->toPlainText();
        preview += "\n=== RESUME DATA ===\n" + parsedResumeData + "\n";
        parsedDataPreview->setPlainText(preview);

        progressBar->setValue(100);
        progressBar->setVisible(false);
    }
}

void StudentSurveyDialog::browseTranscript()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Transcript",
        "",
        "PDF Files (*.pdf);;All Files (*)");

    if (!fileName.isEmpty())
    {
        transcriptPathEdit->setText(fileName);
        transcriptStatusLabel->setText("✓ Transcript uploaded successfully!");

        // Parse transcript
        progressBar->setVisible(true);
        progressBar->setValue(30);

        parsedGPA = parseTranscriptForGPA(fileName);
        parsedCourses = parseTranscriptForCourses(fileName);

        progressBar->setValue(80);

        // Update preview
        QString preview = parsedDataPreview->toPlainText();
        preview += "\n=== TRANSCRIPT DATA ===\n";
        preview += "GPA: " + parsedGPA + "\n";
        preview += "Courses: " + parsedCourses + "\n";
        parsedDataPreview->setPlainText(preview);

        progressBar->setValue(100);
        progressBar->setVisible(false);
    }
}

QString StudentSurveyDialog::parseResumeData(const QString &filePath)
{
    // Simple placeholder parsing - in production, use PDF parsing library
    // For demo purposes, extract filename and return mock data
    QString fileName = QFileInfo(filePath).fileName();

    QString mockData;
    mockData += "Name: [Extracted from resume]\n";
    mockData += "Email: [Contact info extracted]\n";
    mockData += "Phone: [Contact extracted]\n";
    mockData += "Skills: [Skills section parsed]\n";
    mockData += "Experience: [Work history extracted]\n";
    mockData += "Education: [Education details]\n";
    mockData += "\nNote: Full parsing will be implemented with PDF library.";

    return mockData;
}

QString StudentSurveyDialog::parseTranscriptForGPA(const QString &filePath)
{
    // Simple placeholder - would use PDF library to extract GPA
    // Look for patterns like "GPA: 3.75" or "Cumulative GPA 3.8"

    // Mock for demo
    return "3.75 (Placeholder - will parse from actual PDF)";
}

QString StudentSurveyDialog::parseTranscriptForCourses(const QString &filePath)
{
    // Simple placeholder - would extract course codes and names

    // Mock for demo
    return "CSC 101, CSC 211, MAT 131, ENG 101, ... (Placeholder - will parse from actual PDF)";
}

void StudentSurveyDialog::submitSurvey()
{
    // Validate at least resume or international status provided
    if (resumePathEdit->text().isEmpty() && !internationalCheckbox->isChecked())
    {
        QMessageBox::warning(
            this,
            "Incomplete Information",
            "Please upload your resume or indicate your international student status to continue.");
        return;
    }

    // Save survey data
    bool isInternational = internationalCheckbox->isChecked();
    QString resumePath = resumePathEdit->text();
    QString transcriptPath = transcriptPathEdit->text();

    bool success = database->updateSurveyData(
        currentUserId,
        isInternational,
        resumePath,
        transcriptPath,
        parsedResumeData,
        parsedGPA,
        parsedCourses);

    if (success)
    {
        QMessageBox::information(
            this,
            "Profile Complete",
            "Your profile has been updated successfully! You can edit this information anytime from My Profile.");
        emit surveyCompleted();
        accept();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Error",
            "Failed to save survey data. Please try again.");
    }
}

void StudentSurveyDialog::skipSurvey()
{
    auto reply = QMessageBox::question(
        this,
        "Skip Survey",
        "You can complete this later from My Profile page. Continue?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        emit surveyCompleted();
        accept();
    }
}
