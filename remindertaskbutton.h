#ifndef REMINDERTASKBUTTON_H
#define REMINDERTASKBUTTON_H

#include "abstractnoncheckabletaskbutton.h"

class ReminderTaskButton : public AbstractNonCheckableTaskButton
{
    public:
        // Constructor and destructor
            ReminderTaskButton(QString const& button_text,
                               BaseTask *task);
            virtual ~ReminderTaskButton();

        // Methods
            virtual void adapt_button_text();
            virtual void set_button_color();
};

#endif // REMINDERTASKBUTTON_H
