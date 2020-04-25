#ifndef REMINDER_H
#define REMINDER_H

#include <QString>
#include <QDate>

class Reminder
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
            int get_number() const;
            QString get_name() const;
            QString get_comments() const;
            QDate get_date() const;
            int get_weeks_before_task() const;

    protected:
        // Attributes
            int *m_number;
            QString *m_name;
            QString *m_comments;
            QDate *m_date;
            int *m_weeks_before_task;
};

#endif // REMINDER_H
