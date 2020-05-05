#include "reminder.h"

// Constructor and destructor
// --------------------------

// Constructor
Reminder::Reminder(int const& number,
                   QString const& name,
                   QString const& comments,
                   QDate const& date,
                   int const& weeks_before_task) : BaseTask(number, name, comments),
                                                   m_date(date),
                                                   m_weeks_before_task(weeks_before_task)
{}

// Destructor
Reminder::~Reminder()
{}


// Getters
// -------

QDate Reminder::get_date() const
{
    return m_date;
}

int Reminder::get_weeks_before_task() const
{
    return m_weeks_before_task;
}
