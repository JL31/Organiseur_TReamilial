#ifndef PERIODICSUBTASK_H
#define PERIODICSUBTASK_H

#include <QDate>

class PeriodicSubTask
{
    public:
        // Constructor and destructor
            PeriodicSubTask(int const& number,
                            QDate const& date,
                            bool const& is_processed);
            virtual ~PeriodicSubTask();

        // Getters
            int get_st_number() const;
            QDate get_st_date() const;
            bool get_st_is_processed() const;

    protected:
        // Attributes
            int m_st_number;
            QDate m_st_date;
            bool m_st_is_processed;
};

#endif // PERIODICSUBTASK_H
