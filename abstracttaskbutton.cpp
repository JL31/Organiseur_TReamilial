#include "abstracttaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
AbstractTaskButton::AbstractTaskButton(QString const& button_text,
                                       BaseTask *task) : m_button_text(button_text),
                                                         m_task(task)
{
    int taille = 95;
    this->setMinimumSize(taille, taille);
    this->setMaximumSize(taille, taille);
    this->setContentsMargins(5, 5, 5, 5);
}


// Destructor
AbstractTaskButton::~AbstractTaskButton()
{}


// Methods
// -------

// Method that enables to modify the button text
void AbstractTaskButton::adapt_button_text()
{
    // variable initialization
    QString new_texte;

    // definition of the button text
    new_texte.append("Tâche ");
    new_texte.append(QString::number(m_task->get_number()));
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


// Getters
// -------

int AbstractTaskButton::get_task_number() const
{
    return m_task->get_number();
}
