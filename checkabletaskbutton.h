#ifndef CHECKABLETASKBUTTON_H
#define CHECKABLETASKBUTTON_H

#include <QObject>

#include "abstracttaskbutton.h"

class CheckableTaskButton : public AbstractTaskButton
{
    Q_OBJECT

    public:
        // Constructor and destructor
            CheckableTaskButton(QString const& button_text,
                                BaseTask *task);
            virtual ~CheckableTaskButton();

        // Methods
            virtual void set_button_color();
            virtual void set_checkable_state();

    public slots:
        void emit_specific_signal(bool const& button_state);

    signals:

        void button_state_and_task_number(int const& number);
        void button_task_number(int const& number);
};

#endif // CHECKABLETASKBUTTON_H
