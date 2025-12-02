#include "ui/joblistwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QGridLayout>
#include <algorithm>

JobListWidget::JobListWidget(Database *db, QWidget *parent)
    : QWidget(parent), database(db)
{
    setupUI();
    setupStyles();
    loadJobs();
}

void JobListWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Header
    auto headerLayout = new QHBoxLayout();
    titleLabel = new QLabel("Available Campus Jobs", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2196F3;");

    refreshButton = new QPushButton("Refresh", this);
    refreshButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 8px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #1976D2; }");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(refreshButton);
    mainLayout->addLayout(headerLayout);

    // Job grid
    auto gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);
    jobList = new QListWidget(this);
    jobList->setViewMode(QListWidget::IconMode);
    jobList->setIconSize(QSize(300, 180));
    jobList->setSpacing(20);
    jobList->setResizeMode(QListWidget::Adjust);
    jobList->setMovement(QListWidget::Static);
    mainLayout->addWidget(jobList);

    // Connect signals
    connect(refreshButton, &QPushButton::clicked, this, &JobListWidget::refreshJobs);
    connect(jobList, &QListWidget::itemClicked, this, &JobListWidget::handleJobClicked);

    loadJobs();
}

void JobListWidget::refreshJobs()
{
    loadJobs();
}

void JobListWidget::loadJobs()
{
    jobList->clear();
    currentJobs = database->getJobs();

    for (const auto &job : currentJobs)
    {
        auto item = new QListWidgetItem(jobList);
        auto card = createJobCard(job);
        item->setSizeHint(card->sizeHint());
        jobList->setItemWidget(item, card);
        item->setData(Qt::UserRole, job.getId());
    }
}

QWidget *JobListWidget::createJobCard(const Job &job)
{
    auto card = new QWidget;
    card->setFixedSize(300, 180);

    auto layout = new QVBoxLayout(card);
    layout->setSpacing(8);
    layout->setContentsMargins(15, 15, 15, 15);

    // Title
    auto titleLabel = new QLabel(job.getTitle(), card);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1976D2;");

    // Department
    auto deptLabel = new QLabel(job.getDepartment(), card);
    deptLabel->setStyleSheet("color: #666666;");

    // Pay rate
    auto payLabel = new QLabel(job.getPayRateString(), card);
    payLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");

    // Description preview
    auto descLabel = new QLabel(job.getDescription(), card);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("color: #333333;");

    // View Details button
    auto detailsButton = new QPushButton("View Details", card);
    detailsButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 8px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #1976D2; }");

    layout->addWidget(titleLabel);
    layout->addWidget(deptLabel);
    layout->addWidget(payLabel);
    layout->addWidget(descLabel);
    layout->addWidget(detailsButton);

    card->setStyleSheet(
        "QWidget { background-color: white; border: 1px solid #E0E0E0; border-radius: 8px; }");

    return card;
}

void JobListWidget::handleJobClicked(QListWidgetItem *item)
{
    int jobId = item->data(Qt::UserRole).toInt();
    onJobSelected(jobId);
}

void JobListWidget::onJobSelected(int jobId)
{
    auto it = std::find_if(currentJobs.begin(), currentJobs.end(),
                           [jobId](const Job &job)
                           { return job.getId() == jobId; });

    if (it != currentJobs.end())
    {
        QString emailTemplate =
            QString("Subject: Application for %1 Position\n\n"
                    "Dear Hiring Manager,\n\n"
                    "I am writing to express my interest in the %1 position at BMCC.\n\n"
                    "Best regards")
                .arg(it->getTitle());

        QApplication::clipboard()->setText(emailTemplate);

        QMessageBox::information(
            this,
            "Job Details",
            QString("%1\n\nDepartment: %2\nPay: %3\n\nDescription:\n%4\n\n"
                    "Requirements:\n%5\n\n"
                    "Email template has been copied to clipboard.")
                .arg(it->getTitle())
                .arg(it->getDepartment())
                .arg(it->getPayRateString())
                .arg(it->getDescription())
                .arg(it->getRequirements()));
    }
}

void JobListWidget::setupStyles()
{
    setStyleSheet(
        "QWidget { background-color: #FFFFFF; }"
        "QLabel { color: #333333; }"
        "QListWidget { background: transparent; border: none; }"
        "QListWidget::item { background: white; border-radius: 8px; margin: 8px; }"
        "QListWidget::item:hover { background: #E3F2FD; }"
        "QPushButton { padding: 8px 16px; background-color: #2196F3; color: white; "
        "             border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #1976D2; }");
}
