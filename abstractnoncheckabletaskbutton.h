#ifndef ABSTRACTNONCHECKABLETASKBUTTON_H
#define ABSTRACTNONCHECKABLETASKBUTTON_H

#include "abstracttaskbutton.h"

class AbstractNonCheckableTaskButton : public AbstractTaskButton
{
    public:
        // Constructor and destructor
            AbstractNonCheckableTaskButton(QString const& button_text);
            virtual ~AbstractNonCheckableTaskButton();

        // Methods
            virtual void set_checkable_state();
};

#endif // ABSTRACTNONCHECKABLETASKBUTTON_H
