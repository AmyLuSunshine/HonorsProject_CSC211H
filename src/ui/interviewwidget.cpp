#include "interviewwidget.h"
#include <QMessageBox>

InterviewWidget::InterviewWidget(const QString& major, QWidget *parent)
    : QWidget(parent),
    studentMajor(major),
    currentQuestionIndex(0),
    score(0)
{
    setupQuestions();
    setupUI();
}

void InterviewWidget::setupQuestions()
{
    // Arts questions
    artsQuestions = {
        {"What is the capital of France?", "Paris", {"London", "Paris", "Berlin", "Madrid"}},
        {"Who painted the Mona Lisa?", "Leonardo da Vinci", {"Michelangelo", "Leonardo da Vinci", "Raphael", "Donatello"}},
        {"In what year did World War II end?", "1945", {"1943", "1944", "1945", "1946"}}
    };

    // Science questions
    scienceQuestions = {
        {"What is the chemical symbol for water?", "H2O", {"CO2", "H2O", "O2", "NaCl"}},
        {"What is the speed of light?", "299,792,458 m/s", {"150,000,000 m/s", "299,792,458 m/s", "300,000,000 m/s", "250,000,000 m/s"}},
        {"What is the powerhouse of the cell?", "Mitochondria", {"Nucleus", "Ribosome", "Mitochondria", "Chloroplast"}}
    };
}

void InterviewWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    questionLabel = new QLabel(this);
    questionLabel->setWordWrap(true);
    questionLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");
    mainLayout->addWidget(questionLabel);

    // Create answer buttons
    for (int i = 0; i < 4; ++i) {
        QPushButton* button = new QPushButton(this);
        button->setStyleSheet("QPushButton { padding: 10px; margin: 5px; text-align: left; }");
        connect(button, &QPushButton::clicked, this, &InterviewWidget::checkAnswer);
        answerButtons.append(button);
        mainLayout->addWidget(button);
    }

    feedbackLabel = new QLabel(this);
    feedbackLabel->setStyleSheet("font-size: 14px; padding: 10px;");
    feedbackLabel->setWordWrap(true);
    mainLayout->addWidget(feedbackLabel);


    
    nextButton = new QPushButton("Next Question", this);
    nextButton->setEnabled(false);
    connect(nextButton, &QPushButton::clicked, this, &InterviewWidget::nextQuestion);
    mainLayout->addWidget(nextButton);

    setLayout(mainLayout);

    // Load first question
    nextQuestion();
}

void InterviewWidget::checkAnswer()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    const Question* questions = getCurrentQuestions();
    if (currentQuestionIndex >= getCurrentQuestionsCount()) return;

    const Question& currentQuestion = questions[currentQuestionIndex];
    QString selectedAnswer = clickedButton->text();

    // Disable all answer buttons
    for (QPushButton* btn : answerButtons) {
        btn->setEnabled(false);
    }

    if (selectedAnswer == currentQuestion.correctAnswer) {
        showResult("Correct! ✓");
        score++;
    } else {
        showResult("Incorrect. The correct answer was: " + currentQuestion.correctAnswer);
    }

    nextButton->setEnabled(true);
}

void InterviewWidget::nextQuestion()
{
    const Question* questions = getCurrentQuestions();

    if (currentQuestionIndex >= getCurrentQuestionsCount()) {
        showFinalResult();
        return;
    }

    const Question& question = questions[currentQuestionIndex];

    questionLabel->setText(QString("Question %1: %2")
                               .arg(currentQuestionIndex + 1)
                               .arg(question.text));

    // Set button texts
    for (int i = 0; i < answerButtons.size() && i < question.options.size(); ++i) {
        answerButtons[i]->setText(question.options[i]);
        answerButtons[i]->setEnabled(true);
        answerButtons[i]->show();
    }

    // Hide extra buttons if needed
    for (int i = question.options.size(); i < answerButtons.size(); ++i) {
        answerButtons[i]->hide();
    }

    feedbackLabel->clear();
    nextButton->setEnabled(false);
    currentQuestionIndex++;
}

void InterviewWidget::showResult(const QString& result)
{
    feedbackLabel->setText(result);

    if (result.startsWith("Correct")) {
        feedbackLabel->setStyleSheet("color: green; font-weight: bold; padding: 10px;");
    } else {
        feedbackLabel->setStyleSheet("color: red; font-weight: bold; padding: 10px;");
    }

    // Auto-advance after 2 seconds
    QTimer::singleShot(2000, this, [this]() {
        if (currentQuestionIndex < getCurrentQuestionsCount()) {
            nextQuestion();
        } else {
            showFinalResult();
        }
    });
}

void InterviewWidget::showFinalResult()
{
    int totalQuestions = getCurrentQuestionsCount();
    double percentage = (static_cast<double>(score) / totalQuestions) * 100;

    QString resultMessage = QString("Interview Complete!\n\nYour Score: %1/%2 (%3%)")
                                .arg(score)
                                .arg(totalQuestions)
                                .arg(percentage, 0, 'f', 1);

    QMessageBox::information(this, "Interview Results", resultMessage);

    emit interviewComplete(score);
}

const Question* InterviewWidget::getCurrentQuestions() const
{
    if (studentMajor == "Arts") {
        return artsQuestions.constData();
    } else {
        return scienceQuestions.constData();
    }
}

int InterviewWidget::getCurrentQuestionsCount() const
{
    if (studentMajor == "Arts") {
        return artsQuestions.size();
    } else {
        return scienceQuestions.size();
    }
}
