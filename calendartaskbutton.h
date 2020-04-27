#ifndef CALENDARTASKBUTTON_H
#define CALENDARTASKBUTTON_H

#include <QObject>

#include "abstracttaskbutton.h"

class CalendarTaskButton : public AbstractTaskButton
{
    Q_OBJECT

    public:
        // Constructor and destructor
            CalendarTaskButton(QString const& button_text,
                               NonDatedTask const& task);
            virtual ~CalendarTaskButton();

        // Methods
            void set_button_color();
            virtual void set_checkable_state();

    public slots:
        void emit_specific_signal(bool const& button_state);

    signals:

        void button_state_and_task_number(int const& number);
        void button_task_number(int const& number);

    protected:
        // Attributes
};

#endif // CALENDARTASKBUTTON_H
