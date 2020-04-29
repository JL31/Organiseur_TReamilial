#ifndef ABSTRACTTASKBUTTON_H
#define ABSTRACTTASKBUTTON_H

#include <QtWidgets>

#include "nondatedtask.h"
#include "reminder.h"

class AbstractTaskButton : public QPushButton
{
    public:
        // Constructor and destructor
            AbstractTaskButton(QString const& button_text,
                               NonDatedTask *task = nullptr,
                               Reminder *reminder = nullptr);
            virtual ~AbstractTaskButton();

        // Getters
            int get_task_number();

        // Methods
            void mouseDoubleClickEvent(QMouseEvent *event);
            virtual void adapt_button_text();
            virtual void set_button_color() = 0;
            virtual void set_checkable_state() = 0;

    protected:
        // Attributes
            QString *m_button_text;
            NonDatedTask *m_task;
            Reminder *m_reminder;
};

#endif // ABSTRACTTASKBUTTON_H
