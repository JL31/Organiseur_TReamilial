#include "periodictask.h"
#include <QString>
#include <QDate>

// Constructor
PeriodicTask::PeriodicTask(int const& task_number, QString const& task_name, QString const& task_importance, QDate const& task_date, bool const& reminder, int const& nbr_weeks_before_task, QString const& comments, int const& periodicity) :
                           Task(task_number, task_name, task_importance, task_date, reminder, nbr_weeks_before_task, comments),
                           m_periodicity(new int(periodicity))
{}

// Destructor
PeriodicTask::~PeriodicTask()
{
    delete m_periodicity;
    m_periodicity = nullptr;
}
