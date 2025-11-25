#include "ui/profilewidget.h"
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileInfo>
#include <QHeaderView>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

ProfileWidget::ProfileWidget(Database *db, QWidget *parent)
    : QWidget(parent), database(db), currentUserId(-1)
{

    // Initialize document status
    docStatus.pendingCount = 0;
    docStatus.completedCount = 0;
    docStatus.processingCount = 0;

    setupUI();
    setupStyles();
}

void ProfileWidget::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Personal Information Section
    auto personalInfoGroup = new QGroupBox("Personal Information", this);
    auto personalLayout = new QVBoxLayout(personalInfoGroup);
    personalLayout->setSpacing(10);

    nameLabel = new QLabel(this);
    nameLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1976D2;");

    // Create form layout for better organization
    auto formWidget = new QWidget(this);
    auto formLayout = new QVBoxLayout(formWidget);

    // EMPLID
    auto emplidLabel = new QLabel("EMPLID:", this);
    emplidEdit = new QLineEdit(this);
    emplidEdit->setPlaceholderText("Enter EMPLID");
    emplidEdit->setReadOnly(true);

    // Major
    auto majorLabel = new QLabel("Major:", this);
    majorEdit = new QLineEdit(this);
    majorEdit->setPlaceholderText("Enter your major");

    // GPA
    auto gpaLabel = new QLabel("GPA:", this);
    gpaEdit = new QLineEdit(this);
    gpaEdit->setPlaceholderText("0.00 - 4.00");
    gpaEdit->setMaxLength(4);

    // Graduation Date
    auto gradLabel = new QLabel("Expected Graduation:", this);
    gradDateEdit = new QDateEdit(this);
    gradDateEdit->setDisplayFormat("MM/yyyy");
    gradDateEdit->setCalendarPopup(true);
    gradDateEdit->setDate(QDate::currentDate());

    personalLayout->addWidget(nameLabel);
    personalLayout->addSpacing(10);
    personalLayout->addWidget(emplidLabel);
    personalLayout->addWidget(emplidEdit);
    personalLayout->addWidget(majorLabel);
    personalLayout->addWidget(majorEdit);
    personalLayout->addWidget(gpaLabel);
    personalLayout->addWidget(gpaEdit);
    personalLayout->addWidget(gradLabel);
    personalLayout->addWidget(gradDateEdit);

    mainLayout->addWidget(personalInfoGroup);

    // Document Upload Section
    auto documentsGroup = new QGroupBox("Document Upload", this);
    auto documentsLayout = new QVBoxLayout(documentsGroup);
    documentsLayout->setSpacing(10);

    uploadTranscriptBtn = new QPushButton("📄 Upload Transcript (PDF)", this);
    transcriptLabel = new QLabel("No transcript uploaded", this);
    transcriptLabel->setStyleSheet("color: #757575; font-style: italic;");

    uploadCVBtn = new QPushButton("📝 Upload CV/Resume", this);
    cvLabel = new QLabel("No CV uploaded", this);
    cvLabel->setStyleSheet("color: #757575; font-style: italic;");

    documentsLayout->addWidget(uploadTranscriptBtn);
    documentsLayout->addWidget(transcriptLabel);
    documentsLayout->addSpacing(10);
    documentsLayout->addWidget(uploadCVBtn);
    documentsLayout->addWidget(cvLabel);

    mainLayout->addWidget(documentsGroup);

    // Document Status Overview
    auto statusGroup = new QGroupBox("Document Status Overview", this);
    auto statusLayout = new QHBoxLayout(statusGroup);
    statusLayout->setSpacing(15);

    pendingDocsLabel = new QLabel("⏳ Pending: 0", this);
    completedDocsLabel = new QLabel("✓ Completed: 0", this);
    processingDocsLabel = new QLabel("⚙ Processing: 0", this);

    pendingDocsLabel->setAlignment(Qt::AlignCenter);
    completedDocsLabel->setAlignment(Qt::AlignCenter);
    processingDocsLabel->setAlignment(Qt::AlignCenter);

    pendingDocsLabel->setStyleSheet(
        "background: #FFF9C4; padding: 15px; border-radius: 8px; "
        "font-weight: bold; border: 2px solid #FBC02D;");
    completedDocsLabel->setStyleSheet(
        "background: #C8E6C9; padding: 15px; border-radius: 8px; "
        "font-weight: bold; border: 2px solid #43A047;");
    processingDocsLabel->setStyleSheet(
        "background: #BBDEFB; padding: 15px; border-radius: 8px; "
        "font-weight: bold; border: 2px solid #1976D2;");

    statusLayout->addWidget(pendingDocsLabel);
    statusLayout->addWidget(completedDocsLabel);
    statusLayout->addWidget(processingDocsLabel);

    mainLayout->addWidget(statusGroup);

    // Document History Table
    auto tableGroup = new QGroupBox("Document History", this);
    auto tableLayout = new QVBoxLayout(tableGroup);

    documentsTable = new QTableWidget(this);
    documentsTable->setColumnCount(4);
    documentsTable->setHorizontalHeaderLabels({"Document Type", "Upload Date", "Status", "File Name"});
    documentsTable->horizontalHeader()->setStretchLastSection(true);
    documentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    documentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    documentsTable->setAlternatingRowColors(true);
    documentsTable->verticalHeader()->setVisible(false);

    tableLayout->addWidget(documentsTable);
    mainLayout->addWidget(tableGroup);

    // Buttons
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(10);

    saveButton = new QPushButton("💾 Save Changes", this);
    onboardingButton = new QPushButton("📋 View Onboarding Status", this);

    saveButton->setMinimumHeight(40);
    onboardingButton->setMinimumHeight(40);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(onboardingButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    // Connect signals
    connect(saveButton, &QPushButton::clicked, this, &ProfileWidget::handleSaveChanges);
    connect(onboardingButton, &QPushButton::clicked, this, &ProfileWidget::handleOnboarding);
    connect(uploadTranscriptBtn, &QPushButton::clicked, this, &ProfileWidget::uploadTranscript);
    connect(uploadCVBtn, &QPushButton::clicked, this, &ProfileWidget::uploadCV);
}

