#include "reminder.h"

// Constructor
Reminder::Reminder(int const& number,
                   QString const& name,
                   QString const& comments,
                   QDate const& date,
                   int const& weeks_before_task) : m_number(new int(number)),
                                                   m_name(new QString(name)),
                                                   m_comments(new QString(comments)),
                                                   m_date(new QDate(date)),
                                                   m_weeks_before_task(new int(weeks_before_task))
{}

// Destructor
Reminder::~Reminder()
{
    delete m_number;
    delete m_name;
    delete m_comments;
    delete m_date;
    delete m_weeks_before_task;

    m_number = nullptr;
    m_name = nullptr;
    m_comments = nullptr;
    m_date = nullptr;
    m_weeks_before_task = nullptr;
}

// Getters
int Reminder::get_number() const
{
    return *m_number;
}

QString Reminder::get_name() const
{
    return *m_name;
}

QString Reminder::get_comments() const
{
    return *m_comments;
}

QDate Reminder::get_date() const
{
    return *m_date;
}

int Reminder::get_weeks_before_task() const
{
    return *m_weeks_before_task;
}
