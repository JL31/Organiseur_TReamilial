
#include "testnewform.h"
#include "ui_testnewform.h"

// Constructor and destructor
// --------------------------

// Constructor
TestNewForm::TestNewForm(QWidget *parent) : QGroupBox(parent),
                                            ui(new Ui::TestNewForm)
{
    ui->setupUi(this);
}

// Desctructor
TestNewForm::~TestNewForm()
{
    delete ui;
}
