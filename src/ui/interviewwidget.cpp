#include "interviewwidget.h"
#include <QMessageBox>

// Constructor: Initialize the interview widget
InterviewWidget::InterviewWidget(const QString &degree, QWidget *parent)
    : QWidget(parent)
{
    // Save the student's degree from their profile
    studentDegree = degree;

    // Start with questions matching their degree
    // If degree contains "Art" or "Humanities" -> Arts questions
    // Otherwise -> Science questions
    if (degree.contains("Art", Qt::CaseInsensitive))
    {
        currentMode = "Arts";
    }
    else
    {
        currentMode = "Science";
    }

    currentQuestionIndex = 0; // Start at first question
    score = 0;                // No points yet

    setupQuestions(); // Create all the questions
    setupUI();        // Build the user interface
}

// Create all interview questions for both Arts and Science
void InterviewWidget::setupQuestions()
{
    // === ARTS QUESTIONS ===
    // Focus: Office software (Excel, Word) and customer service skills

    artsQuestions.append({"In Microsoft Excel, which function adds up a range of numbers?",
                          "SUM",
                          {"COUNT", "SUM", "AVERAGE", "TOTAL"}});

    artsQuestions.append({"In Microsoft Word, what does 'Ctrl + B' do?",
                          "Makes text bold",
                          {"Makes text italic", "Makes text bold", "Underlines text", "Saves the document"}});

    artsQuestions.append({"A customer is upset about a delayed order. What should you do first?",
                          "Listen and apologize",
                          {"Ignore them", "Listen and apologize", "Blame shipping", "Offer a discount immediately"}});

    // === SCIENCE QUESTIONS ===
    // Focus: Simple coding logic (LeetCode-style thinking)

    scienceQuestions.append({"What is the result of 5 % 2 in most programming languages?",
                             "1",
                             {"0", "1", "2", "2.5"}});

    scienceQuestions.append({"Which data structure uses LIFO (Last In, First Out)?",
                             "Stack",
                             {"Queue", "Stack", "Array", "Tree"}});

    scienceQuestions.append({"What does this code output? for(int i=0; i<3; i++) print(i);",
                             "0 1 2",
                             {"1 2 3", "0 1 2", "0 1 2 3", "1 2"}});
}

// Build the user interface
void InterviewWidget::setupUI()
{
    // STEP 1: Create main layout (vertical stack)
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // STEP 2: Create degree label (shows "Arts" or "Science")
    degreeLabel = new QLabel(this);
    degreeLabel->setText("Interview Questions: " + currentMode);
    degreeLabel->setAlignment(Qt::AlignCenter);
    // Simple styling: just bigger text and bold
    degreeLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(degreeLabel);

    // STEP 3: Create switch button
    switchButton = new QPushButton(this);
    // Set button text based on current mode
    if (currentMode == "Arts")
    {
        switchButton->setText("Switch to Science Questions");
    }
    else
    {
        switchButton->setText("Switch to Arts Questions");
    }
    // Simple button - no fancy styling
    switchButton->setMinimumHeight(40);
    connect(switchButton, &QPushButton::clicked, this, &InterviewWidget::switchQuestionType);
    mainLayout->addWidget(switchButton);

    mainLayout->addSpacing(10);

    // STEP 4: Create question label
    questionLabel = new QLabel(this);
    questionLabel->setWordWrap(true);
    // Simple styling: just bigger text
    questionLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(questionLabel);

    // STEP 5: Create 4 answer buttons
    for (int i = 0; i < 4; i++)
    {
        QPushButton *answerBtn = new QPushButton(this);
        // Simple button - just set minimum height
        answerBtn->setMinimumHeight(50);

        // Connect click to checkAnswer function
        connect(answerBtn, &QPushButton::clicked, this, &InterviewWidget::checkAnswer);

        answerButtons.append(answerBtn);
        mainLayout->addWidget(answerBtn);
    }

    // STEP 6: Create feedback label (shows correct/wrong)
    feedbackLabel = new QLabel(this);
    feedbackLabel->setWordWrap(true);
    // Simple styling: just set font size
    feedbackLabel->setStyleSheet("font-size: 14px;");
    mainLayout->addWidget(feedbackLabel);

    // STEP 7: Create next button
    nextButton = new QPushButton("Next Question", this);
    nextButton->setEnabled(false);
    // Simple button - just set height
    nextButton->setMinimumHeight(40);
    connect(nextButton, &QPushButton::clicked, this, &InterviewWidget::nextQuestion);
    mainLayout->addWidget(nextButton);

    // STEP 8: Load first question
    nextQuestion();
}

