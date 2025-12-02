// #ifndef ONBOARDINGPAGE_H
// #define ONBOARDINGPAGE_H

// #include <QWidget>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QLabel>
// #include <QPushButton>
// #include <QScrollArea>
// #include <QFrame>
// #include <QListWidget>
// #include <QStackedWidget>
// #include <QString>
// #include <QIcon>

// // Forward declarations for helper classes
// class DocumentItem;
// class StepCard;

// /**
//  * @brief Main onboarding page widget
//  * 
//  * This class displays the onboarding process for students based on their status:
//  * - Work-Study students (domestic)
//  * - International students (F-1 visa holders)
//  * 
// //  * The page shows required documents, submission deadlines, and step-by-step guidance.
//  */
// class OnboardingPage : public QWidget {
//     Q_OBJECT

// public:
//     /**
//      * @brief Constructor
//      * @param isInternational - true if student is international, false for domestic
//      * @param parent - parent widget
//      */
//     explicit OnboardingPage(bool isInternational = false, QWidget *parent = nullptr);
//     ~OnboardingPage();

//     // Getters
//     bool isInternationalStudent() const { return m_isInternational; }

//     // Setters
//     void setStudentType(bool isInternational);

// signals:
//     void backButtonClicked();
//     void documentStatusChanged(const QString& documentName, bool isComplete);

// private slots:
//     void onBackClicked();
//     void onDocumentCheckChanged(int state);
//     void onViewGuidelinesClicked();
//     void onContactDepartmentClicked(const QString& department);

// private:
//     // Setup methods
//     void setupUI();
//     void createHeaderSection();
//     void createProcessTypeSection();
//     void createDocumentSection();
//     void createStepsSection();
//     void createContactSection();
    
//     // Helper methods for UI creation
//     QWidget* createInfoBox(const QString& title, const QString& content, 
//                            const QString& iconText = "ℹ️");
//     QFrame* createSectionFrame();
//     QPushButton* createStyledButton(const QString& text, bool isPrimary = false);
    
//     // Content population methods
//     void loadWorkStudyContent();
//     void loadInternationalContent();
//     void updateDocumentList();
//     void updateStepsList();
    
//     // Member variables
//     bool m_isInternational;
    
//     // Main layout
//     QVBoxLayout* m_mainLayout;
//     QScrollArea* m_scrollArea;
//     QWidget* m_contentWidget;
    
//     // Section widgets
//     QLabel* m_titleLabel;
//     QLabel* m_statusLabel;
//     QLabel* m_descriptionLabel;
//     QPushButton* m_backButton;
    
//     // Document section
//     QVBoxLayout* m_documentLayout;
//     QListWidget* m_documentList;
    
//     // Steps section
//     QVBoxLayout* m_stepsLayout;
    
//     // Contact section
//     QVBoxLayout* m_contactLayout;
    
//     // Style colors
//     const QString COLOR_PRIMARY = "#0066CC";
//     const QString COLOR_SUCCESS = "#28A745";
//     const QString COLOR_WARNING = "#FFC107";
//     const QString COLOR_DANGER = "#DC3545";
//     const QString COLOR_LIGHT = "#F8F9FA";
//     const QString COLOR_BORDER = "#DEE2E6";
// };

// /**
//  * @brief Document item widget for checklist
//  * 
//  * Displays a single document requirement with checkbox and status
//  */
// class DocumentItem : public QWidget {
//     Q_OBJECT

// public:
//     explicit DocumentItem(const QString& title, const QString& location,
//                           const QString& deadline, const QString& status,
//                           QWidget* parent = nullptr);

// signals:
//     void statusChanged(bool isComplete);

// private:
//     void setupUI();
    
//     QString m_title;
//     QString m_location;
//     QString m_deadline;
//     QString m_status;
// };

// /**
//  * @brief Step card widget for process steps
//  * 
//  * Displays a single step in the onboarding process
//  */
// class StepCard : public QFrame {
//     Q_OBJECT

// public:
//     explicit StepCard(int stepNumber, const QString& title, 
//                       const QString& description, QWidget* parent = nullptr);

// private:
//     void setupUI();
    
//     int m_stepNumber;
//     QString m_title;
//     QString m_description;
// };

// #endif // ONBOARDINGPAGE_H