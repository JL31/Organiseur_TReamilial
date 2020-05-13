#ifndef ABSTRACTNONCHECKABLETASKBUTTON_H
#define ABSTRACTNONCHECKABLETASKBUTTON_H

#include "abstracttaskbutton.h"

class AbstractNonCheckableTaskButton : public AbstractTaskButton
{
    public:
        // Constructor and destructor
            AbstractNonCheckableTaskButton(QString const& button_text,
                                           BaseTask *task);
            virtual ~AbstractNonCheckableTaskButton();

        // Methods
            void mouseDoubleClickEvent(QMouseEvent *event);
            virtual void set_button_color() = 0;
            void set_checkable_state();
};

#endif // ABSTRACTNONCHECKABLETASKBUTTON_H
