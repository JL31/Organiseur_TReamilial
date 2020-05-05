#include "periodicsubtask.h"

// Cosntructor
PeriodicSubTask::PeriodicSubTask(int const& number,
                                 QDate const& initial_date,
                                 QDate const& delayed_date,
                                 bool const& is_processed) : m_number(number),
                                                             m_initial_date(initial_date),
                                                             m_delayed_date(delayed_date),
                                                             m_is_processed(is_processed)
{}

// Destructor
PeriodicSubTask::~PeriodicSubTask()
{}

// Getters
int PeriodicSubTask::get_number() const
{
    return m_number;
}

QDate PeriodicSubTask::get_initial_date() const
{
    return m_initial_date;
}

QDate PeriodicSubTask::get_delayed_date() const
{
    return m_delayed_date;
}

bool PeriodicSubTask::get_is_processed() const
{
    return m_is_processed;
}
