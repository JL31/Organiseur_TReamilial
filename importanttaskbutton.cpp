#include "importanttaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
ImportantTaskButton::ImportantTaskButton(QString const& button_text,
                                         BaseTask *task) : AbstractNonCheckableTaskButton(button_text, task)
{
    adapt_button_text();
    set_button_color();
    set_checkable_state();
}


// Destructor
ImportantTaskButton::~ImportantTaskButton()
{}


// Methods
// -------

// Method that enables to change the color button
void ImportantTaskButton::set_button_color()
{
    this->setStyleSheet("background-color:orange");
}
