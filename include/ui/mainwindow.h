#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include "database/database.h"
#include "models/user.h"
#include "ui/myprofilepage.h"
#include "ui/myapplicationpage.h"
#include "ui/oncampusjobspage.h"
#include "ui/interviewwidget.h"
#include "ui/landingpage.h"
#include "ui/studentsurveydialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(Database *db, const QString &username, QWidget *parent = nullptr);

private slots:
    void switchToProfile();
    void switchToApplication();
    void switchToInterview();
    void switchToOnCampusJobs();
    void handleLogout();
    void showStudentSurvey();
    void handleSurveyCompleted();

private:
    void setupUI();
    void setupStyles();
    void loadUserData(const QString &username);
    void applyFadeTransition(QWidget *widget);
    void checkAndShowLandingPage();

    Database *database;
    User currentUser;
    QStackedWidget *stackedWidget;
    QToolBar *toolbar;
    MyProfilePage *profilePage;
    MyApplicationPage *applicationPage;
    OnCampusJobsPage *onCampusJobsPage;
    InterviewWidget *interviewWidget;
    LandingPage *landingPage;
};