void ProfileWidget::setupStyles()
{
    setStyleSheet(
        "QWidget {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #ffffff, stop:1 #f8f9fa);"
        "}"
        "QGroupBox { "
        "   font-weight: bold; "
        "   font-size: 14px; "
        "   border: 2px solid #BDBDBD; "
        "   border-radius: 10px; "
        "   margin-top: 14px; "
        "   padding-top: 12px; "
        "   background: white;"
        "}"
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 15px; "
        "   padding: 0 8px; "
        "   color: #1976D2; "
        "   background: white;"
        "}"
        "QLineEdit, QDateEdit { "
        "   padding: 12px; "
        "   border: 2px solid #E0E0E0; "
        "   border-radius: 8px; "
        "   background: white; "
        "   font-size: 14px; "
        "   min-height: 40px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus { "
        "   border: 2px solid #2196F3; "
        "   background: #f8fcff;"
        "}"
        "QLineEdit:read-only { "
        "   background: #F5F5F5; "
        "   color: #757575; "
        "}"
        "QPushButton { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #2196F3, stop:1 #1976D2);"
        "   color: white; "
        "   padding: 12px 24px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   font-size: 14px; "
        "   border: none; "
        "   min-height: 44px;"
        "}"
        "QPushButton:hover { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #1976D2, stop:1 #0D47A1);"
        "}"
        "QPushButton:pressed { "
        "   background-color: #0D47A1; "
        "}"
        "QLabel { "
        "   color: #424242; "
        "   font-size: 13px; "
        "}"
        "QTableWidget { "
        "   border: 1px solid #E0E0E0; "
        "   border-radius: 8px; "
        "   gridline-color: #F5F5F5; "
        "   background: white;"
        "}"
        "QHeaderView::section { "
        "   background-color: #F5F5F5; "
        "   padding: 10px; "
        "   border: none; "
        "   font-weight: bold; "
        "   color: #424242; "
        "}");

    onboardingButton->setStyleSheet(
        "QPushButton { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #4CAF50, stop:1 #388E3C);"
        "   min-height: 44px;"
        "}"
        "QPushButton:hover { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #388E3C, stop:1 #2E7D32);"
        "}"
        "QPushButton:pressed { "
        "   background-color: #2E7D32; "
        "}");

    // Apply smooth fade-in effect when profile widget is shown
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);

    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(400);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void ProfileWidget::setUserId(int id)
{
    currentUserId = id;
    loadUserProfile(id);
}

