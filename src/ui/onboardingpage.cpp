// #include "OnboardingPage.h"
// #include <QCheckBox>
// #include <QGroupBox>
// #include <QMessageBox>
// #include <QDesktopServices>
// #include <QUrl>

// // ============================================================================
// // OnboardingPage Implementation
// // ============================================================================

// OnboardingPage::OnboardingPage(bool isInternational, QWidget *parent)
//     : QWidget(parent)
//     , m_isInternational(isInternational)
// {
//     setupUI();
    
//     // Load content based on student type
//     if (m_isInternational) {
//         loadInternationalContent();
//     } else {
//         loadWorkStudyContent();
//     }
// }

// OnboardingPage::~OnboardingPage()
// {
//     // Qt automatically cleans up child widgets
// }

// void OnboardingPage::setupUI()
// {
//     // Create main layout
//     m_mainLayout = new QVBoxLayout(this);
//     m_mainLayout->setContentsMargins(0, 0, 0, 0);
//     m_mainLayout->setSpacing(0);
    
//     // Create scroll area for content
//     m_scrollArea = new QScrollArea(this);
//     m_scrollArea->setWidgetResizable(true);
//     m_scrollArea->setFrameShape(QFrame::NoFrame);
//     m_scrollArea->setStyleSheet("QScrollArea { background-color: #F5F5F5; }");
    
//     // Create content widget
//     m_contentWidget = new QWidget();
//     QVBoxLayout* contentLayout = new QVBoxLayout(m_contentWidget);
//     contentLayout->setContentsMargins(40, 40, 40, 40);
//     contentLayout->setSpacing(30);
    
//     // Add sections
//     createHeaderSection();
//     createProcessTypeSection();
//     createDocumentSection();
//     createStepsSection();
//     createContactSection();
    
//     // Add stretch at the bottom
//     contentLayout->addStretch();
    
//     // Set content widget to scroll area
//     m_scrollArea->setWidget(m_contentWidget);
//     m_mainLayout->addWidget(m_scrollArea);
// }

// void OnboardingPage::createHeaderSection()
// {
//     // Header container
//     QWidget* headerWidget = new QWidget();
//     headerWidget->setStyleSheet(QString("background-color: %1; padding: 20px;").arg(COLOR_PRIMARY));
//     QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    
//     // Back button
//     m_backButton = new QPushButton("← Back to Dashboard");
//     m_backButton->setStyleSheet(
//         "QPushButton {"
//         "   background-color: transparent;"
//         "   color: white;"
//         "   border: 2px solid white;"
//         "   padding: 10px 20px;"
//         "   font-size: 14px;"
//         "   font-weight: bold;"
//         "   border-radius: 5px;"
//         "}"
//         "QPushButton:hover {"
//         "   background-color: rgba(255, 255, 255, 0.1);"
//         "}"
//     );
//     connect(m_backButton, &QPushButton::clicked, this, &OnboardingPage::onBackClicked);
    
//     // Title
//     m_titleLabel = new QLabel("Student Onboarding");
//     m_titleLabel->setStyleSheet("color: white; font-size: 32px; font-weight: bold;");
    
//     // Status label
//     m_statusLabel = new QLabel("Work-Study Student Process");
//     m_statusLabel->setStyleSheet("color: white; font-size: 18px; margin-top: 5px;");
    
//     // Description
//     m_descriptionLabel = new QLabel(
//         "Welcome to the Campus Hire onboarding process. Follow the steps below to complete your employment setup."
//     );
//     m_descriptionLabel->setWordWrap(true);
//     m_descriptionLabel->setStyleSheet("color: white; font-size: 14px; margin-top: 10px;");
    
//     // Add to layout
//     headerLayout->addWidget(m_backButton, 0, Qt::AlignLeft);
//     headerLayout->addSpacing(20);
//     headerLayout->addWidget(m_titleLabel);
//     headerLayout->addWidget(m_statusLabel);
//     headerLayout->addWidget(m_descriptionLabel);
    
//     // Add to content
//     m_contentWidget->layout()->addWidget(headerWidget);
// }

// void OnboardingPage::createProcessTypeSection()
// {
//     QFrame* frame = createSectionFrame();
//     QVBoxLayout* layout = new QVBoxLayout(frame);
    
