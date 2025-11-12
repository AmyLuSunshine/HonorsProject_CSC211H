#include "ui/myapplicationpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

MyApplicationPage::MyApplicationPage(Database* db, QWidget* parent)
    : QWidget(parent), database(db), currentUserId(-1)
{
    setupUI();
    setupStyles();

    // Fade-in
    auto* effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);
    auto* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(350);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}

void MyApplicationPage::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Upload section
    auto documentsGroup = new QGroupBox("Document Upload", this);
    auto documentsLayout = new QVBoxLayout(documentsGroup);

    uploadTranscriptBtn = new QPushButton("📄 Upload Transcript (PDF)", this);
    transcriptLabel = new QLabel("No transcript uploaded", this);
    transcriptLabel->setStyleSheet("color:#757575; font-style: italic;");

    uploadCVBtn = new QPushButton("📝 Upload CV/Resume", this);
    cvLabel = new QLabel("No CV uploaded", this);
    cvLabel->setStyleSheet("color:#757575; font-style: italic;");

    documentsLayout->addWidget(uploadTranscriptBtn);
    documentsLayout->addWidget(transcriptLabel);
    documentsLayout->addSpacing(8);
    documentsLayout->addWidget(uploadCVBtn);
    documentsLayout->addWidget(cvLabel);

    connect(uploadTranscriptBtn, &QPushButton::clicked, this, &MyApplicationPage::uploadTranscript);
    connect(uploadCVBtn, &QPushButton::clicked, this, &MyApplicationPage::uploadCV);

    mainLayout->addWidget(documentsGroup);

    // Status overview
    auto statusGroup = new QGroupBox("Document Status Overview", this);
    auto statusLayout = new QHBoxLayout(statusGroup);
    statusLayout->setSpacing(15);
    pendingDocsLabel = new QLabel("⏳ Pending: 0", this);
    completedDocsLabel = new QLabel("✓ Completed: 0", this);
    processingDocsLabel = new QLabel("⚙ Processing: 0", this);

    for (auto* lbl : {pendingDocsLabel, completedDocsLabel, processingDocsLabel}) {
        lbl->setAlignment(Qt::AlignCenter);
    }
    pendingDocsLabel->setStyleSheet("background:#FFF9C4; padding:15px; border-radius:8px; font-weight:bold; border:2px solid #FBC02D;");
    completedDocsLabel->setStyleSheet("background:#C8E6C9; padding:15px; border-radius:8px; font-weight:bold; border:2px solid #43A047;");
    processingDocsLabel->setStyleSheet("background:#BBDEFB; padding:15px; border-radius:8px; font-weight:bold; border:2px solid #1976D2;");

    statusLayout->addWidget(pendingDocsLabel);
    statusLayout->addWidget(completedDocsLabel);
    statusLayout->addWidget(processingDocsLabel);
    mainLayout->addWidget(statusGroup);

    // Document history table
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

    // Actions and onboarding
    auto actions = new QHBoxLayout();
    saveButton = new QPushButton("Save Changes", this);
    onboardingButton = new QPushButton("View Onboarding Status", this);
    actions->addStretch();
    actions->addWidget(saveButton);
    actions->addWidget(onboardingButton);
    mainLayout->addLayout(actions);

    connect(onboardingButton, &QPushButton::clicked, this, &MyApplicationPage::showOnboardingStatus);
}

void MyApplicationPage::setupStyles()
{
    setStyleSheet(
        "QWidget { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #ffffff, stop:1 #f8f9fa);"
        " font-family: 'Segoe UI', Arial, sans-serif; }"
        "QGroupBox { font-weight: 600; font-size: 14px; border: 2px solid #BDBDBD; border-radius: 10px;"
        " margin-top: 14px; padding-top: 12px; background: white; }"
        "QGroupBox::title { left: 15px; padding: 0 8px; color: #1976D2; background: white; }"
        "QPushButton { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #2196F3, stop:1 #1976D2); color:white;"
        " padding: 12px 24px; border-radius: 8px; font-weight: 600; font-size: 14px; border: none; min-height: 40px; }"
        "QPushButton:hover { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #1976D2, stop:1 #0D47A1); }"
        "QTableWidget { border: 1px solid #E0E0E0; border-radius: 8px; gridline-color: #F5F5F5; background: white; }"
        "QHeaderView::section { background-color: #F5F5F5; padding: 10px; border: none; font-weight: bold; color: #424242; }"
    );
}

