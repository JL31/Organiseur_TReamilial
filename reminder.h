#ifndef REMINDER_H
#define REMINDER_H

#include "basetask.h"

#include <QString>
#include <QDate>

class Reminder : public BaseTask
{
    public:
        // Constructor and destructor
            Reminder(int const& number,
                     QString const& name,
                     QString const& comments,
                     QDate const& date,
                     int const& weeks_before_task);
            virtual ~Reminder();

        // Getters
            QDate get_date() const;
            int get_weeks_before_task() const;

    protected:
        // Attributes
            QDate m_date;
            int m_weeks_before_task;
};

#endif // REMINDER_H