void ProfileWidget::loadUserProfile(int userId)
{
    if (!database || userId < 0)
    {
        qWarning() << "Invalid database or user ID";
        return;
    }

    currentUserId = userId;

    // Load user information from database
    QSqlQuery query;
    query.prepare("SELECT username, emplid, major, gpa, graduation_date FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (query.exec() && query.next())
    {
        QString username = query.value("username").toString();
        QString emplid = query.value("emplid").toString();
        QString major = query.value("major").toString();
        double gpa = query.value("gpa").toDouble();
        QString gradDateStr = query.value("graduation_date").toString();

        nameLabel->setText("👤 " + username);
        emplidEdit->setText(emplid);
        majorEdit->setText(major);
        gpaEdit->setText(QString::number(gpa, 'f', 2));

        // Parse graduation date
        QDate gradDate = QDate::fromString(gradDateStr, "yyyy-MM-dd");
        if (gradDate.isValid())
        {
            gradDateEdit->setDate(gradDate);
        }
    }
    else
    {
        qWarning() << "Failed to load user profile:" << query.lastError().text();
    }

    // Load documents and status
    loadDocuments();
    updateDocumentCounts();
}

void ProfileWidget::loadDocuments()
{
    if (!database || currentUserId < 0)
        return;

    documentsTable->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT document_type, file_path, upload_date, status FROM documents WHERE user_id = :user_id ORDER BY upload_date DESC");
    query.bindValue(":user_id", currentUserId);

    if (query.exec())
    {
        int row = 0;
        while (query.next())
        {
            documentsTable->insertRow(row);

            QString docType = query.value("document_type").toString();
            QString filePath = query.value("file_path").toString();
            QString uploadDate = query.value("upload_date").toString();
            QString status = query.value("status").toString();

            documentsTable->setItem(row, 0, new QTableWidgetItem(docType));
            documentsTable->setItem(row, 1, new QTableWidgetItem(uploadDate));
            documentsTable->setItem(row, 2, new QTableWidgetItem(status));
            documentsTable->setItem(row, 3, new QTableWidgetItem(QFileInfo(filePath).fileName()));

            // Update current document labels
            if (docType == "Transcript" && status != "Deleted")
            {
                transcriptLabel->setText("✓ " + QFileInfo(filePath).fileName());
                transcriptLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
                docStatus.transcriptPath = filePath;
            }
            else if (docType == "CV" && status != "Deleted")
            {
                cvLabel->setText("✓ " + QFileInfo(filePath).fileName());
                cvLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
                docStatus.cvPath = filePath;
            }

            row++;
        }
    }
}

void ProfileWidget::updateDocumentCounts()
{
    if (!database || currentUserId < 0)
        return;

    QSqlQuery query;
    query.prepare("SELECT status, COUNT(*) as count FROM documents WHERE user_id = :user_id GROUP BY status");
    query.bindValue(":user_id", currentUserId);

    docStatus.pendingCount = 0;
    docStatus.completedCount = 0;
    docStatus.processingCount = 0;

    if (query.exec())
    {
        while (query.next())
        {
            QString status = query.value("status").toString();
            int count = query.value("count").toInt();

            if (status == "Pending")
            {
                docStatus.pendingCount = count;
            }
            else if (status == "Completed" || status == "Approved")
            {
                docStatus.completedCount = count;
            }
            else if (status == "Processing" || status == "Under Review")
            {
                docStatus.processingCount = count;
            }
        }
    }

    pendingDocsLabel->setText(QString("⏳ Pending: %1").arg(docStatus.pendingCount));
    completedDocsLabel->setText(QString("✓ Completed: %1").arg(docStatus.completedCount));
    processingDocsLabel->setText(QString("⚙ Processing: %1").arg(docStatus.processingCount));
}

bool ProfileWidget::validateInputs()
{
    // Validate GPA
    bool ok;
    double gpa = gpaEdit->text().toDouble(&ok);
    if (!ok || gpa < 0.0 || gpa > 4.0)
    {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid GPA between 0.00 and 4.00");
        gpaEdit->setFocus();
        return false;
    }

    // Validate major
    if (majorEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Invalid Input", "Please enter your major");
        majorEdit->setFocus();
        return false;
    }

    return true;
}

void ProfileWidget::handleSaveChanges()
{
    if (!database || currentUserId < 0)
    {
        QMessageBox::warning(this, "Error", "No user loaded");
        return;
    }

    if (!validateInputs())
    {
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET major = :major, gpa = :gpa, graduation_date = :grad_date WHERE id = :id");
    query.bindValue(":major", majorEdit->text().trimmed());
    query.bindValue(":gpa", gpaEdit->text().toDouble());
    query.bindValue(":grad_date", gradDateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":id", currentUserId);

    if (query.exec())
    {
        QMessageBox::information(this, "Success", "✓ Profile updated successfully!");
        emit profileUpdated();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to update profile:\n" + query.lastError().text());
    }
}

void ProfileWidget::handleOnboarding()
{
    showDocumentStatus();
}

void ProfileWidget::uploadTranscript()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Upload Transcript",
        QDir::homePath(),
        "PDF Files (*.pdf);;All Files (*)");

    if (!fileName.isEmpty())
    {
        saveDocumentToDatabase(fileName, "Transcript");
        transcriptLabel->setText("✓ " + QFileInfo(fileName).fileName());
        transcriptLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
        loadDocuments();
        updateDocumentCounts();
    }
}

