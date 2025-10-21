#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include "database/database.h"
#include "models/user.h"
#include "ui/joblistwidget.h"
#include "ui/profilewidget.h"
#include "ui/interviewwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(Database* db, const QString& username, QWidget *parent = nullptr);

private slots:
    void switchToJobs();
    void switchToProfile();
    void switchToInterview();
    void handleLogout();

private:
    void setupUI();
    void setupStyles();
    void loadUserData(const QString& username);

    Database* database;
    User currentUser;
    QStackedWidget* stackedWidget;
    QToolBar* toolbar;
    JobListWidget* jobListWidget;
    ProfileWidget* profileWidget;
    InterviewWidget* interviewWidget;
};
