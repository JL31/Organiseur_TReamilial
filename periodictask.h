#ifndef PERIODICTASK_H
#define PERIODICTASK_H

#include <QString>

#include "task.h"

class PeriodicTask : public Task
{
    public:
        // Constructors and destructor
            PeriodicTask(int const& task_number, QString const& task_name, QString const& task_importance, QDate const& task_date, bool const& reminder, int const& nbr_weeks_before_task, QString const& comments, int const& periodicity);
            virtual ~PeriodicTask();

    protected:
        int *m_periodicity;
};

#endif // PERIODICTASK_H
