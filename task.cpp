
#include <QString>
#include <QDate>
#include "task.h"

using namespace std;

// Constructor
Task::Task(int const& task_number, QString const& task_name, QString const& task_importance, QDate const& task_date, bool const& reminder, int const& nbr_weeks_before_task, QString const& comments) :
           m_task_number(task_number),
           m_task_name(task_name),
           m_task_importance(task_importance),
           m_task_date(task_date),
           m_reminder(reminder),
           m_nbr_weeks_before_task(nbr_weeks_before_task),
           m_comments(comments),
           m_processed_task(false)
{}

//Destructor
Task::~Task()
{}

// Getters
QString Task::get_task_name() const
{
    return m_task_name;
}

QString Task::get_task_importance() const
{
    return m_task_importance;
}

QDate Task::get_task_date() const
{
    return m_task_date;
}

bool Task::get_reminder_state() const
{
    return m_reminder;
}

int Task::get_nbr_weeks_before_task() const
{
    return m_nbr_weeks_before_task;
}

QString Task::get_comments() const
{
    return m_comments;
}

bool Task::get_processed_task() const
{
    return m_processed_task;
}