//     // Section title
//     QLabel* titleLabel = new QLabel("📋 Your Onboarding Type");
//     titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333;");
//     layout->addWidget(titleLabel);
    
//     // Info box based on student type
//     QString infoText;
//     QString iconText;
    
//     if (m_isInternational) {
//         iconText = "🌍";
//         infoText = "<b>International Student (F-1 Visa)</b><br><br>"
//                    "As an international student, you have specific employment requirements "
//                    "and must follow F-1 visa regulations. Your employment must be authorized "
//                    "by the International Students Office.";
//     } else {
//         iconText = "🎓";
//         infoText = "<b>Federal Work-Study Student</b><br><br>"
//                    "As a work-study student, you are eligible to work on campus through "
//                    "the Federal Work-Study Career Access Program (FWSCAP). You can work "
//                    "up to 20 hours per week at $15.00 per hour.";
//     }
    
//     QWidget* infoBox = createInfoBox("Student Type", infoText, iconText);
//     layout->addWidget(infoBox);
    
//     m_contentWidget->layout()->addWidget(frame);
// }

// void OnboardingPage::createDocumentSection()
// {
//     QFrame* frame = createSectionFrame();
//     m_documentLayout = new QVBoxLayout(frame);
    
//     // Section title
//     QLabel* titleLabel = new QLabel("📄 Required Documents");
//     titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333;");
//     m_documentLayout->addWidget(titleLabel);
    
//     // Description
//     QLabel* descLabel = new QLabel(
//         "Please review the following document requirements. Check off items as you complete them."
//     );
//     descLabel->setWordWrap(true);
//     descLabel->setStyleSheet("font-size: 14px; color: #666; margin-bottom: 15px;");
//     m_documentLayout->addWidget(descLabel);
    
//     // Document list will be populated by load methods
//     m_documentList = new QListWidget();
//     m_documentList->setStyleSheet(
//         "QListWidget {"
//         "   border: 1px solid #DEE2E6;"
//         "   border-radius: 5px;"
//         "   background-color: white;"
//         "}"
//         "QListWidget::item {"
//         "   padding: 15px;"
//         "   border-bottom: 1px solid #DEE2E6;"
//         "}"
//         "QListWidget::item:last {"
//         "   border-bottom: none;"
//         "}"
//     );
//     m_documentLayout->addWidget(m_documentList);
    
//     m_contentWidget->layout()->addWidget(frame);
// }

// void OnboardingPage::createStepsSection()
// {
//     QFrame* frame = createSectionFrame();
//     m_stepsLayout = new QVBoxLayout(frame);
    
//     // Section title
//     QLabel* titleLabel = new QLabel("📝 Onboarding Steps");
//     titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333;");
//     m_stepsLayout->addWidget(titleLabel);
    
//     // Description
//     QLabel* descLabel = new QLabel(
//         "Follow these steps in order to complete your onboarding process."
//     );
//     descLabel->setWordWrap(true);
//     descLabel->setStyleSheet("font-size: 14px; color: #666; margin-bottom: 15px;");
//     m_stepsLayout->addWidget(descLabel);
    
//     // Steps will be populated by load methods
    
//     m_contentWidget->layout()->addWidget(frame);
// }

// void OnboardingPage::createContactSection()
// {
//     QFrame* frame = createSectionFrame();
//     m_contactLayout = new QVBoxLayout(frame);
    
//     // Section title
//     QLabel* titleLabel = new QLabel("📞 Need Help?");
//     titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #333;");
//     m_contactLayout->addWidget(titleLabel);
    
//     // Contact cards
//     QHBoxLayout* contactCardsLayout = new QHBoxLayout();
//     contactCardsLayout->setSpacing(20);
    
//     // HR Department
//     QFrame* hrCard = new QFrame();
//     hrCard->setFrameShape(QFrame::StyledPanel);
//     hrCard->setStyleSheet(
//         "QFrame {"
//         "   background-color: white;"
//         "   border: 2px solid #E0E0E0;"
//         "   border-radius: 8px;"
//         "   padding: 20px;"
//         "}"
//     );
//     QVBoxLayout* hrLayout = new QVBoxLayout(hrCard);
    
//     QLabel* hrTitle = new QLabel("🏢 Human Resources");
//     hrTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
//     hrLayout->addWidget(hrTitle);
    
