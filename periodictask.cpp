#include "periodictask.h"

using namespace std;

// Constructor
PeriodicTask::PeriodicTask(int const& number,
                           QString const& name,
                           bool const& is_important,
                           QString const& comments,
                           bool const& is_processed,
                           bool const& is_dated,
                           QDate const& date,
                           bool const& reminder,
                           int const& weeks_before_task,
                           bool const& is_periodic,
                           int const& periodicity) : NormalTask(number,name, is_important, comments, is_processed, is_dated, date, reminder, weeks_before_task),
                                                     m_is_periodic(new bool(is_periodic)),
                                                     m_periodicity(new int(periodicity)),
                                                     m_periodic_sub_tasks(new vector<PeriodicSubTask*>())
{}

// Destructor
PeriodicTask::~PeriodicTask()
{
    delete m_is_periodic;
    delete m_periodicity;
    delete m_periodic_sub_tasks;

    m_is_periodic = nullptr;
    m_periodicity = nullptr;
    m_periodic_sub_tasks = nullptr;
}

// Getters
bool PeriodicTask::get_is_periodic() const
{
    return *m_is_periodic;
}

int PeriodicTask::get_periodicity() const
{
    return *m_periodicity;
}

// Method to add a sub task to the periodic task map
void PeriodicTask::add_sub_task(int const& number, QDate const& initial_date, QDate const& delayed_date, bool const& is_processed)
{
    m_periodic_sub_tasks->push_back(new PeriodicSubTask(number, initial_date, delayed_date, is_processed));
}
