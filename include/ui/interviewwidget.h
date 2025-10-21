
#pragma once
#include <QWidget>
#include <QVector>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QGroupBox>
#include "database/database.h"
#include "models/user.h"

struct InterviewQuestion {
    QString question;
    QVector<QString> options;
    int correctIndex;
};

class InterviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit InterviewWidget(Database* db, User* user, QWidget* parent = nullptr);

private slots:
    void handleNext();
    void handleSubmit();

private:
    void loadQuestions();
    void showQuestion(int index);
    void showResult();

    Database* database;
    User* currentUser;

    QVector<InterviewQuestion> questions;
    QVector<int> userAnswers;
    int currentQuestionIndex;

    QLabel* questionLabel;
    QButtonGroup* optionsGroup;
    QVector<QRadioButton*> optionButtons;
    QPushButton* nextButton;
    QPushButton* submitButton;
    QLabel* resultLabel;
    QVBoxLayout* mainLayout;
};