//     QLabel* hrInfo = new QLabel(
//         "Room: S-701<br>"
//         "For: I-9 Documentation<br>"
//         "Hours: Mon-Fri 9AM-5PM"
//     );
//     hrInfo->setWordWrap(true);
//     hrInfo->setStyleSheet("font-size: 13px; color: #666; margin-top: 10px;");
//     hrLayout->addWidget(hrInfo);
    
//     QPushButton* hrButton = createStyledButton("Contact HR", false);
//     connect(hrButton, &QPushButton::clicked, [this]() {
//         onContactDepartmentClicked("Human Resources");
//     });
//     hrLayout->addWidget(hrButton);
    
//     contactCardsLayout->addWidget(hrCard);
    
//     // Payroll Department
//     QFrame* payrollCard = new QFrame();
//     payrollCard->setFrameShape(QFrame::StyledPanel);
//     payrollCard->setStyleSheet(
//         "QFrame {"
//         "   background-color: white;"
//         "   border: 2px solid #E0E0E0;"
//         "   border-radius: 8px;"
//         "   padding: 20px;"
//         "}"
//     );
//     QVBoxLayout* payrollLayout = new QVBoxLayout(payrollCard);
    
//     QLabel* payrollTitle = new QLabel("💰 Payroll Office");
//     payrollTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
//     payrollLayout->addWidget(payrollTitle);
    
//     QLabel* payrollInfo = new QLabel(
//         "Room: S-702<br>"
//         "For: Direct Deposit, Tax Forms<br>"
//         "Hours: Mon-Fri 9AM-5PM"
//     );
//     payrollInfo->setWordWrap(true);
//     payrollInfo->setStyleSheet("font-size: 13px; color: #666; margin-top: 10px;");
//     payrollLayout->addWidget(payrollInfo);
    
//     QPushButton* payrollButton = createStyledButton("Contact Payroll", false);
//     connect(payrollButton, &QPushButton::clicked, [this]() {
//         onContactDepartmentClicked("Payroll");
//     });
//     payrollLayout->addWidget(payrollButton);
    
//     contactCardsLayout->addWidget(payrollCard);
    
//     // International Students Office (if applicable)
//     if (m_isInternational) {
//         QFrame* issoCard = new QFrame();
//         issoCard->setFrameShape(QFrame::StyledPanel);
//         issoCard->setStyleSheet(
//             "QFrame {"
//             "   background-color: white;"
//             "   border: 2px solid #E0E0E0;"
//             "   border-radius: 8px;"
//             "   padding: 20px;"
//             "}"
//         );
//         QVBoxLayout* issoLayout = new QVBoxLayout(issoCard);
        
//         QLabel* issoTitle = new QLabel("🌍 International Students Office");
//         issoTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
//         issoLayout->addWidget(issoTitle);
        
//         QLabel* issoInfo = new QLabel(
//             "For: Work Authorization<br>"
//             "Email: isso@bmcc.cuny.edu<br>"
//             "Hours: Mon-Fri 9AM-5PM"
//         );
//         issoInfo->setWordWrap(true);
//         issoInfo->setStyleSheet("font-size: 13px; color: #666; margin-top: 10px;");
//         issoLayout->addWidget(issoInfo);
        
//         QPushButton* issoButton = createStyledButton("Contact ISSO", false);
//         connect(issoButton, &QPushButton::clicked, [this]() {
//             onContactDepartmentClicked("International Students Office");
//         });
//         issoLayout->addWidget(issoButton);
        
//         contactCardsLayout->addWidget(issoCard);
//     }
    
//     m_contactLayout->addLayout(contactCardsLayout);
    
//     // View Guidelines Button
//     QPushButton* guidelinesButton = createStyledButton("📖 View Complete Guidelines", true);
//     guidelinesButton->setMaximumWidth(300);
//     connect(guidelinesButton, &QPushButton::clicked, this, &OnboardingPage::onViewGuidelinesClicked);
//     m_contactLayout->addWidget(guidelinesButton, 0, Qt::AlignCenter);
    
//     m_contentWidget->layout()->addWidget(frame);
// }

// // ============================================================================
// // Helper Methods for UI Creation
// // ============================================================================

