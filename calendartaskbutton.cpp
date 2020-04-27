#include "calendartaskbutton.h"

// Constructor
CalendarTaskButton::CalendarTaskButton(QString const& button_text,
                                       NonDatedTask const& task) : AbstractTaskButton(button_text, task)
{
    set_button_color();
    set_checkable_state();

    connect(this, SIGNAL(toggled(bool)), this, SLOT(emit_specific_signal(bool)));
}


// Destructor
CalendarTaskButton::~CalendarTaskButton()
{}


// SLOT
void CalendarTaskButton::emit_specific_signal(bool const& button_state)
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


// Method that enables to change the color button (notably is task is Important)
void CalendarTaskButton::set_button_color()
{
    if ( m_task->get_is_important() )
    {
        this->setStyleSheet("background-color:orange");
    }
}


// Method that enables to define if the current button can be checkable or not
void CalendarTaskButton::set_checkable_state()
{
    this->setCheckable(true);
}