void ProfileWidget::uploadCV()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Upload CV/Resume",
        QDir::homePath(),
        "PDF Files (*.pdf);;Word Documents (*.doc *.docx);;All Files (*)");

    if (!fileName.isEmpty())
    {
        saveDocumentToDatabase(fileName, "CV");
        cvLabel->setText("✓ " + QFileInfo(fileName).fileName());
        cvLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
        loadDocuments();
        updateDocumentCounts();
    }
}

void ProfileWidget::saveDocumentToDatabase(const QString &filePath, const QString &documentType)
{
    if (!database || currentUserId < 0)
        return;

    QSqlQuery query;
    query.prepare(
        "INSERT INTO documents (user_id, document_type, file_path, upload_date, status) "
        "VALUES (:user_id, :doc_type, :file_path, :upload_date, :status)");
    query.bindValue(":user_id", currentUserId);
    query.bindValue(":doc_type", documentType);
    query.bindValue(":file_path", filePath);
    query.bindValue(":upload_date", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":status", "Pending");

    if (!query.exec())
    {
        qWarning() << "Failed to save document:" << query.lastError().text();
        QMessageBox::warning(this, "Error", "Failed to save document to database");
    }
}

void ProfileWidget::refreshDocumentStatus()
{
    loadDocuments();
    updateDocumentCounts();
}

void ProfileWidget::showDocumentStatus()
{
    QString statusText = "<h3>📋 Onboarding Document Status</h3>";

    statusText += "<h4 style='color: #F57C00;'>🏢 In-Person Document Requirements:</h4>";
    statusText += "<ul>";
    statusText += "<li><b>Original I-9 Documentation</b><br>"
                  "Submit to: HR Office Room S-701<br>"
                  "Deadline: Within 3 days of start date</li><br>";
    statusText += "<li><b>Social Security Card</b><br>"
                  "Submit to: Payroll Office Room S-702<br>"
                  "Deadline: Before first paycheck</li>";
    statusText += "</ul>";

    statusText += "<h4 style='color: #1976D2;'>💻 Online Submission Requirements:</h4>";
    statusText += "<ul>";
    statusText += "<li>W-4 Form: <span style='color: #F57C00;'>Pending Submission</span></li>";
    statusText += "<li>Direct Deposit Form: <span style='color: #F57C00;'>Pending Submission</span></li>";

    if (!docStatus.transcriptPath.isEmpty())
    {
        statusText += "<li>Transcript: <span style='color: #4CAF50;'>✓ Uploaded</span></li>";
    }
    else
    {
        statusText += "<li>Transcript: <span style='color: #F57C00;'>Not Uploaded</span></li>";
    }

    if (!docStatus.cvPath.isEmpty())
    {
        statusText += "<li>CV/Resume: <span style='color: #4CAF50;'>✓ Uploaded</span></li>";
    }
    else
    {
        statusText += "<li>CV/Resume: <span style='color: #F57C00;'>Not Uploaded</span></li>";
    }
    statusText += "</ul>";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Onboarding Status");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(statusText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}