#include "periodictask.h"

using namespace std;

// Constructor
PeriodicTask::PeriodicTask(int const& number,
                           QString const& name,
                           QString const& comments,
                           bool const& is_important,
                           bool const& is_processed,
                           bool const& is_dated,
                           QDate const& date,
                           bool const& reminder,
                           int const& weeks_before_task,
                           bool const& is_periodic,
                           int const& periodicity) : NormalTask(number,name, comments, is_important, is_processed, is_dated, date, reminder, weeks_before_task),
                                                     m_is_periodic(is_periodic),
                                                     m_periodicity(periodicity),
                                                     m_periodic_sub_tasks(vector<PeriodicSubTask*>())
{}

// Destructor
PeriodicTask::~PeriodicTask()
{}

// Getters
bool PeriodicTask::get_is_periodic() const
{
    return m_is_periodic;
}

int PeriodicTask::get_periodicity() const
{
    return m_periodicity;
}

// Method to add a sub task to the periodic task map
void PeriodicTask::add_sub_task(int const& number, QDate const& initial_date, QDate const& delayed_date, bool const& is_processed)
{
    m_periodic_sub_tasks.push_back(new PeriodicSubTask(number, initial_date, delayed_date, is_processed));
}
