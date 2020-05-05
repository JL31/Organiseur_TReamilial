#include "basetask.h"

// Constructor and destructor
// --------------------------

// Constructor
BaseTask::BaseTask(int number,
                   QString name,
                   QString comments) : m_number(number),
                                       m_name(name),
                                       m_comments(comments)
{}


// Destructor
BaseTask::~BaseTask()
{}


// Getters
// -------

int BaseTask::get_number() const
{
    return m_number;
}

QString BaseTask::get_name() const
{
    return m_name;
}

QString BaseTask::get_comments() const
{
    return m_comments;
}


// Virtual getters
// ---------------

bool BaseTask::get_is_important() const
{
    return false;
}

int BaseTask::get_weeks_before_task() const
{
    return 0;
}
