#include "normaltask.h"
#include "reminder.h"

// Constructor and destructor
// --------------------------

// Constructor
NormalTask::NormalTask(int const& number,
                       QString const& name,
                       QString const& comments,
                       bool const& is_important,
                       bool const& is_processed,
                       bool const& is_dated,
                       QDate const& date,
                       bool const& reminder,
                       int const& weeks_before_task) : NonDatedTask(number, name, comments, is_important, is_processed),
                                                       m_is_dated(is_dated),
                                                       m_date(date),
                                                       m_reminder(reminder),
                                                       m_weeks_before_task(weeks_before_task)
{}

// Destructor
NormalTask::~NormalTask()
{}


// Getters
// -------

bool NormalTask::get_is_dated() const
{
    return m_is_dated;
}

QDate NormalTask::get_date() const
{
    return m_date;
}

bool NormalTask::get_reminder() const
{
    return m_reminder;
}

int NormalTask::get_weeks_before_task() const
{
    return m_weeks_before_task;
}
