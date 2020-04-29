#include "normaltask.h"
#include "reminder.h"

// Constructor and destructor
// --------------------------

// Constructor
NormalTask::NormalTask(int const& number,
                       QString const& name,
                       bool const& is_important,
                       QString const& comments,
                       bool const& is_processed,
                       bool const& is_dated,
                       QDate const& date,
                       bool const& reminder,
                       int const& weeks_before_task) : NonDatedTask(number, name, is_important, comments, is_processed),
                                                       m_is_dated(new bool(is_dated)),
                                                       m_date(new QDate(date)),
                                                       m_reminder(new bool(reminder)),
                                                       m_weeks_before_task(new int(weeks_before_task))
{}

// Destructor
NormalTask::~NormalTask()
{
    delete m_is_dated;
    delete m_date;
    delete m_reminder;
    delete m_weeks_before_task;

    m_is_dated = nullptr;
    m_date = nullptr;
    m_reminder = nullptr;
    m_weeks_before_task = nullptr;
}


// Getters
// -------

bool NormalTask::get_is_dated() const
{
    return *m_is_dated;
}

QDate NormalTask::get_date() const
{
    return *m_date;
}

bool NormalTask::get_reminder() const
{
    return *m_reminder;
}

int NormalTask::get_weeks_before_task() const
{
    return *m_weeks_before_task;
}
