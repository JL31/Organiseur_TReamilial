#ifndef NORMALTASK_H
#define NORMALTASK_H

#include "nondatedtask.h"

class NormalTask : public NonDatedTask
{
    public:
        // Constructor and destructor
            NormalTask(int const& number,
                       QString const& name,
                       bool const& is_important,
                       QString const& comments,
                       bool const& is_processed,
                       bool const& is_dated,
                       QDate const& date,
                       bool const& reminder,
                       int const& weeks_before_task);
            virtual ~NormalTask();

        // Getters
            bool get_is_dated() const;
            QDate get_date() const;
            bool get_reminder() const;
            int get_weeks_before_task() const;

    protected:
        // Attributes
            bool *m_is_dated;
            QDate *m_date;
            bool *m_reminder;
            int *m_weeks_before_task;

};

#endif // NORMALTASK_H
