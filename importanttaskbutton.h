#ifndef IMPORTANTTASKBUTTON_H
#define IMPORTANTTASKBUTTON_H

#include "abstractnoncheckabletaskbutton.h"

class ImportantTaskButton : public AbstractNonCheckableTaskButton
{
    public:
        // Constructor and destructor
            ImportantTaskButton(QString const& button_text,
                                BaseTask *task);
            virtual ~ImportantTaskButton();

        // Methods
            virtual void set_button_color();
};

#endif // IMPORTANTTASKBUTTON_H
