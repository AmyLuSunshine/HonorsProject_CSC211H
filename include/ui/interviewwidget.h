#ifndef INTERVIEWWIDGET_H
#define INTERVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <QString>
#include <QTimer>

// Question structure
struct Question {
    QString text;
    QString correctAnswer;
    QVector<QString> options;
};

class InterviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InterviewWidget(const QString& major, QWidget *parent = nullptr);

signals:
    void interviewComplete(int finalScore);
    void backToMenu();

private slots:
    void checkAnswer();
    void nextQuestion();
    void showResult(const QString& result);

private:
    void setupQuestions();
    void setupUI();
    void setupStyles();
    const Question* getCurrentQuestions() const;
    int getCurrentQuestionsCount() const;
    void showFinalResult();

    // Member variables
    QString studentMajor;
    int currentQuestionIndex;
    int score;

    // Questions
    QVector<Question> artsQuestions;
    QVector<Question> scienceQuestions;

    // UI elements
    QLabel* questionLabel;
    QLabel* feedbackLabel;
    QVector<QPushButton*> answerButtons;
    QPushButton* nextButton;

    QVBoxLayout* mainLayout;
};

#endif // INTERVIEWWIDGET_H
