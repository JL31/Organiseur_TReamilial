#include "periodicsubtask.h"

// Cosntructor
PeriodicSubTask::PeriodicSubTask(int const& number,
                                 QDate const& initial_date,
                                 QDate const& delayed_date,
                                 bool const& is_processed) : m_number(new int(number)),
                                                             m_initial_date(new QDate(initial_date)),
                                                             m_delayed_date(new QDate(delayed_date)),
                                                             m_is_processed(new bool(is_processed))
{}

// Destructor
PeriodicSubTask::~PeriodicSubTask()
{
    delete m_number;
    delete m_initial_date;
    delete m_delayed_date;
    delete m_is_processed;

    m_number = nullptr;
    m_initial_date = nullptr;
    m_delayed_date = nullptr;
    m_is_processed = nullptr;
}

// Getters
int PeriodicSubTask::get_number() const
{
    return *m_number;
}

QDate PeriodicSubTask::get_initial_date() const
{
    return *m_initial_date;
}

QDate PeriodicSubTask::get_delayed_date() const
{
    return *m_delayed_date;
}

bool PeriodicSubTask::get_is_processed() const
{
    return *m_is_processed;
}
