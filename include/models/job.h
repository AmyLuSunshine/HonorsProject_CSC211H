#pragma once
#include <QString>
#include <QDateTime>

enum class JobStatus
{
    Open,
    ClosingSoon,
    Closed,
    Upcoming
};

enum class JobCategory
{
    AcademicSupport,
    ResidentialLife,
    Administrative,
    Technology,
    LibraryServices,
    FoodServices,
    RecreationFitness,
    StudentServices
};

class Job
{
public:
    Job() : id(-1), payRateMin(0), payRateMax(0), hoursPerWeek(0),
            positionsAvailable(0), workStudyEligible(false), internationalEligible(false), status(JobStatus::Closed) {}

    Job(int id, const QString &title, const QString &department, const QString &category,
        const QString &description, double payRateMin, double payRateMax,
        int hoursPerWeek, int positionsAvailable, JobStatus status,
        bool workStudyEligible, bool internationalEligible, const QString &deadline, const QString &requirements = "",
        const QString &duties = "", const QString &skills = "", const QString &supervisorInfo = "",
        const QString &contactEmail = "", const QString &schedule = "")
        : id(id), title(title), department(department), category(category),
          description(description), payRateMin(payRateMin), payRateMax(payRateMax),
          hoursPerWeek(hoursPerWeek), positionsAvailable(positionsAvailable),
          status(status), workStudyEligible(workStudyEligible), internationalEligible(internationalEligible), deadline(deadline),
          requirements(requirements), duties(duties), skills(skills),
          supervisorInfo(supervisorInfo), contactEmail(contactEmail), schedule(schedule) {}

    // Getters
    int getId() const { return id; }
    QString getTitle() const { return title; }
    QString getDepartment() const { return department; }
    QString getCategory() const { return category; }
    QString getDescription() const { return description; }
    double getPayRateMin() const { return payRateMin; }
    double getPayRateMax() const { return payRateMax; }
    int getHoursPerWeek() const { return hoursPerWeek; }
    int getPositionsAvailable() const { return positionsAvailable; }
    JobStatus getStatus() const { return status; }
    bool isWorkStudyEligible() const { return workStudyEligible; }
    bool isInternationalEligible() const { return internationalEligible; }
    QString getDeadline() const { return deadline; }
    QString getRequirements() const { return requirements; }
    QString getDuties() const { return duties; }
    QString getSkills() const { return skills; }
    QString getSupervisorInfo() const { return supervisorInfo; }
    QString getContactEmail() const { return contactEmail; }
    QString getSchedule() const { return schedule; }

    // Helper methods
    QString getStatusString() const
    {
        switch (status)
        {
        case JobStatus::Open:
            return "OPEN";
        case JobStatus::ClosingSoon:
            return "CLOSING SOON";
        case JobStatus::Closed:
            return "CLOSED";
        case JobStatus::Upcoming:
            return "UPCOMING";
        default:
            return "UNKNOWN";
        }
    }

    QString getPayRateString() const
    {
        if (payRateMin == payRateMax)
        {
            return QString("$%1/hour").arg(payRateMin, 0, 'f', 2);
        }
        return QString("$%1-$%2/hour").arg(payRateMin, 0, 'f', 2).arg(payRateMax, 0, 'f', 2);
    }

private:
    int id;
    QString title;
    QString department;
    QString category;
    QString description;
    double payRateMin;
    double payRateMax;
    int hoursPerWeek;
    int positionsAvailable;
    JobStatus status;
    bool workStudyEligible;
    bool internationalEligible;
    QString deadline;
    QString requirements;
    QString duties;
    QString skills;
    QString supervisorInfo;
    QString contactEmail;
    QString schedule;
};
