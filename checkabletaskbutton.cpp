#include "checkabletaskbutton.h"

// Constructor and Destructor
// --------------------------

// Constructor
CheckableTaskButton::CheckableTaskButton(QString const& button_text,
                                         NonDatedTask *task,
                                         Reminder *reminder) : AbstractTaskButton(button_text, task, reminder)
{
    adapt_button_text();
    set_button_color();
    set_checkable_state();

    connect(this, SIGNAL(toggled(bool)), this, SLOT(emit_specific_signal(bool)));
}


// Destructor
CheckableTaskButton::~CheckableTaskButton()
{}


// SLOTS
// -----

// Code of the SLOT that emits different signals when the button is toggled
void CheckableTaskButton::emit_specific_signal(bool const& button_state)
{
    if ( button_state )
    {
        emit button_state_and_task_number(m_task->get_number());
    }
    else
    {
        emit button_task_number(m_task->get_number());
    }
}


// Methods
// -------

// Method that enables to change the color button (notably if task is Important)
void CheckableTaskButton::set_button_color()
{
    if ( m_task->get_is_important() )
    {
        this->setStyleSheet("background-color:orange");
    }
}


// Method that enables to define if the current button can be checkable or not
void CheckableTaskButton::set_checkable_state()
{
    this->setCheckable(true);
}
