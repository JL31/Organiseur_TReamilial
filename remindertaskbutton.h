#ifndef REMINDERTASKBUTTON_H
#define REMINDERTASKBUTTON_H

#include "abstracttaskbutton.h"
#include "reminder.h"

class ReminderTaskButton  : public AbstractTaskButton
{
    public:
        // Constructor and destructor
            ReminderTaskButton(QString const& button_text,
                               NonDatedTask const& task,
                               Reminder const& reminder_task);
            virtual ~ReminderTaskButton();

        // Methods
            virtual void adapt_button_text();
            virtual void set_button_color();
            virtual void set_checkable_state();

    protected:
         // Attributes
            Reminder *m_reminder_task;
};

#endif // REMINDERTASKBUTTON_H
