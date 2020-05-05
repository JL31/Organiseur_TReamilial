#ifndef BASETASK_H
#define BASETASK_H

#include <QString>

class BaseTask
{
    public:
        // Constructor and destructor
            BaseTask(int number,
                     QString name,
                     QString comments);
            virtual ~BaseTask();

        // Getters
            int get_number() const;
            QString get_name() const;
            QString get_comments() const;

        // Virtual getters
            virtual bool get_is_important() const;
            virtual int get_weeks_before_task() const;

    protected:
        // Attributes
            int m_number;
            QString m_name;
            QString m_comments;
};

#endif // BASETASK_H
