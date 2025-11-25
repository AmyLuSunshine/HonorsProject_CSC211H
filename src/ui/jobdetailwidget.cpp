#include "ui/jobdetailwidget.h"
#include <QVBoxLayout>

JobDetailWidget::JobDetailWidget(Database *db, QWidget *parent)
    : QWidget(parent), database(db)
{
    setupUI();
    setupStyles();
}

void JobDetailWidget::setupUI()
{
    auto layout = new QVBoxLayout(this);

    titleLabel = new QLabel(this);
    departmentLabel = new QLabel(this);
    descriptionLabel = new QLabel(this);
    payLabel = new QLabel(this);
    applyButton = new QPushButton("Apply", this);

    layout->addWidget(titleLabel);
    layout->addWidget(departmentLabel);
    layout->addWidget(descriptionLabel);
    layout->addWidget(payLabel);
    layout->addWidget(applyButton);
}

void JobDetailWidget::setupStyles()
{
    setStyleSheet(
        "QLabel { font-size: 16px; margin: 5px; }"
        "QPushButton { padding: 10px; background-color: #2196F3; "
        "color: white; border: none; border-radius: 5px; }");
}

void JobDetailWidget::setJob(const Job &job)
{
    titleLabel->setText(job.getTitle());
    departmentLabel->setText(job.getDepartment());
    descriptionLabel->setText(job.getDescription());
    payLabel->setText(job.getPayRateString());
}