// QWidget* OnboardingPage::createInfoBox(const QString& title, const QString& content, const QString& iconText)
// {
//     QFrame* frame = new QFrame();
//     frame->setFrameShape(QFrame::StyledPanel);
//     frame->setStyleSheet(
//         "QFrame {"
//         "   background-color: #E7F3FF;"
//         "   border-left: 4px solid " + COLOR_PRIMARY + ";"
//         "   border-radius: 5px;"
//         "   padding: 15px;"
//         "}"
//     );
    
//     QHBoxLayout* layout = new QHBoxLayout(frame);
    
//     // Icon
//     QLabel* icon = new QLabel(iconText);
//     icon->setStyleSheet("font-size: 32px;");
//     icon->setAlignment(Qt::AlignTop);
//     layout->addWidget(icon);
    
//     // Content
//     QVBoxLayout* contentLayout = new QVBoxLayout();
    
//     QLabel* contentLabel = new QLabel(content);
//     contentLabel->setWordWrap(true);
//     contentLabel->setStyleSheet("font-size: 14px; color: #333;");
//     contentLayout->addWidget(contentLabel);
    
//     layout->addLayout(contentLayout, 1);
    
//     return frame;
// }

// QFrame* OnboardingPage::createSectionFrame()
// {
//     QFrame* frame = new QFrame();
//     frame->setFrameShape(QFrame::StyledPanel);
//     frame->setStyleSheet(
//         "QFrame {"
//         "   background-color: white;"
//         "   border-radius: 8px;"
//         "   padding: 25px;"
//         "}"
//     );
//     return frame;
// }

// QPushButton* OnboardingPage::createStyledButton(const QString& text, bool isPrimary)
// {
//     QPushButton* button = new QPushButton(text);
    
//     if (isPrimary) {
//         button->setStyleSheet(
//             "QPushButton {"
//             "   background-color: " + COLOR_PRIMARY + ";"
//             "   color: white;"
//             "   border: none;"
//             "   padding: 12px 24px;"
//             "   font-size: 14px;"
//             "   font-weight: bold;"
//             "   border-radius: 5px;"
//             "}"
//             "QPushButton:hover {"
//             "   background-color: #0052A3;"
//             "}"
//             "QPushButton:pressed {"
//             "   background-color: #003D7A;"
//             "}"
//         );
//     } else {
//         button->setStyleSheet(
//             "QPushButton {"
//             "   background-color: white;"
//             "   color: " + COLOR_PRIMARY + ";"
//             "   border: 2px solid " + COLOR_PRIMARY + ";"
//             "   padding: 10px 20px;"
//             "   font-size: 13px;"
//             "   font-weight: bold;"
//             "   border-radius: 5px;"
//             "}"
//             "QPushButton:hover {"
//             "   background-color: #E7F3FF;"
//             "}"
//         );
//     }
    
//     button->setCursor(Qt::PointingHandCursor);
//     return button;
// }

// // ============================================================================
// // Content Loading Methods
// // ============================================================================

// void OnboardingPage::loadWorkStudyContent()
// {
//     m_statusLabel->setText("Work-Study Student Process");
    
//     // Clear existing content
//     m_documentList->clear();
    
//     // Clear existing steps
//     QLayoutItem* item;
//     while ((item = m_stepsLayout->takeAt(2)) != nullptr) {
//         delete item->widget();
//         delete item;
//     }
    
//     // Add work-study documents
//     updateDocumentList();
    
//     // Add work-study steps
//     StepCard* step1 = new StepCard(
//         1,
//         "Apply to Federal Work-Study Career Access Program",
//         "Fill out the interest form and attend an information session. You must be awarded "
//         "Federal Work-Study (check your CUNYfirst Financial Aid page) and enrolled in at least 6 credits."
//     );
//     m_stepsLayout->addWidget(step1);
    
//     StepCard* step2 = new StepCard(
//         2,
//         "Complete Online Application",
//         "Apply for positions of interest on the Student Employment Portal. You may be invited "
//         "to attend an individual or group interview."
//     );
//     m_stepsLayout->addWidget(step2);
    
//     StepCard* step3 = new StepCard(
//         3,
//         "Submit Required Documents",
//         "Within 3 days of your start date, submit your original I-9 documentation to HR Office "
//         "Room S-701. Also submit your Social Security Card to Payroll Office Room S-702 before your first paycheck."
//     );
//     m_stepsLayout->addWidget(step3);
    
