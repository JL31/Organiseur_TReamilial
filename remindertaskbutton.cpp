#include "remindertaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
ReminderTaskButton::ReminderTaskButton(QString const& button_text,
                                       NonDatedTask *task,
                                       Reminder *reminder_task) : AbstractTaskButton(button_text, task, reminder_task)
{
    adapt_button_text();
    set_button_color();
    set_checkable_state();
}


// Desctructor
ReminderTaskButton::~ReminderTaskButton()
{}


// Method that enables to modify the button text
void ReminderTaskButton::adapt_button_text()
{
    // variable initialization
    QString *new_texte = new QString();

    // definition of the button text
    new_texte->append("Rappel de la\n");
    new_texte->append("tâche n°");
    new_texte->append(QString::number(m_reminder->get_number()));
    new_texte->append("\n\n");
    new_texte->append("Prévue dans\n");
    new_texte->append(QString::number(m_reminder->get_weeks_before_task()));
    new_texte->append(" semaine");

    if ( m_reminder->get_weeks_before_task() > 1)
    {
        new_texte->append("s");
    }

    /*if ( text.size() > 10 )
    {
        // compliqué, à terminer plus tard...
        // il faudra gérer la hauteur du bouton pour l'adapter aux nombres de lignes du texte du bouton
        // il faudra couper le texte à la fonction de la longueur du texte
        // il ne faudra pas couper de mots
        new_texte->append(text);
    }
    else
    {
        new_texte->append(text);
    }*/

    // modification of the button text
    this->setText(*new_texte);

    // variable cleaning
    delete new_texte;
    new_texte = nullptr;
}


// Method that enables to change the color button
void ReminderTaskButton::set_button_color()
{
    this->setStyleSheet("background-color:rgb(0, 150, 220)");
}


// Method that enables to define if the current button can be checkable or not
void ReminderTaskButton::set_checkable_state()
{
    this->setCheckable(false);
}
