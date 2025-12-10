#include "interviewwidget.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QStackedWidget>

// Constructor: Initialize the interview widget
InterviewWidget::InterviewWidget(const QString &degree, QWidget *parent)
    : QWidget(parent)
{
    // Initialize data
    currentMode = "";         // Not set yet - user will choose
    currentQuestionIndex = 0; // Start at first question
    score = 0;                // No points yet

    setupQuestions(); // Create all the questions

    // Create main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create stacked widget to switch between landing and questions
    QStackedWidget *stackedWidget = new QStackedWidget(this);

    // Create landing screen
    setupLandingUI();
    stackedWidget->addWidget(landingWidget);

    // Create question screen
    setupQuestionUI();
    stackedWidget->addWidget(questionWidget);

    // Add to main layout
    mainLayout->addWidget(stackedWidget);

    // Start with landing screen (index 0)
    stackedWidget->setCurrentIndex(0);

    // Save pointer for later use
    this->findChild<QStackedWidget *>(); // Will use sender's parent to switch screens
}

// Create all interview questions for both Behavior and Technical
void InterviewWidget::setupQuestions()
{
    // === BEHAVIOR QUESTIONS ===
    // Focus: Interpersonal, conflict, prioritization, time management, goal-setting

    behaviorQuestions.append({"In Microsoft Excel, which function adds up a range of numbers?",
                              "SUM",
                              {"COUNT", "SUM", "AVERAGE", "TOTAL"}});

    behaviorQuestions.append({"In Microsoft Word, what does 'Ctrl + B' do?",
                              "Makes text bold",
                              {"Makes text italic", "Makes text bold", "Underlines text", "Saves the document"}});

    behaviorQuestions.append({"A customer is upset about a delayed order. What should you do first?",
                              "Listen and apologize",
                              {"Ignore them", "Listen and apologize", "Blame shipping", "Offer a discount immediately"}});

    // === TECHNICAL QUESTIONS ===
    // Focus: Simple coding logic (LeetCode-style thinking)

    technicalQuestions.append({"What is the result of 5 % 2 in most programming languages?",
                               "1",
                               {"0", "1", "2", "2.5"}});

    technicalQuestions.append({"Which data structure uses LIFO (Last In, First Out)?",
                               "Stack",
                               {"Queue", "Stack", "Array", "Tree"}});

    technicalQuestions.append({"What does this code output? for(int i=0; i<3; i++) print(i);",
                               "0 1 2",
                               {"1 2 3", "0 1 2", "0 1 2 3", "1 2"}});
}

// === LANDING SCREEN SETUP ===
// This shows 2 big blocks: Behavior Questions and Technical Questions
void InterviewWidget::setupLandingUI()
{
    landingWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(landingWidget);
    layout->setSpacing(30);
    layout->setContentsMargins(40, 40, 40, 40);

    // STEP 1: Add title
    QLabel *titleLabel = new QLabel("Interview Preparation");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #000000;");
    layout->addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel("Choose a question type to get started");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #666666;");
    layout->addWidget(subtitleLabel);

    layout->addSpacing(20);

    // STEP 2: Create two blocks in a horizontal layout
    QHBoxLayout *blocksLayout = new QHBoxLayout();
    blocksLayout->setSpacing(30);

    // LEFT BLOCK: Behavior Questions - using labels for proper styling
    QWidget *behaviorWidget = new QWidget();
    QVBoxLayout *behaviorVLayout = new QVBoxLayout(behaviorWidget);
    behaviorVLayout->setContentsMargins(12, 12, 12, 12);
    behaviorVLayout->setSpacing(0);
    behaviorVLayout->addStretch();

    QLabel *behaviorTitle = new QLabel("Behavior Questions");
    behaviorTitle->setAlignment(Qt::AlignCenter);
    behaviorTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #000000;");
    behaviorVLayout->addWidget(behaviorTitle);

    behaviorVLayout->addSpacing(6);

    QLabel *behaviorDesc = new QLabel("Learn about workplace\nconflict and teamwork");
    behaviorDesc->setAlignment(Qt::AlignCenter);
    behaviorDesc->setStyleSheet("font-size: 11px; color: #666666; line-height: 1.3;");
    behaviorVLayout->addWidget(behaviorDesc);

    behaviorVLayout->addStretch();

    behaviorWidget->setMinimumSize(200, 150);
    behaviorWidget->setStyleSheet(
        "QWidget {"
        "  background-color: #f5f5f5;"
        "}");

    QPushButton *behaviorBlock = new QPushButton();
    behaviorBlock->setLayout(behaviorVLayout);
    behaviorBlock->setMinimumHeight(150);
    behaviorBlock->setMinimumWidth(200);
    behaviorBlock->setStyleSheet(
        "QPushButton {"
        "  background-color: #f5f5f5;"
        "  border: 2px solid #999999;"
        "  border-radius: 8px;"
        "  padding: 0px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #eeeeee;"
        "}");
    connect(behaviorBlock, &QPushButton::clicked, this, &InterviewWidget::startBehaviorQuestions);
    blocksLayout->addWidget(behaviorBlock);

    // RIGHT BLOCK: Technical Questions - using labels for proper styling
    QWidget *technicalWidget = new QWidget();
    QVBoxLayout *technicalVLayout = new QVBoxLayout(technicalWidget);
    technicalVLayout->setContentsMargins(12, 12, 12, 12);
    technicalVLayout->setSpacing(0);
    technicalVLayout->addStretch();

    QLabel *technicalTitle = new QLabel("Technical Questions");
    technicalTitle->setAlignment(Qt::AlignCenter);
    technicalTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #000000;");
    technicalVLayout->addWidget(technicalTitle);

    technicalVLayout->addSpacing(6);

    QLabel *technicalDesc = new QLabel("Learn about coding\nconcepts and logic");
    technicalDesc->setAlignment(Qt::AlignCenter);
    technicalDesc->setStyleSheet("font-size: 11px; color: #666666; line-height: 1.3;");
    technicalVLayout->addWidget(technicalDesc);

    technicalVLayout->addStretch();

    technicalWidget->setMinimumSize(200, 150);
    technicalWidget->setStyleSheet(
        "QWidget {"
        "  background-color: #ffffff;"
        "}");

    QPushButton *technicalBlock = new QPushButton();
    technicalBlock->setLayout(technicalVLayout);
    technicalBlock->setMinimumHeight(150);
    technicalBlock->setMinimumWidth(200);
    technicalBlock->setStyleSheet(
        "QPushButton {"
        "  background-color: #ffffff;"
        "  border: 2px solid #999999;"
        "  border-radius: 8px;"
        "  padding: 0px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f9f9f9;"
        "}");
    connect(technicalBlock, &QPushButton::clicked, this, &InterviewWidget::startTechnicalQuestions);
    blocksLayout->addWidget(technicalBlock);

    layout->addLayout(blocksLayout);
    layout->addStretch();
}

