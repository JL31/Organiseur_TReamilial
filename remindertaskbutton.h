#ifndef REMINDERTASKBUTTON_H
#define REMINDERTASKBUTTON_H

#include "abstracttaskbutton.h"

class ReminderTaskButton : public AbstractTaskButton
{
    public:
        // Constructor and destructor
            ReminderTaskButton(QString const& button_text,
                               BaseTask *task);
            virtual ~ReminderTaskButton();

        // Methods
            virtual void adapt_button_text();
            virtual void set_button_color();
            virtual void set_checkable_state();

    protected:
         // Attributes
};

#endif // REMINDERTASKBUTTON_H
