#include "periodictaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
PeriodicTaskButton::PeriodicTaskButton(QString const& button_text,
                                       BaseTask *task,
                                       int const& sub_task_number) : CheckableTaskButton(button_text, task),
                                                                     m_sub_task_number(sub_task_number)
{
    adapt_button_text();
    set_button_color();
    set_checkable_state();

    connect(this, SIGNAL(toggled(bool)), this, SLOT(emit_specific_signal(bool)));
    connect(this, SIGNAL(toggled(bool)), this, SLOT(emit_specific_sub_task_signal()));
}

// Destructor
PeriodicTaskButton::~PeriodicTaskButton()
{}


// Methods
// -------

// Method that enables to modify the button text
void PeriodicTaskButton::adapt_button_text()
{
    // variable initialization
    QString new_texte;

    // definition of the button text
    new_texte.append("Tâche ");
    new_texte.append(QString::number(m_task->get_number()));
    new_texte.append(" - ");
    new_texte.append(QString::number(m_sub_task_number));
    new_texte.append("\n\n");

    if ( m_button_text.length() > 11 )
    {
        new_texte.append(m_button_text.left(8) + "...");
    }
    else
    {
        new_texte.append(m_button_text);
    }

    // modification of the button text
    this->setText(new_texte);
}

// Method that enables to change the color button
void PeriodicTaskButton::set_button_color()
{
    if ( m_task->get_is_important() )
    {
        this->setStyleSheet("background-color:orange");
    }
    else
    {
        this->setStyleSheet("background-color:rgb(255, 255, 10)");
    }
}


// SLOTS
// -----

// Code of the SLOT that emits a signal when the button is toggled
void PeriodicTaskButton::emit_specific_sub_task_signal()
{
    emit sub_task_number(m_sub_task_number);
}
