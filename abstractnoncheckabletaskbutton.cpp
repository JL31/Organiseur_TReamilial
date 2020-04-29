#include "abstractnoncheckabletaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
AbstractNonCheckableTaskButton::AbstractNonCheckableTaskButton(QString const& button_text) : AbstractTaskButton(button_text)
{}


// Destructor
AbstractNonCheckableTaskButton::~AbstractNonCheckableTaskButton()
{}


// Method that enables to define if the current button can be checkable or not
void AbstractNonCheckableTaskButton::set_checkable_state()
{
    this->setCheckable(false);
}