// === QUESTION SCREEN SETUP ===
// This shows one question at a time with answer options
void InterviewWidget::setupQuestionUI()
{
    questionWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(questionWidget);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);

    // STEP 1: Add title showing which question type
    QLabel *titleLabel = new QLabel("Interview Question");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #000000;");
    layout->addWidget(titleLabel);

    layout->addSpacing(10);

    // STEP 2: Add the question text
    questionLabel = new QLabel();
    questionLabel->setWordWrap(true);
    questionLabel->setStyleSheet("font-size: 16px; color: #333333; line-height: 1.6;");
    layout->addWidget(questionLabel);

    layout->addSpacing(20);

    // STEP 3: Create 4 answer buttons
    for (int i = 0; i < 4; i++)
    {
        QPushButton *answerBtn = new QPushButton();
        answerBtn->setMinimumHeight(50);
        answerBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #ffffff;"
            "  color: #000000;"
            "  border: 2px solid #cccccc;"
            "  border-radius: 5px;"
            "  font-size: 14px;"
            "  padding: 10px;"
            "  text-align: left;"
            "}"
            "QPushButton:hover {"
            "  background-color: #f0f0f0;"
            "  border: 2px solid #999999;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #e0e0e0;"
            "}");
        connect(answerBtn, &QPushButton::clicked, this, &InterviewWidget::checkAnswer);
        answerButtons.append(answerBtn);
        layout->addWidget(answerBtn);
    }

    layout->addSpacing(15);

    // STEP 4: Add feedback label
    feedbackLabel = new QLabel();
    feedbackLabel->setWordWrap(true);
    feedbackLabel->setAlignment(Qt::AlignCenter);
    feedbackLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #000000;");
    layout->addWidget(feedbackLabel);

    layout->addSpacing(15);

    // STEP 5: Create button layout for Next and Back buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    backButton = new QPushButton("Back to Menu");
    backButton->setMinimumHeight(40);
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #e0e0e0;"
        "  color: #000000;"
        "  border: 1px solid #999999;"
        "  border-radius: 5px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #d0d0d0;"
        "}");
    connect(backButton, &QPushButton::clicked, this, &InterviewWidget::backToLanding);
    buttonLayout->addWidget(backButton);

    nextButton = new QPushButton("Next Question");
    nextButton->setMinimumHeight(40);
    nextButton->setEnabled(false);
    nextButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #ffffff;"
        "  color: #000000;"
        "  border: 2px solid #cccccc;"
        "  border-radius: 5px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover:!disabled {"
        "  background-color: #f0f0f0;"
        "  border: 2px solid #999999;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #f5f5f5;"
        "  color: #999999;"
        "}");
    connect(nextButton, &QPushButton::clicked, this, &InterviewWidget::nextQuestion);
    buttonLayout->addWidget(nextButton);

    layout->addLayout(buttonLayout);
    layout->addStretch();
}

// === SLOT: Start Behavior Questions ===
void InterviewWidget::startBehaviorQuestions()
{
    currentMode = "Behavior Questions";
    currentQuestionIndex = 0;
    score = 0;

    // Switch to question screen
    QStackedWidget *stacked = findChild<QStackedWidget *>();
    if (stacked)
        stacked->setCurrentIndex(1);

    // Load first question
    nextQuestion();
}

// === SLOT: Start Technical Questions ===
void InterviewWidget::startTechnicalQuestions()
{
    currentMode = "Technical Questions";
    currentQuestionIndex = 0;
    score = 0;

    // Switch to question screen
    QStackedWidget *stacked = findChild<QStackedWidget *>();
    if (stacked)
        stacked->setCurrentIndex(1);

    // Load first question
    nextQuestion();
}

// === SLOT: Back to Landing Screen ===
void InterviewWidget::backToLanding()
{
    // Switch back to landing screen
    QStackedWidget *stacked = findChild<QStackedWidget *>();
    if (stacked)
        stacked->setCurrentIndex(0);
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
    if (currentMode == "Behavior Questions")
    {
        return behaviorQuestions;
    }
    else
    {
        return technicalQuestions;
    }
}
