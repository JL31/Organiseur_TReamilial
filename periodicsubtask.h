#ifndef PERIODICSUBTASK_H
#define PERIODICSUBTASK_H

#include <QDate>

class PeriodicSubTask
{
    public:
        // Constructor and destructor
            PeriodicSubTask(int const& number,
                            QDate const& initial_date,
                            QDate const& delayed_date,
                            bool const& is_processed);
            virtual ~PeriodicSubTask();

        // Getters
            int get_number() const;
            QDate get_initial_date() const;
            QDate get_delayed_date() const;
            bool get_is_processed() const;

    protected:
        // Attributes
            int *m_number;
            QDate *m_initial_date;
            QDate *m_delayed_date;
            bool *m_is_processed;
};

#endif // PERIODICSUBTASK_H
