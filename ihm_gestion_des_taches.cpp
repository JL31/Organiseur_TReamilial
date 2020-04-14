
// Includes
#include "ihm_gestion_des_taches.h"
#include "taskmanager.h"

using namespace std;

// Constructeur
IHMGestionDesTaches::IHMGestionDesTaches(QWidget *parent) : QDialog(parent), ui(new Ui::FenetreGestionDesTaches)
{
    ui->setupUi(this);

    // connexion des signaux et des slots
    connexions();
}


// Destructeur
IHMGestionDesTaches::~IHMGestionDesTaches()
{}


// Code de connexion des signaux et des slots
void IHMGestionDesTaches::connexions() const
{
    connect(ui->validate_button, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
}


// Code pour initialiser la fenêtre de dialogue en vue de la modification d'une tâche
void IHMGestionDesTaches::dialog_initialization_for_task_modification(map<string, string> *data_from_DB)
{
    // dialog widgets initialization
    this->setWindowTitle("Modifier une tâche");
    ui->gb_task_management->setTitle("Modifier une tâche");
    ui->validate_button->setText("Valider la modification");

    ui->le_task_name->setText(QString::fromStdString((*data_from_DB)["NAME"]));

    ui->cb_task_importance->setCurrentText(QString::fromStdString((*data_from_DB)["IMPORTANCE"]));

    QDate *modified_date;
    modified_date = new QDate(QString::fromStdString((*data_from_DB)["YEAR"]).toInt(),
                              QString::fromStdString((*data_from_DB)["MONTH"]).toInt(),
                              QString::fromStdString((*data_from_DB)["DAY"]).toInt());
    ui->de_task_date->setDate(*modified_date);

    int *reminder_state;
    reminder_state = new int(QString::fromStdString((*data_from_DB)["REMINDER"]).toInt());
    ui->gb_reminder->setChecked(*reminder_state);

    if ( *reminder_state )
    {
        ui->sb_nbr_weeks_before_task->setValue(QString::fromStdString((*data_from_DB)["WEEKS_BEFORE_TASK"]).toInt());
    }

    int *periodic_task_state;
    periodic_task_state = new int(QString::fromStdString((*data_from_DB)["PERIODIC_TASK"]).toInt());
    ui->gb_periodic_task->setChecked(*periodic_task_state);

    if ( *periodic_task_state )
    {
        ui->sb_task_periodicty->setValue(QString::fromStdString((*data_from_DB)["PERIODICITY"]).toInt());
    }

    ui->te_task_comments->setPlainText(QString::fromStdString((*data_from_DB)["COMMENTS"]));

    // variables cleaning
    delete modified_date;
    delete reminder_state;
    delete periodic_task_state;

    modified_date = nullptr;
    reminder_state = nullptr;
    periodic_task_state = nullptr;
}


// Accesseurs
QString IHMGestionDesTaches::get_task_name() const
{
    return ui->le_task_name->text();
}

QString IHMGestionDesTaches::get_task_importance() const
{
    return ui->cb_task_importance->currentText();
}

QDate IHMGestionDesTaches::get_task_date() const
{
    return ui->de_task_date->date();
}

bool IHMGestionDesTaches::get_periodic_task_state() const
{
    return ui->gb_periodic_task->isChecked();
}

int IHMGestionDesTaches::get_task_periodicity() const
{
    return ui->sb_task_periodicty->value();
}

bool IHMGestionDesTaches::get_reminder_state() const
{
    return ui->gb_reminder->isChecked();
}

int IHMGestionDesTaches::get_nbr_weeks_before_task() const
{
    return ui->sb_nbr_weeks_before_task->value();
}

QString IHMGestionDesTaches::get_comments() const
{
    return ui->te_task_comments->toPlainText();
}