//     StepCard* step4 = new StepCard(
//         4,
//         "Complete Online Forms",
//         "Submit your W-4 Form and Direct Deposit Form online. Make sure to upload your "
//         "Transcript and CV/Resume as required."
//     );
//     m_stepsLayout->addWidget(step4);
    
//     StepCard* step5 = new StepCard(
//         5,
//         "Attend Program Training",
//         "Learn about timesheet submission process and additional protocols. Confirm your work schedule "
//         "and attend the welcome meeting to meet your team."
//     );
//     m_stepsLayout->addWidget(step5);
    
//     StepCard* step6 = new StepCard(
//         6,
//         "Start Working & Participate",
//         "Begin your position (up to 20 hours/week at $15/hour). Attend career readiness workshops "
//         "and participate in professional development sessions throughout the semester."
//     );
//     m_stepsLayout->addWidget(step6);
// }

// void OnboardingPage::loadInternationalContent()
// {
//     m_statusLabel->setText("International Student (F-1) Process");
    
//     // Clear existing content
//     m_documentList->clear();
    
//     // Clear existing steps
//     QLayoutItem* item;
//     while ((item = m_stepsLayout->takeAt(2)) != nullptr) {
//         delete item->widget();
//         delete item;
//     }
    
//     // Add international student documents
//     updateDocumentList();
    
//     // Add international student steps
//     StepCard* step1 = new StepCard(
//         1,
//         "Verify F-1 Employment Eligibility",
//         "Confirm that you are maintaining valid F-1 status and are enrolled full-time. "
//         "Check with the International Students Office (ISSO) to ensure you are eligible for on-campus employment."
//     );
//     m_stepsLayout->addWidget(step1);
    
//     StepCard* step2 = new StepCard(
//         2,
//         "Obtain Work Authorization",
//         "Visit the International Students Office to get your employment authorization letter. "
//         "This is required before you can begin working. Bring your I-20, passport, and visa documentation."
//     );
//     m_stepsLayout->addWidget(step2);
    
//     StepCard* step3 = new StepCard(
//         3,
//         "Apply for Social Security Number (if needed)",
//         "If you don't have a Social Security Number, apply for one after receiving your employment "
//         "authorization. The ISSO can provide guidance on this process."
//     );
//     m_stepsLayout->addWidget(step3);
    
//     StepCard* step4 = new StepCard(
//         4,
//         "Submit Required Documents",
//         "Submit your I-9 documentation (passport, visa, I-20) to HR Office Room S-701 within 3 days "
//         "of your start date. Also submit your Social Security Card to Payroll Office Room S-702."
//     );
//     m_stepsLayout->addWidget(step4);
    
//     StepCard* step5 = new StepCard(
//         5,
//         "Complete Employment Forms",
//         "Fill out W-4 Form (tax withholding), Direct Deposit Form, and upload required documents. "
//         "International students may have specific tax treaty considerations."
//     );
//     m_stepsLayout->addWidget(step5);
    
//     StepCard* step6 = new StepCard(
//         6,
//         "Understand Work Limitations",
//         "F-1 students can work up to 20 hours/week during the academic year on campus. "
//         "Make sure you understand your work limitations to maintain your visa status."
//     );
//     m_stepsLayout->addWidget(step6);
    
//     StepCard* step7 = new StepCard(
//         7,
//         "Start Working",
//         "Begin your on-campus position. Keep track of your hours and report any changes in your "
//         "employment status to the ISSO immediately."
//     );
//     m_stepsLayout->addWidget(step7);
// }

// void OnboardingPage::updateDocumentList()
// {
//     m_documentList->clear();
    
