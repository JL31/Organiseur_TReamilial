#ifndef NONDATEDTASK_H
#define NONDATEDTASK_H

#include "basetask.h"

#include <QString>
#include <QDate>

class NonDatedTask : public BaseTask
{
    public:
        // Constructor and destructor
            NonDatedTask();
            NonDatedTask(int const& number,
                         QString const& name,
                         QString const& comments,
                         bool const& is_important,
                         bool const& is_processed);
            virtual ~NonDatedTask();

        // Getters
            virtual bool get_is_important() const;
            bool get_is_processed() const;

    protected:
        // Attributes
            bool m_is_important;
            bool m_is_processed;
};

#endif // NONDATEDTASK_H
