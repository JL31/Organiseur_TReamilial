#include "taskhandlinghmi.h"
#include "ui_task_handling_hmi.h"

using namespace std;

// Constructor and destructor
// --------------------------

// Constructor
TaskHandlingHMI::TaskHandlingHMI(QWidget *parent) : QWidget(parent),
                                                    ui(new Ui::task_handling_hmi)
{
    ui->setupUi(this);

    ui->de_task_date->setDate(QDate::currentDate());    // initialisation à la date du jour

    // taille de base des boutons cancel et ajouter : 80

    // signals and slots connection
    connections();
}

// Destructor
TaskHandlingHMI::~TaskHandlingHMI()
{
    delete ui;
    ui = nullptr;
}


// Methods
// -------

// Method to connect the signals and slots
void TaskHandlingHMI::connections() const
{
    connect(ui->gb_dated_task, SIGNAL(toggled(bool)), this, SLOT(dated_task_selection()));
    connect(ui->le_task_name, SIGNAL(textChanged(QString)), this, SLOT(name_content_length_check(QString)));
}

// Code pour initialiser la fenêtre de dialogue en vue de la modification d'une tâche
void TaskHandlingHMI::dialog_initialization_for_task_modification(map<string, string> *data_from_DB)
{
    // dialog widgets initialization
    this->setWindowTitle("Modifier / valider une tâche");
    ui->gb_task_management->setTitle("Modifier / valider une tâche");

    ui->cancel_validate_button->setText("Valider la tâche");
    ui->add_modify_button->setText("Modifier la tâche");

    ui->cancel_validate_button->setMaximumSize(125, 30);
    ui->add_modify_button->setMaximumSize(125, 30);

    ui->le_task_name->setText(QString::fromStdString((*data_from_DB)["NAME"]));

    if ( stoi((*data_from_DB)["IS_IMPORTANT"]) == 1 )
    {
        ui->cb_important_task->setChecked(true);
    }
    else
    {
        ui->cb_important_task->setChecked(false);
    }

    if ( stoi((*data_from_DB)["IS_DATED"]) == 1 )
    {
        ui->gb_dated_task->setChecked(true);

        QDate *date;
        date = new QDate(stoi((*data_from_DB)["YEAR"]),
                         stoi((*data_from_DB)["MONTH"]),
                         stoi((*data_from_DB)["DAY"]));
        ui->de_task_date->setDate(*date);

        delete date;
        date = nullptr;
    }
    else
    {
        ui->gb_dated_task->setChecked(false);
    }

    int *reminder_state;
    reminder_state = new int(QString::fromStdString((*data_from_DB)["REMINDER"]).toInt());
    ui->gb_reminder->setChecked(*reminder_state);

    if ( *reminder_state )
    {
        ui->sb_nbr_weeks_before_task->setValue(QString::fromStdString((*data_from_DB)["WEEKS_BEFORE_TASK"]).toInt());
    }

    int *is_periodic;
    is_periodic = new int(QString::fromStdString((*data_from_DB)["IS_PERIODIC"]).toInt());
    ui->gb_periodic_task->setChecked(*is_periodic);

    if ( *is_periodic )
    {
        ui->sb_task_periodicty->setValue(QString::fromStdString((*data_from_DB)["PERIODICITY"]).toInt());
    }

    ui->te_task_comments->setPlainText(QString::fromStdString((*data_from_DB)["COMMENTS"]));

    // variables cleaning
    delete reminder_state;
    delete is_periodic;

    reminder_state = nullptr;
    is_periodic = nullptr;
}


// Getters
// -------

QPushButton* TaskHandlingHMI::get_cancel_validate_button()
{
    return ui->cancel_validate_button;
}

QPushButton* TaskHandlingHMI::get_add_modify_button()
{
    return ui->add_modify_button;
}

QString TaskHandlingHMI::get_name() const
{
    return ui->le_task_name->text();
}

bool TaskHandlingHMI::get_is_important() const
{
    return ui->cb_important_task->isChecked();
}

bool TaskHandlingHMI::get_is_dated() const
{
    return ui->gb_dated_task->isChecked();
}

QDate TaskHandlingHMI::get_date() const
{
    return ui->de_task_date->date();
}

bool TaskHandlingHMI::get_is_periodic() const
{
    return ui->gb_periodic_task->isChecked();
}

int TaskHandlingHMI::get_periodicity() const
{
    return ui->sb_task_periodicty->value();
}

bool TaskHandlingHMI::get_reminder() const
{
    return ui->gb_reminder->isChecked();
}

int TaskHandlingHMI::get_weeks_before_task() const
{
    return ui->sb_nbr_weeks_before_task->value();
}

QString TaskHandlingHMI::get_comments() const
{
    return ui->te_task_comments->toPlainText();
}


// SLOTS
// -----

// SLOT code that enables to handle the group boxes as regards the options for :
// - reminders
// - periodic tasks
// Wether the user choose the task to be dated or not
void TaskHandlingHMI::dated_task_selection()
{
    if ( ui->gb_dated_task->isChecked() )
    {
        ui->gb_reminder->setEnabled(true);
        ui->gb_periodic_task->setEnabled(true);
    }
    else
    {
        ui->gb_reminder->setEnabled(false);
        ui->gb_reminder->setChecked(false);

        ui->gb_periodic_task->setEnabled(false);
        ui->gb_periodic_task->setChecked(false);
    }
}

// SLOT code to check the length of the name content
void TaskHandlingHMI::name_content_length_check(QString const& text)
{
    if ( text.length() > 11 and text.split(" ")[0].length() > 11 )
    {
        QString message = "Merci d'utiliser moins de 12 caractères pour le premier mot du nom de la tâche";
        QMessageBox::information(this, "Trop de caractères pour le premier mot !", message);

        ui->le_task_name->backspace();  // permet de supprimer le caractère en trop
    }
}
