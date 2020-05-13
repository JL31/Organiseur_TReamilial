
// Includes
#include "ihm_gestion_des_taches.h"
#include "taskmanager.h"
#include <QMessageBox>

using namespace std;

// Constructeur
IHMGestionDesTaches::IHMGestionDesTaches(QWidget *parent) : QDialog(parent), ui(new Ui::FenetreGestionDesTaches)
{
    ui->setupUi(this);

    ui->de_task_date->setDate(QDate::currentDate());    // initialisation à la date du jour

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
    connect(ui->gb_dated_task, SIGNAL(toggled(bool)), this, SLOT(dated_task_selection()));

    connect(ui->le_task_name, SIGNAL(textChanged(QString)), this, SLOT(name_content_length_check(QString)));
}


// ...
void IHMGestionDesTaches::name_content_length_check(QString const& text)
{
    if ( text.length() > 11 and text.split(" ")[0].length() > 11 )
    {
        QString message = "Merci d'utiliser moins de 12 caractères pour le premier mot du nom de la tâche";
        QMessageBox::information(this, "Trop de caractères pour le premier mot !", message);

        ui->le_task_name->backspace();  // permet de supprimer le caractère en trop
    }
}


// Code du slot qui permet de gérer les GroupBox concernant les options pour :
// - les rappels
// - les tâches périodiques
// selon si l'utilisateur à choisi ou non que la tâche soit datée
void IHMGestionDesTaches::dated_task_selection()
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


// Code pour initialiser la fenêtre de dialogue en vue de la modification d'une tâche
void IHMGestionDesTaches::dialog_initialization_for_task_modification(map<string, string> *data_from_DB)
{
    // dialog widgets initialization
    this->setWindowTitle("Modifier une tâche");
    ui->gb_task_management->setTitle("Modifier une tâche");
    ui->validate_button->setText("Valider la modification");

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


// Accesseurs
QString IHMGestionDesTaches::get_name() const
{
    return ui->le_task_name->text();
}

bool IHMGestionDesTaches::get_is_important() const
{
    return ui->cb_important_task->isChecked();
}

bool IHMGestionDesTaches::get_is_dated() const
{
    return ui->gb_dated_task->isChecked();
}

QDate IHMGestionDesTaches::get_date() const
{
    return ui->de_task_date->date();
}

bool IHMGestionDesTaches::get_is_periodic() const
{
    return ui->gb_periodic_task->isChecked();
}

int IHMGestionDesTaches::get_periodicity() const
{
    return ui->sb_task_periodicty->value();
}

bool IHMGestionDesTaches::get_reminder() const
{
    return ui->gb_reminder->isChecked();
}

int IHMGestionDesTaches::get_weeks_before_task() const
{
    return ui->sb_nbr_weeks_before_task->value();
}

QString IHMGestionDesTaches::get_comments() const
{
    return ui->te_task_comments->toPlainText();
}
