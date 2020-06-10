#include "periodicsubtask.h"

// Constructor and destructor
// ==========================

// Constructor
PeriodicSubTask::PeriodicSubTask(int const& number,
                                 QDate const& date,
                                 bool const& is_processed) : m_st_number(number),
                                                             m_st_date(date),
                                                             m_st_is_processed(is_processed)
{}

// Destructor
PeriodicSubTask::~PeriodicSubTask()
{}


// Getters
// =======

int PeriodicSubTask::get_st_number() const
{
    return m_st_number;
}

QDate PeriodicSubTask::get_st_date() const
{
    return m_st_date;
}

bool PeriodicSubTask::get_st_is_processed() const
{
    return m_st_is_processed;
}
