#ifndef ONBOARDINGPAGE_H
#define ONBOARDINGPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

// Simple onboarding page for demo
class OnboardingPage : public QWidget
{
    Q_OBJECT

public:
    // Create onboarding page
    // isInternational: true for F-1 students, false for work-study
    OnboardingPage(bool isInternational = false, QWidget *parent = nullptr);

signals:
    void backButtonClicked();      // User clicked back button
    void viewApplicationClicked(); // User wants to see application page

private slots:
    void onBackClicked();      // Handle back button click
    void onViewGuideClicked(); // Handle view guide button click

private:
    // Setup functions
    void setupUI();     // Create all the widgets
    void loadContent(); // Load documents and steps based on student type

    // Data
    bool isInternational; // Student type

    // UI widgets
    QLabel *titleLabel;           // Page title
    QLabel *studentTypeLabel;     // Shows Arts or Science
    QPushButton *backButton;      // Back button
    QPushButton *viewGuideButton; // View guide button
    QListWidget *documentList;    // List of required documents
    QListWidget *stepsList;       // List of onboarding steps
};

#endif // ONBOARDINGPAGE_H
