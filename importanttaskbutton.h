#ifndef IMPORTANTTASKBUTTON_H
#define IMPORTANTTASKBUTTON_H

#include "abstracttaskbutton.h"

class ImportantTaskButton : public AbstractTaskButton
{
    public:
        // Constructor and destructor
            ImportantTaskButton(QString const& button_text,
                                BaseTask *task);
            virtual ~ImportantTaskButton();

        // Methods
            virtual void set_button_color();
            virtual void set_checkable_state();
};

#endif // IMPORTANTTASKBUTTON_H