void MyApplicationPage::setUserId(int userId)
{
    currentUserId = userId;
    loadDocuments();
    updateDocumentCounts();
}

void MyApplicationPage::loadDocuments()
{
    documentsTable->setRowCount(0);
    if (!database || currentUserId < 0) return;
    auto docs = database->getUserDocuments(currentUserId);
    int row = 0;
    for (const auto& d : docs) {
        documentsTable->insertRow(row);
        documentsTable->setItem(row, 0, new QTableWidgetItem(d.documentType));
        documentsTable->setItem(row, 1, new QTableWidgetItem(d.uploadDate));
        documentsTable->setItem(row, 2, new QTableWidgetItem(d.status));
        documentsTable->setItem(row, 3, new QTableWidgetItem(QFileInfo(d.filePath).fileName()));
        row++;
    }
}

void MyApplicationPage::updateDocumentCounts()
{
    if (!database || currentUserId < 0) return;
    int pending = database->getDocumentCountByStatus(currentUserId, "Pending");
    int completed = database->getDocumentCountByStatus(currentUserId, "Completed");
    int processing = database->getDocumentCountByStatus(currentUserId, "Processing");
    pendingDocsLabel->setText(QString("⏳ Pending: %1").arg(pending));
    completedDocsLabel->setText(QString("✓ Completed: %1").arg(completed));
    processingDocsLabel->setText(QString("⚙ Processing: %1").arg(processing));
}

void MyApplicationPage::uploadTranscript()
{
    if (currentUserId < 0 || !database) return;
    QString filePath = QFileDialog::getOpenFileName(this, "Select Transcript (PDF)", QString(), "PDF Files (*.pdf);;All Files (*.*)");
    if (filePath.isEmpty()) return;
    if (database->addDocument(currentUserId, "Transcript", filePath, "Pending")) {
        transcriptLabel->setText(QFileInfo(filePath).fileName());
        loadDocuments();
        updateDocumentCounts();
        emit documentsUpdated();
    } else {
        QMessageBox::warning(this, "Upload", "Failed to save transcript record.");
    }
}

void MyApplicationPage::uploadCV()
{
    if (currentUserId < 0 || !database) return;
    QString filePath = QFileDialog::getOpenFileName(this, "Select CV/Resume", QString(), "PDF Files (*.pdf);;All Files (*.*)");
    if (filePath.isEmpty()) return;
    if (database->addDocument(currentUserId, "CV", filePath, "Pending")) {
        cvLabel->setText(QFileInfo(filePath).fileName());
        loadDocuments();
        updateDocumentCounts();
        emit documentsUpdated();
    } else {
        QMessageBox::warning(this, "Upload", "Failed to save CV record.");
    }
}

void MyApplicationPage::showOnboardingStatus()
{
    // Simple dialog replicating the screenshot with rich text styling
    QString html = R"(
        <div style='font-family: Segoe UI, Arial; font-size:13px;'>
        <h3 style='color:#0D47A1;'>📑 Onboarding Document Status</h3>
        <h4 style='color:#E65100;'>In-Person Document Requirements:</h4>
        <p><b>• Original I-9 Documentation</b><br/>
           Submit to: HR Office Room S-701<br/>
           Deadline: Within 3 days of start date</p>
        <p><b>• Social Security Card</b><br/>
           Submit to: Payroll Office Room S-702<br/>
           Deadline: Before first paycheck</p>
        <h4 style='color:#0D47A1;'>Online Submission Requirements:</h4>
        <ul>
          <li>W-4 Form: <span style='color:#F57C00;'>Pending Submission</span></li>
          <li>Direct Deposit Form: <span style='color:#F57C00;'>Pending Submission</span></li>
          <li>Transcript: <span style='color:#D32F2F;'>Not Uploaded</span></li>
          <li>CV/Resume: <span style='color:#D32F2F;'>Not Uploaded</span></li>
        </ul>
        </div>
    )";
    QMessageBox box(this);
    box.setWindowTitle("Onboarding Status");
    box.setIcon(QMessageBox::Information);
    box.setTextFormat(Qt::RichText);
    box.setText(html);
    box.exec();
}
