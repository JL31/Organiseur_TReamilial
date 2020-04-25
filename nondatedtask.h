#ifndef NONDATEDTASK_H
#define NONDATEDTASK_H

#include <QString>
#include <QDate>

class NonDatedTask
{
    public:
        // Constructor and destructor
            NonDatedTask(int const& number,
                         QString const& name,
                         bool const& is_important,
                         QString const& comments,
                         bool const& is_processed);
            virtual ~NonDatedTask();

        // Getters
            int get_number() const;
            QString get_name() const;
            bool get_is_important() const;
            QString get_comments() const;
            bool get_is_processed() const;

    protected:
        // Attributes
            int *m_number;
            QString *m_name;
            bool *m_is_important;
            QString *m_comments;
            bool *m_is_processed;
};

#endif // NONDATEDTASK_H
