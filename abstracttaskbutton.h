#ifndef ABSTRACTTASKBUTTON_H
#define ABSTRACTTASKBUTTON_H

#include <QtWidgets>

#include "basetask.h"

class AbstractTaskButton : public QPushButton
{
    public:
        // Constructor and destructor
            AbstractTaskButton(QString const& button_text,
                               BaseTask *m_task);
            virtual ~AbstractTaskButton();

        // Methods
            void mouseDoubleClickEvent(QMouseEvent *event);
            virtual void adapt_button_text();
            virtual void set_button_color() = 0;
            virtual void set_checkable_state() = 0;

        // Getters
            int get_task_number() const;

    protected:
        // Attributes
            QString m_button_text;
            BaseTask *m_task;
};

#endif // ABSTRACTTASKBUTTON_H
