#ifndef TASK_DEF
#define TASK_DEF

#include <QString>
#include <QDate>

class Task
{
    public:
        // Constructors et destructor
            Task(int const& task_number, QString const& task_name, QString const& task_importance, QDate const& task_date, bool const& reminder, int const& nbr_weeks_before_task, QString const& comments);
            virtual ~Task();

        // Getters
            QString get_task_name() const;
            QString get_task_importance() const;
            QDate get_task_date() const;
            bool get_reminder_state() const;
            int get_nbr_weeks_before_task() const;
            QString get_comments() const;
            bool get_processed_task() const;

    protected:

        int m_task_number;              ///< task number
        QString m_task_name;            ///< task name
        QString m_task_importance;      ///< task importance
                                        ///< it can have two values : Importante or Normale
                                        ///< if the task is marked as "Importante" then a specific formatting will be applied (e.g. red background or text color)
                                        ///< moreover, after the program opening a window will appear and will indicate that this task needs to be done this day
                                        ///< if the task is marked as "Normale" then no specific formatting will be applied
        QDate m_task_date;              ///< task date at which the task shall be realized
        bool m_reminder;                ///< boolean that indicates if this task possesses a reminder (instance of the "Reminder" class)
        int m_nbr_weeks_before_task;    ///< the number of weeks before the task : will enable to calculate the reminder date
        QString m_comments;             ///< comments related to the task
        bool m_processed_task;          ///< boolean that indicates if the task has been processed
};

#endif  // TASK_DEF
