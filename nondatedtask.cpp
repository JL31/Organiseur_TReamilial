#include "nondatedtask.h"

NonDatedTask::NonDatedTask(int const& number,
                           QString const& name,
                           bool const& is_important,
                           QString const& comments,
                           bool const& is_processed) : m_number(new int(number)),
                                                       m_name(new QString(name)),
                                                       m_is_important(new bool(is_important)),
                                                       m_comments(new QString(comments)),
                                                       m_is_processed(new bool(is_processed))
{}

NonDatedTask::~NonDatedTask()
{
    delete m_number;
    delete m_name;
    delete m_is_important;
    delete m_comments;
    delete m_is_processed;

    m_number = nullptr;
    m_name = nullptr;
    m_is_important = nullptr;
    m_comments = nullptr;
    m_is_processed = nullptr;
}

// Getters
int NonDatedTask::get_number() const
{
    return *m_number;
}

QString NonDatedTask::get_name() const
{
    return *m_name;
}

bool NonDatedTask::get_is_important() const
{
    return *m_is_important;
}

QString NonDatedTask::get_comments() const
{
    return *m_comments;
}

bool NonDatedTask::get_is_processed() const
{
    return *m_is_processed;
}