//     if (m_isInternational) {
//         // International student documents
//         m_documentList->addItem("✓ I-20 Form (Current and Valid)");
//         m_documentList->addItem("✓ Passport (Valid for at least 6 months)");
//         m_documentList->addItem("✓ F-1 Visa");
//         m_documentList->addItem("✓ I-94 Arrival/Departure Record");
//         m_documentList->addItem("✓ Employment Authorization Letter (from ISSO)");
//         m_documentList->addItem("✓ Social Security Card (or application receipt)");
//         m_documentList->addItem("📝 W-4 Form (Tax Withholding) - Submit Online");
//         m_documentList->addItem("📝 Direct Deposit Form - Submit Online");
//         m_documentList->addItem("📄 Transcript - Upload Online");
//         m_documentList->addItem("📄 CV/Resume - Upload Online");
//     } else {
//         // Work-study documents
//         m_documentList->addItem("✓ Original I-9 Documentation → HR Office S-701");
//         m_documentList->addItem("    Deadline: Within 3 days of start date");
//         m_documentList->addItem("");
//         m_documentList->addItem("✓ Social Security Card → Payroll Office S-702");
//         m_documentList->addItem("    Deadline: Before first paycheck");
//         m_documentList->addItem("");
//         m_documentList->addItem("📝 W-4 Form → Submit Online");
//         m_documentList->addItem("    Status: Pending Submission");
//         m_documentList->addItem("");
//         m_documentList->addItem("📝 Direct Deposit Form → Submit Online");
//         m_documentList->addItem("    Status: Pending Submission");
//         m_documentList->addItem("");
//         m_documentList->addItem("📄 Transcript → Upload Online");
//         m_documentList->addItem("    Status: Not Uploaded");
//         m_documentList->addItem("");
//         m_documentList->addItem("📄 CV/Resume → Upload Online");
//         m_documentList->addItem("    Status: Not Uploaded");
//     }
// }

// void OnboardingPage::updateStepsList()
// {
//     // This method can be used to dynamically update steps if needed
// }

// // ============================================================================
// // Public Methods
// // ============================================================================

// void OnboardingPage::setStudentType(bool isInternational)
// {
//     if (m_isInternational == isInternational) {
//         return; // No change needed
//     }
    
//     m_isInternational = isInternational;
    
//     // Reload content
//     if (m_isInternational) {
//         loadInternationalContent();
//     } else {
//         loadWorkStudyContent();
//     }
// }

// // ============================================================================
// // Slot Implementations
// // ============================================================================

// void OnboardingPage::onBackClicked()
// {
//     emit backButtonClicked();
// }

// void OnboardingPage::onDocumentCheckChanged(int state)
// {
//     // Handle document checkbox state changes
//     QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender());
//     if (checkbox) {
//         QString documentName = checkbox->text();
//         bool isComplete = (state == Qt::Checked);
//         emit documentStatusChanged(documentName, isComplete);
//     }
// }

// void OnboardingPage::onViewGuidelinesClicked()
// {
//     QString url;
//     if (m_isInternational) {
//         url = "https://bmccprodstroac.blob.core.windows.net/uploads/ported/international/upload/isso_f1_employment%20trial.pdf";
//     } else {
//         url = "https://www.bmcc.cuny.edu/student-affairs/federal-work-study-career-access/";
//     }
    
//     QDesktopServices::openUrl(QUrl(url));
// }

// void OnboardingPage::onContactDepartmentClicked(const QString& department)
// {
//     QMessageBox msgBox(this);
//     msgBox.setWindowTitle("Contact " + department);
//     msgBox.setIcon(QMessageBox::Information);
    
//     QString message;
//     if (department == "Human Resources") {
//         message = "Human Resources Office\n\n"
//                   "Location: Room S-701\n"
//                   "Hours: Monday - Friday, 9:00 AM - 5:00 PM\n\n"
//                   "Services:\n"
//                   "• I-9 Documentation\n"
//                   "• Employment Verification\n"
//                   "• Benefits Information\n\n"
//                   "Please visit during office hours with your required documents.";
//     } else if (department == "Payroll") {
//         message = "Payroll Office\n\n"
//                   "Location: Room S-702\n"
//                   "Hours: Monday - Friday, 9:00 AM - 5:00 PM\n\n"
//                   "Services:\n"
//                   "• Direct Deposit Setup\n"
//                   "• Tax Forms (W-4)\n"
//                   "• Paycheck Inquiries\n\n"
//                   "Please bring your Social Security Card for verification.";
//     } else if (department == "International Students Office") {
//         message = "International Students Office (ISSO)\n\n"
//                   "Email: isso@bmcc.cuny.edu\n"
//                   "Hours: Monday - Friday, 9:00 AM - 5:00 PM\n\n"
//                   "Services:\n"
//                   "• F-1 Work Authorization\n"
//                   "• Employment Letters\n"
//                   "• Visa Status Questions\n\n"
//                   "Contact them before starting any employment.";
//     }
    