// Called when student clicks an answer button
void InterviewWidget::checkAnswer()
{
    // STEP 1: Find which button was clicked
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton)
        return;

    // STEP 2: Get the current question list
    QVector<Question> &questions = getCurrentQuestions();
    if (currentQuestionIndex > questions.size())
        return;

    // STEP 3: Get the question we just answered
    const Question &q = questions[currentQuestionIndex - 1];

    // STEP 4: Get student's selected answer
    QString studentAnswer = clickedButton->text();

    // STEP 5: Disable all answer buttons (prevent clicking twice)
    for (int i = 0; i < answerButtons.size(); i++)
    {
        answerButtons[i]->setEnabled(false);
    }

    // STEP 6: Check if answer is correct
    if (studentAnswer == q.correctAnswer)
    {
        // Correct answer!
        feedbackLabel->setText("✓ Correct!");
        feedbackLabel->setStyleSheet("color: green; font-weight: bold; font-size: 14px;");
        score = score + 1;
    }
    else
    {
        // Wrong answer
        feedbackLabel->setText("✗ Wrong. Correct answer: " + q.correctAnswer);
        feedbackLabel->setStyleSheet("color: red; font-weight: bold; font-size: 14px;");
    }

    // STEP 7: Enable next button
    nextButton->setEnabled(true);
}

// Load the next question (or show results if done)
void InterviewWidget::nextQuestion()
{
    // STEP 1: Get the question list
    QVector<Question> &questions = getCurrentQuestions();

    // STEP 2: Check if we finished all questions
    if (currentQuestionIndex >= questions.size())
    {
        showFinalResult();
        return;
    }

    // STEP 3: Get the next question
    const Question &q = questions[currentQuestionIndex];

    // STEP 4: Show question text
    int questionNumber = currentQuestionIndex + 1;
    int totalQuestions = questions.size();
    questionLabel->setText("Question " + QString::number(questionNumber) + " of " + QString::number(totalQuestions) + ": " + q.text);

    // STEP 5: Fill in answer buttons
    for (int i = 0; i < 4; i++)
    {
        if (i < q.options.size())
        {
            answerButtons[i]->setText(q.options[i]);
            answerButtons[i]->setEnabled(true);
            answerButtons[i]->show();
        }
        else
        {
            answerButtons[i]->hide();
        }
    }

    // STEP 6: Clear old feedback
    feedbackLabel->clear();

    // STEP 7: Disable next button
    nextButton->setEnabled(false);

    // STEP 8: Move to next question
    currentQuestionIndex++;
}

// Switch between Arts and Science questions
void InterviewWidget::switchQuestionType()
{
    // STEP 1: Switch the mode
    if (currentMode == "Arts")
    {
        currentMode = "Science";
        switchButton->setText("Switch to Arts Questions");
    }
    else
    {
        currentMode = "Arts";
        switchButton->setText("Switch to Science Questions");
    }

    // STEP 2: Update the top label
    degreeLabel->setText("Interview Questions: " + currentMode);

    // STEP 3: Reset everything
    currentQuestionIndex = 0;
    score = 0;
    feedbackLabel->clear();

    // STEP 4: Load first question
    nextQuestion();
}

// Show final score when all questions are answered
void InterviewWidget::showFinalResult()
{
    // STEP 1: Get total number of questions
    QVector<Question> &questions = getCurrentQuestions();
    int total = questions.size();

    // STEP 2: Calculate percentage
    double percent = (score * 100.0) / total;

    // STEP 3: Build message
    QString message = "Interview Complete!\n\n";
    message += "Your Score: " + QString::number(score) + " / " + QString::number(total);
    message += " (" + QString::number(percent, 'f', 1) + "%)";

    // STEP 4: Show popup
    QMessageBox::information(this, "Interview Results", message);

    // STEP 5: Tell main window we're done
    emit interviewComplete(score);
}

// Helper function: Get the current question list based on mode
QVector<Question> &InterviewWidget::getCurrentQuestions()
{
    if (currentMode == "Arts")
    {
        return artsQuestions;
    }
    else
    {
        return scienceQuestions;
    }
}
