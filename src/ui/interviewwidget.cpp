#include "ui/interviewwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMessageBox>

InterviewWidget::InterviewWidget(const QString& major, QWidget* parent)
    : QWidget(parent), studentMajor(major), currentQuestionIndex(0)
{
    setupQuestions();
    setupUI();
    showQuestion(0);
}

void InterviewWidget::setupQuestions()
{
    // Sample questions for Arts
    artsQuestions = {
        {"Who painted the Mona Lisa?", {"Leonardo da Vinci", "Vincent van Gogh", "Pablo Picasso", "Claude Monet"}, 0},
        {"What is the main theme of Shakespeare's Hamlet?", {"Revenge", "Love", "War", "Friendship"}, 0},
        {"Which art movement is Salvador Dalí associated with?", {"Surrealism", "Impressionism", "Cubism", "Baroque"}, 0}
    };

    // Sample questions for Science
    scienceQuestions = {
        {"What is the chemical symbol for water?", {"H2O", "CO2", "O2", "NaCl"}, 0},
        {"Who developed the theory of relativity?", {"Isaac Newton", "Albert Einstein", "Marie Curie", "Nikola Tesla"}, 1},
        {"What planet is known as the Red Planet?", {"Venus", "Mars", "Jupiter", "Saturn"}, 1}
    };
}

void InterviewWidget::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    questionLabel = new QLabel(this);
    layout->addWidget(questionLabel);

    optionsGroup = new QButtonGroup(this);
    for (int i = 0; i < 4; ++i) {
        QRadioButton* option = new QRadioButton(this);
        optionsGroup->addButton(option, i);
        layout->addWidget(option);
        optionButtons.append(option);
    }

    submitButton = new QPushButton("Submit Answer", this);
    layout->addWidget(submitButton);

    connect(submitButton, &QPushButton::clicked, this, &InterviewWidget::handleSubmit);

    resultLabel = new QLabel(this);
    layout->addWidget(resultLabel);

    setLayout(layout);
}

void InterviewWidget::showQuestion(int index)
{
    const Question* q = getCurrentQuestions();
    if (!q) return;

    int totalQuestions = getCurrentQuestionsCount();
    if (index < 0 || index >= totalQuestions) return;

    const Question& question = q[index];
    questionLabel->setText(QString("Q%1: %2").arg(index + 1).arg(question.text));
    for (int i = 0; i < optionButtons.size(); ++i) {
        optionButtons[i]->setText(question.options[i]);
        optionButtons[i]->setChecked(false);
    }
    resultLabel->clear();
}

void InterviewWidget::handleSubmit()
{
    const Question* q = getCurrentQuestions();
    if (!q) return;

    int totalQuestions = getCurrentQuestionsCount();
    if (currentQuestionIndex < 0 || currentQuestionIndex >= totalQuestions) return;

    int selected = optionsGroup->checkedId();
    if (selected == -1) {
        QMessageBox::warning(this, "No Selection", "Please select an answer.");
        return;
    }

    const Question& question = q[currentQuestionIndex];
    if (selected == question.correctOption) {
        resultLabel->setText("Correct!");
        ++score;
    } else {
        resultLabel->setText(QString("Incorrect! Correct answer: %1").arg(question.options[question.correctOption]));
    }

    submitButton->setEnabled(false);

    // Show next question after a short delay
    QTimer::singleShot(1200, this, [this]() {
        submitButton->setEnabled(true);
        ++currentQuestionIndex;
        if (currentQuestionIndex < getCurrentQuestionsCount()) {
            showQuestion(currentQuestionIndex);
        } else {
            showFinalResult();
        }
    });
}

void InterviewWidget::showFinalResult()
{
    questionLabel->setText("Interview Complete!");
    for (auto* btn : optionButtons) {
        btn->hide();
    }
    submitButton->hide();
    resultLabel->setText(QString("Your score: %1/%2").arg(score).arg(getCurrentQuestionsCount()));
}

const InterviewWidget::Question* InterviewWidget::getCurrentQuestions() const
{
    if (studentMajor.compare("Arts", Qt::CaseInsensitive) == 0)
        return artsQuestions.data();
    else
        return scienceQuestions.data();
}

int InterviewWidget::getCurrentQuestionsCount() const
{
    if (studentMajor.compare("Arts", Qt::CaseInsensitive) == 0)
        return artsQuestions.size();
    else
        return scienceQuestions.size();
}