//     msgBox.setText(message);
//     msgBox.exec();
// }

// // ============================================================================
// // DocumentItem Implementation
// // ============================================================================

// DocumentItem::DocumentItem(const QString& title, const QString& location,
//                            const QString& deadline, const QString& status,
//                            QWidget* parent)
//     : QWidget(parent)
//     , m_title(title)
//     , m_location(location)
//     , m_deadline(deadline)
//     , m_status(status)
// {
//     setupUI();
// }

// void DocumentItem::setupUI()
// {
//     QVBoxLayout* layout = new QVBoxLayout(this);
//     layout->setContentsMargins(10, 10, 10, 10);
    
//     // Title with checkbox
//     QHBoxLayout* titleLayout = new QHBoxLayout();
//     QCheckBox* checkbox = new QCheckBox(m_title);
//     checkbox->setStyleSheet("font-size: 14px; font-weight: bold;");
//     titleLayout->addWidget(checkbox);
//     titleLayout->addStretch();
    
//     // Status label
//     QLabel* statusLabel = new QLabel(m_status);
//     if (m_status == "Pending Submission") {
//         statusLabel->setStyleSheet("color: #FFC107; font-weight: bold;");
//     } else if (m_status == "Not Uploaded") {
//         statusLabel->setStyleSheet("color: #DC3545; font-weight: bold;");
//     } else {
//         statusLabel->setStyleSheet("color: #6C757D;");
//     }
//     titleLayout->addWidget(statusLabel);
    
//     layout->addLayout(titleLayout);
    
//     // Location info
//     if (!m_location.isEmpty()) {
//         QLabel* locationLabel = new QLabel("📍 " + m_location);
//         locationLabel->setStyleSheet("font-size: 12px; color: #666; margin-left: 25px;");
//         layout->addWidget(locationLabel);
//     }
    
//     // Deadline info
//     if (!m_deadline.isEmpty()) {
//         QLabel* deadlineLabel = new QLabel("⏰ " + m_deadline);
//         deadlineLabel->setStyleSheet("font-size: 12px; color: #666; margin-left: 25px;");
//         layout->addWidget(deadlineLabel);
//     }
// }

// // ============================================================================
// // StepCard Implementation
// // ============================================================================

// StepCard::StepCard(int stepNumber, const QString& title, const QString& description, QWidget* parent)
//     : QFrame(parent)
//     , m_stepNumber(stepNumber)
//     , m_title(title)
//     , m_description(description)
// {
//     setupUI();
// }

// void StepCard::setupUI()
// {
//     setFrameShape(QFrame::StyledPanel);
//     setStyleSheet(
//         "StepCard {"
//         "   background-color: #F8F9FA;"
//         "   border-left: 4px solid #0066CC;"
//         "   border-radius: 5px;"
//         "   padding: 15px;"
//         "   margin-bottom: 15px;"
//         "}"
//     );
    
//     QHBoxLayout* mainLayout = new QHBoxLayout(this);
    
//     // Step number circle
//     QLabel* numberLabel = new QLabel(QString::number(m_stepNumber));
//     numberLabel->setAlignment(Qt::AlignCenter);
//     numberLabel->setFixedSize(40, 40);
//     numberLabel->setStyleSheet(
//         "QLabel {"
//         "   background-color: #0066CC;"
//         "   color: white;"
//         "   font-size: 18px;"
//         "   font-weight: bold;"
//         "   border-radius: 20px;"
//         "}"
//     );
//     mainLayout->addWidget(numberLabel, 0, Qt::AlignTop);
    
//     // Content
//     QVBoxLayout* contentLayout = new QVBoxLayout();
    
//     QLabel* titleLabel = new QLabel(m_title);
//     titleLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #333;");
//     titleLabel->setWordWrap(true);
//     contentLayout->addWidget(titleLabel);
    
//     QLabel* descLabel = new QLabel(m_description);
//     descLabel->setStyleSheet("font-size: 13px; color: #666; margin-top: 5px;");
//     descLabel->setWordWrap(true);
//     contentLayout->addWidget(descLabel);
    
//     mainLayout->addLayout(contentLayout, 1);
// }