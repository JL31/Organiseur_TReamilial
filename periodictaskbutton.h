#ifndef PERIODICTASKBUTTON_H
#define PERIODICTASKBUTTON_H

#include "checkabletaskbutton.h"

class PeriodicTaskButton : public CheckableTaskButton
{
    Q_OBJECT;

    public:
        // Constructor and destructor
            PeriodicTaskButton(QString const& button_text,
                               BaseTask *task,
                               int const& sub_task_number);
            virtual ~PeriodicTaskButton();

        // Methods
            virtual void adapt_button_text();
            virtual void set_button_color();

    public slots:

        void emit_specific_sub_task_signal();

    signals:

        void sub_task_number(int const& number);

    protected:
        // Attributes
            int m_sub_task_number;
};

#endif // PERIODICTASKBUTTON_H
