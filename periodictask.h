#ifndef PERIODICTASK_H
#define PERIODICTASK_H

#include "normaltask.h"
#include "periodicsubtask.h"

#include <vector>

class PeriodicTask : public NormalTask
{
    public:
        // Constructor and destructor
            PeriodicTask();
            PeriodicTask(int const& number,
                         QString const& name,
                         QString const& comments,
                         bool const& is_important,
                         bool const& is_processed,
                         bool const& is_dated,
                         QDate const& date,
                         bool const& reminder,
                         int const& weeks_before_task,
                         bool const& is_periodic,
                         int const& periodicity);
            virtual ~PeriodicTask();

        // Getters
            bool get_is_periodic() const;
            int get_periodicity() const;
            std::vector<PeriodicSubTask> get_periodic_sub_tasks_list() const;

        // Other methods
            void add_sub_task(int const& number,
                              QDate const& date,
                              bool const& is_processed);

    protected:
        // Attributes
            bool m_is_periodic;
            int m_periodicity;
            std::vector<PeriodicSubTask> m_periodic_sub_tasks;
};

#endif // PERIODICTASK_H
