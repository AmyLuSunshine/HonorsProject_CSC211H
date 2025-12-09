#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QHBoxLayout>
#include "../database/database.h"

class LandingPage : public QWidget
{
    Q_OBJECT

public:
    // Constructor - creates the landing page
    explicit LandingPage(Database *db, int userId, QWidget *parent = nullptr);

    // Allow updating the user ID after construction
    void setUserId(int userId) { currentUserId = userId; }

signals:
    // Signal emitted when user clicks "Let's Get Started"
    void startSurvey();

    // Signal emitted when user clicks "Skip for Now"
    void skipToJobs();

    // Signals for feature box clicks
    void jobsBoxClicked();
    void onboardingBoxClicked();
    void interviewBoxClicked();

private:
    // Setup functions
    void setupUI();     // Creates all widgets and layouts
    void setupStyles(); // Applies styling

    // Helper function to create feature boxes
    QPushButton* createFeatureBox(const QString& title, const QString& description, const QString& bgColor);
   
    // Database and user info
    Database *database;
    int currentUserId;

    // UI Components
    QScrollArea *scrollArea;     // Allows scrolling if content is too tall
    QLabel *titleLabel;          // Main title
    QLabel *subtitleLabel;       // Subtitle under main title
    QLabel *descriptionLabel;    // Description paragraph
    QPushButton *getStartedBtn;  // Primary action button
    QPushButton *skipBtn;        // Secondary action button
    QHBoxLayout *featuresLayout; // Layout holding the 3 feature boxes
};

#endif // LANDINGPAGE_H