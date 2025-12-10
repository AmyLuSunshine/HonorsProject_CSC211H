#ifndef INTERVIEWWIDGET_H
#define INTERVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QString>

// Simple structure to hold one interview question
struct Question
{
    QString text;             // The question text
    QString correctAnswer;    // The correct answer
    QVector<QString> options; // All answer choices (including correct one)
};

class InterviewWidget : public QWidget
{
    Q_OBJECT

public:
    // Constructor: takes the student's degree ("Arts" or "Science")
    explicit InterviewWidget(const QString &degree, QWidget *parent = nullptr);

signals:
    void interviewComplete(int finalScore); // Emitted when interview ends

private slots:
    void checkAnswer();             // Called when student clicks an answer
    void nextQuestion();            // Loads the next question
    void startBehaviorQuestions();  // Start behavior questions quiz
    void startTechnicalQuestions(); // Start technical questions quiz
    void backToLanding();           // Go back to landing screen

private:
    // Setup functions
    void setupQuestions();  // Creates all the questions
    void setupLandingUI();  // Creates the landing screen with 2 blocks
    void setupQuestionUI(); // Creates the question screen
    void showFinalResult(); // Shows score at the end

    // Helper to get current question list based on mode
    QVector<Question> &getCurrentQuestions();

    // Data
    QString currentMode;      // "Behavior Questions" or "Technical Questions"
    int currentQuestionIndex; // Which question we're on (0, 1, 2...)
    int score;                // How many questions answered correctly

    // Question banks
    QVector<Question> behaviorQuestions;  // Behavior questions
    QVector<Question> technicalQuestions; // Technical questions

    // UI elements
    QWidget *landingWidget;  // Landing screen container
    QWidget *questionWidget; // Question screen container

    QLabel *questionLabel;                // Shows the question text
    QLabel *feedbackLabel;                // Shows "Correct!" or "Wrong"
    QVector<QPushButton *> answerButtons; // The 4 answer buttons
    QPushButton *nextButton;              // "Next Question" button
    QPushButton *backButton;              // "Back to Landing" button

    QVBoxLayout *mainLayout; // Main vertical layout
};

#endif // INTERVIEWWIDGET_H
