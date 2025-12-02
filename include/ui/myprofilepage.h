#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QGroupBox>
#include "database/database.h"

class MyProfilePage : public QWidget
{
    Q_OBJECT
public:
    explicit MyProfilePage(Database *db, QWidget *parent = nullptr);
    void setUserId(int userId);

signals:
    void profileSaved();

private slots:
    void handleSaveProfile();
    void handleChangePassword();

private:
    void setupUI();
    void setupStyles();
    void loadUser();

    Database *database;
    int currentUserId;

    // Personal info widgets
    QLabel *nameLabel;
    QLineEdit *fullNameEdit;
    QLineEdit *emplidEdit;
    QLineEdit *emailEdit;
    QLineEdit *addressEdit;
    QLineEdit *majorEdit;
    QLineEdit *gpaEdit;
    QDateEdit *gradDateEdit;

    // Account info moved to SettingsPage
    QPushButton *saveButton;
};
