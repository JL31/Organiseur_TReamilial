#include "nondatedtask.h"

// Constructor and destructor
// --------------------------

// Constructor
NonDatedTask::NonDatedTask(int const& number,
                           QString const& name,
                           QString const& comments,
                           bool const& is_important,
                           bool const& is_processed) : BaseTask(number, name, comments),
                                                       m_is_important(is_important),
                                                       m_is_processed(is_processed)
{}

// Destructor
NonDatedTask::~NonDatedTask()
{}


// Getters
// -------

bool NonDatedTask::get_is_important() const
{
    return m_is_important;
}

bool NonDatedTask::get_is_processed() const
{
    return m_is_processed;
}
