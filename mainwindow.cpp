
// Includes
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ihm_gestion_des_taches.h"
#include "calendartaskbutton.h"
#include "importanttaskbutton.h"
#include "remindertaskbutton.h"

using namespace std;

// Constructeur
MainWindow::MainWindow(QWidget *parent) :
                       QMainWindow(parent),
                       ui(new Ui::MainWindow),
                       tm(new TaskManager()),
                       m_data_from_DB(new map<string, string>),
                       m_current_date(new QDate(QDate::currentDate())),
                       m_current_week_tasks(new vector<int>),
                       m_current_week_periodic_tasks(new vector<int>),
                       m_layout_taches_lundi(new QVBoxLayout()),
                       m_layout_taches_mardi(new QVBoxLayout()),
                       m_layout_taches_mercredi(new QVBoxLayout()),
                       m_layout_taches_jeudi(new QVBoxLayout()),
                       m_layout_taches_vendredi(new QVBoxLayout()),
                       m_layout_taches_samedi(new QVBoxLayout()),
                       m_layout_taches_dimanche(new QVBoxLayout()),
                       m_sa_list(new vector<QScrollArea*>),
                       m_selected_task_number(new int(0))
{
    ui->setupUi(this);
    ui->action_Quitter->setShortcuts({ tr("Alt+Q"), tr("Esc") });

    i_to_s_month[1] = "janvier";
    i_to_s_month[2] = "février";
    i_to_s_month[3] = "mars";
    i_to_s_month[4] = "avril";
    i_to_s_month[5] = "mai";
    i_to_s_month[6] = "juin";
    i_to_s_month[7] = "juillet";
    i_to_s_month[8] = "août";
    i_to_s_month[9] = "septembre";
    i_to_s_month[10] = "octobre";
    i_to_s_month[11] = "novembre";
    i_to_s_month[12] = "décembre";

    m_sa_list->push_back(ui->sa_lundi);
    m_sa_list->push_back(ui->sa_mardi);
    m_sa_list->push_back(ui->sa_mercredi);
    m_sa_list->push_back(ui->sa_jeudi);
    m_sa_list->push_back(ui->sa_vendredi);
    m_sa_list->push_back(ui->sa_samedi);
    m_sa_list->push_back(ui->sa_dimanche);

    days_layout_definition();
    actions_connection();
    calendar_week_update();
    tm->load_current_week_data(m_current_date->year(), m_current_date->weekNumber());
    tasks_update();
    //important_tasks_update();
    //reminder_tasks_update();
    //current_week_tasks_update();
    // current_week_periodic_task_update();
}

// Destructeur
MainWindow::~MainWindow()
{
    delete ui;
    delete tm;
    delete ihm_gdt;
    delete m_data_from_DB;
    delete m_current_week_tasks,
    delete m_sa_list;
    delete m_selected_task_number;

    ui = nullptr;
    tm = nullptr;
    ihm_gdt = nullptr;
    m_data_from_DB = nullptr;
    m_current_week_tasks = nullptr;
    m_sa_list = nullptr;
    m_selected_task_number = nullptr;
}

// Method that defines the layout for the several days group boxes
void MainWindow::days_layout_definition()
{
    ui->sa_lundi->setLayout(m_layout_taches_lundi);
    ui->sa_mardi->setLayout(m_layout_taches_mardi);
    ui->sa_mercredi->setLayout(m_layout_taches_mercredi);
    ui->sa_jeudi->setLayout(m_layout_taches_jeudi);
    ui->sa_vendredi->setLayout(m_layout_taches_vendredi);
    ui->sa_samedi->setLayout(m_layout_taches_samedi);
    ui->sa_dimanche->setLayout(m_layout_taches_dimanche);
}

// Method that adds a spacer at the end of the several days group boxes
void MainWindow::days_spacer_addition()
{
    m_layout_taches_lundi->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_layout_taches_mardi->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_layout_taches_mercredi->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_layout_taches_jeudi->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_layout_taches_vendredi->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_layout_taches_samedi->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_layout_taches_dimanche->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

// Connexion des actions
void MainWindow::actions_connection() const
{
    // Connexion des actions du menu "Menu"
    connect(ui->action_Quitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Connexion des actions du menu "Gestion des tâches"
    connect(ui->action_Ajouter_une_tache, SIGNAL(triggered()), this, SLOT(task_addition()));

    connect(ui->action_Modifier_une_tache, SIGNAL(triggered()), this, SLOT(task_modification()));
    connect(ui->action_Valider_une_tache, SIGNAL(triggered()), this, SLOT(processed_task()));

    connect(ui->b_semaine_precedente, SIGNAL(clicked()), this, SLOT(go_to_previous_week()));
    connect(ui->b_semaine_suivante, SIGNAL(clicked()), this, SLOT(go_to_next_week()));
}

// Méthode qui permet de mettre à jour le calendrier de la semaine courante
void MainWindow::calendar_week_update()
{
    // Variables initialization
    int *day_of_the_week = new int(m_current_date->dayOfWeek());
    QDate *tmp_date = new QDate(*m_current_date);

    // Current week and year display update
    ui->l_infos_date_courante->setText("Semaine " + QString::number(m_current_date->weekNumber()) + " - " + QString::number(m_current_date->year()));

    // Calendar current week date retrieval
    map<string, pair<int, QString>> *day_and_month_values = new map<string, pair<int, QString>>();

    if ( *day_of_the_week != 1)
    {
        *tmp_date = tmp_date->addDays(- (*day_of_the_week) + 1);
    }

    day_and_month_values->insert(make_pair("lundi", make_pair(tmp_date->day(), i_to_s_month[tmp_date->month()])));
    *tmp_date = tmp_date->addDays(1);
    day_and_month_values->insert(make_pair("mardi", make_pair(tmp_date->day(), i_to_s_month[tmp_date->month()])));
    *tmp_date = tmp_date->addDays(1);
    day_and_month_values->insert(make_pair("mercredi", make_pair(tmp_date->day(), i_to_s_month[tmp_date->month()])));
    *tmp_date = tmp_date->addDays(1);
    day_and_month_values->insert(make_pair("jeudi", make_pair(tmp_date->day(), i_to_s_month[tmp_date->month()])));
    *tmp_date = tmp_date->addDays(1);
    day_and_month_values->insert(make_pair("vendredi", make_pair(tmp_date->day(), i_to_s_month[tmp_date->month()])));
    *tmp_date = tmp_date->addDays(1);
    day_and_month_values->insert(make_pair("samedi", make_pair(tmp_date->day(), i_to_s_month[tmp_date->month()])));
    *tmp_date = tmp_date->addDays(1);
    day_and_month_values->insert(make_pair("dimanche", make_pair(tmp_date->day(), i_to_s_month[tmp_date->month()])));

    // Calendar display update
    ui->l_lundi->setText("Lundi\n" + QString::number((*day_and_month_values)["lundi"].first) + " " + (*day_and_month_values)["lundi"].second);
    ui->l_mardi->setText("Mardi\n" + QString::number((*day_and_month_values)["mardi"].first) + " " + (*day_and_month_values)["mardi"].second);
    ui->l_mercredi->setText("Mercredi\n" + QString::number((*day_and_month_values)["mercredi"].first) + " " + (*day_and_month_values)["mercredi"].second);
    ui->l_jeudi->setText("Jeudi\n" + QString::number((*day_and_month_values)["jeudi"].first) + " " + (*day_and_month_values)["jeudi"].second);
    ui->l_vendredi->setText("Vendredi\n" + QString::number((*day_and_month_values)["vendredi"].first) + " " + (*day_and_month_values)["vendredi"].second);
    ui->l_samedi->setText("Samedi\n" + QString::number((*day_and_month_values)["samedi"].first) + " " + (*day_and_month_values)["samedi"].second);
    ui->l_dimanche->setText("Dimanche\n" + QString::number((*day_and_month_values)["dimanche"].first) + " " + (*day_and_month_values)["dimanche"].second);

    // Variables cleaning
    delete tmp_date;
    tmp_date = nullptr;
    delete day_of_the_week;
    day_of_the_week = nullptr;
    delete day_and_month_values;
    day_and_month_values = nullptr;
}


// Method to update the tasks of the several parts of the HMI
void MainWindow::tasks_update()
{
    //non_dated_tasks_update();
    //important_tasks_update();
    calendar_tasks_update();
}


// Method to update the tasks of the non dated tasks part of the HMI
void MainWindow::non_dated_tasks_update()
{
    // non dated tasks part prior cleaning
    // ... à implémenter, sans doute dans une autre méthode
}


// Method to update the tasks of the important tasks part of the HMI
void MainWindow::important_tasks_update()
{
    // important tasks part prior cleaning
}


// Method to update the tasks of the calendar tasks part of the HMI
void MainWindow::calendar_tasks_update()
{
    // calendar tasks part prior cleaning

    // Création des tâches (i.e. les boutons)
    for (auto it = tm->get_normal_tasks_list().begin(); it != tm->get_normal_tasks_list().end(); it++)
    {
        int *current_task_day_of_week = new int(it->second->get_date().dayOfWeek());

        switch (*current_task_day_of_week)
        {
            case 1:

                m_layout_taches_lundi->addWidget(new CalendarTaskButton(it->second->get_name(), *(it->second)));
                break;

            case 2:

                m_layout_taches_mardi->addWidget(new CalendarTaskButton(it->second->get_name(), *(it->second)));
                break;

            case 3:

                m_layout_taches_mercredi->addWidget(new CalendarTaskButton(it->second->get_name(), *(it->second)));
                break;

            case 4:

                m_layout_taches_jeudi->addWidget(new CalendarTaskButton(it->second->get_name(), *(it->second)));
                break;

            case 5:

                m_layout_taches_vendredi->addWidget(new CalendarTaskButton(it->second->get_name(), *(it->second)));
                break;

            case 6:

                m_layout_taches_samedi->addWidget(new CalendarTaskButton(it->second->get_name(), *(it->second)));
                break;

            case 7:

                m_layout_taches_dimanche->addWidget(new CalendarTaskButton(it->second->get_name(), *(it->second)));
                break;
        }

        delete current_task_day_of_week;
        current_task_day_of_week = nullptr;
    }

    // Connect the TaskButton signals to Mainwindow slots
    connect_task_button_signals();

    // ...
    // reminder_tasks_update();

    // ajout d'un spacer dans chacune des group box des différents jours
    days_spacer_addition();
}


/*// Code de la méthode de chargement des tâches de la semaine courante
void MainWindow::current_week_tasks_update()
{
    // nettoyage préalable
    current_week_tasks_cleaning();

    // Récupération des numéros des tâches de la semaine courante
    tm->load_current_week_tasks_numbers(m_current_date->year(), m_current_date->weekNumber(), m_current_week_tasks);

    // Chargement des tâches de la semaine courante
    for (vector<int>::iterator it = m_current_week_tasks->begin(); it != m_current_week_tasks->end(); it++)
    {
        tm->load_current_week_tasks(*it);
    }

    // Création des tâches (i.e. les boutons)
    map<int, NormalTask*> normal_task_list = tm->get_normal_tasks_list();

    for (map<int, NormalTask*>::iterator ottl_iterator = normal_task_list.begin(); ottl_iterator != normal_task_list.end(); ottl_iterator++)
    {
        int *current_task_day_of_week = new int(ottl_iterator->second->get_task_date().dayOfWeek());

        switch (*current_task_day_of_week)
        {
            case 1:

                m_layout_taches_lundi->addWidget(new TaskButton(ottl_iterator->second->get_task_name(), ottl_iterator->first, ottl_iterator->second->get_task_importance(), ottl_iterator->second->get_comments(), ottl_iterator->second->get_reminder_state(), true, this));
                break;

            case 2:

                m_layout_taches_mardi->addWidget(new TaskButton(ottl_iterator->second->get_task_name(), ottl_iterator->first, ottl_iterator->second->get_task_importance(), ottl_iterator->second->get_comments(), ottl_iterator->second->get_reminder_state(), true, this));
                break;

            case 3:

                m_layout_taches_mercredi->addWidget(new TaskButton(ottl_iterator->second->get_task_name(), ottl_iterator->first, ottl_iterator->second->get_task_importance(), ottl_iterator->second->get_comments(), ottl_iterator->second->get_reminder_state(), true, this));
                break;

            case 4:

                m_layout_taches_jeudi->addWidget(new TaskButton(ottl_iterator->second->get_task_name(), ottl_iterator->first, ottl_iterator->second->get_task_importance(), ottl_iterator->second->get_comments(), ottl_iterator->second->get_reminder_state(), true, this));
                break;

            case 5:

                m_layout_taches_vendredi->addWidget(new TaskButton(ottl_iterator->second->get_task_name(), ottl_iterator->first, ottl_iterator->second->get_task_importance(), ottl_iterator->second->get_comments(), ottl_iterator->second->get_reminder_state(), true, this));
                break;

            case 6:

                m_layout_taches_samedi->addWidget(new TaskButton(ottl_iterator->second->get_task_name(), ottl_iterator->first, ottl_iterator->second->get_task_importance(), ottl_iterator->second->get_comments(), ottl_iterator->second->get_reminder_state(), true, this));
                break;

            case 7:

                m_layout_taches_dimanche->addWidget(new TaskButton(ottl_iterator->second->get_task_name(), ottl_iterator->first, ottl_iterator->second->get_task_importance(), ottl_iterator->second->get_comments(), ottl_iterator->second->get_reminder_state(), true, this));
                break;
        }

        delete current_task_day_of_week;
        current_task_day_of_week = nullptr;
    }

    // Connect the TaskButton signals to Mainwindow slots
    connect_task_button_signals();

    // ...
    reminder_tasks_update();

    // ajout d'un spacer dans chacune des group box des différents jours
    days_spacer_addition();
}
*/

//
void MainWindow::connect_task_button_signals()
{
    // variable initialization
    QList<CalendarTaskButton *> *task_button_list = new QList<CalendarTaskButton *>();

    // retrieval of the CalendarTaskButton instances
    for (auto it = m_sa_list->begin(); it != m_sa_list->end(); it++)
    {
        task_button_list->append((*it)->findChildren<CalendarTaskButton *>());
    }

    // connection of the CalendarTaskButton instances signals to the current class slots
    for (auto it = task_button_list->begin(); it != task_button_list->end(); it++)
    {
        connect(*it, SIGNAL(button_state_and_task_number(int)), this, SLOT(change_task_number_and_selected_button(int)));
        connect(*it, SIGNAL(button_task_number(int)), this, SLOT(change_task_number(int)));
    }

    // variable cleaning
    delete task_button_list;
    task_button_list = nullptr;
}

//
void MainWindow::change_task_number_and_selected_button(int const& task_number)
{
    QList<CalendarTaskButton *> *task_button_list = new QList<CalendarTaskButton *>();

    for (auto it = m_sa_list->begin(); it != m_sa_list->end(); it++)
    {
        task_button_list->append((*it)->findChildren<CalendarTaskButton *>());
    }

    for (auto it = task_button_list->begin(); it != task_button_list->end(); it++)
    {
        if ( (*it)->get_task_number() != task_number and (*it)->isChecked() )
        {
            (*it)->setChecked(false);
        }
    }

    *m_selected_task_number = task_number;
}

//
void MainWindow::change_task_number(int const& task_number)
{
    if ( *m_selected_task_number != task_number )
    {
        *m_selected_task_number = task_number;
    }
    else
    {
        *m_selected_task_number = 0;
    }
}
/*
//
void MainWindow::current_week_tasks_cleaning()
{
    QList<TaskButton *> *task_button_list = new QList<TaskButton *>();

    for (vector<QScrollArea *>::iterator sa_list_iterator = m_sa_list->begin(); sa_list_iterator != m_sa_list->end(); sa_list_iterator++)
    {
        task_button_list->append((*sa_list_iterator)->findChildren<TaskButton *>());
    }

    for (QList<TaskButton *>::iterator tb_iterator = task_button_list->begin(); tb_iterator != task_button_list->end(); tb_iterator++)
    {
        delete (*tb_iterator);
        (*tb_iterator) = nullptr;
    }

    //
    m_current_week_tasks->clear();
    tm->clear_current_week_tasks();
    tm->clear_current_week_reminder_tasks();
}

//
void MainWindow::current_week_important_tasks_cleaning()
{
    QList<TaskButton *> *important_task_button_list = new QList<TaskButton *>();

    important_task_button_list->append(ui->gb_important_tasks->findChildren<TaskButton *>());

    for (QList<TaskButton *>::iterator tb_iterator = important_task_button_list->begin(); tb_iterator != important_task_button_list->end(); tb_iterator++)
    {
        delete (*tb_iterator);
        (*tb_iterator) = nullptr;
    }

    //
    tm->clear_current_week_important_tasks();
}

// Code du SLOT pour passer à la semaine précédente
void MainWindow::go_to_previous_week()
{
    QDate *updated_date = new QDate(*m_current_date);

    *updated_date = updated_date->addDays(-7);

    *m_current_date = *updated_date;

    calendar_week_update();
    important_tasks_update();
    current_week_tasks_update();

    delete updated_date;
    updated_date = nullptr;
}

// Code du SLOT pour passer à la semaine suivante
void MainWindow::go_to_next_week()
{
    QDate *updated_date = new QDate(*m_current_date);

    *updated_date = updated_date->addDays(7);

    *m_current_date = *updated_date;

    calendar_week_update();
    important_tasks_update();
    current_week_tasks_update();

    delete updated_date;
    updated_date = nullptr;
}
*/
// Code du SLOT d'ajout de tâche
void MainWindow::task_addition()
{
    ihm_gdt = new IHMGestionDesTaches(this);
    m_choix_gdt = ihm_gdt->exec();

    if (m_choix_gdt == QDialog::Accepted)
    {
        // variables initialization
        QDate *date;
        int *weeks_before_task;
        int *periodicity;

        // is_dated case
        if ( ihm_gdt->get_is_dated() )
        {
            date = new QDate(ihm_gdt->get_date());
        }
        else
        {
            date = new QDate(1900, 1, 1);
        }

        // reminder case
        if (ihm_gdt->get_reminder())
        {
            weeks_before_task = new int(ihm_gdt->get_weeks_before_task());
        }
        else
        {
            weeks_before_task = new int(0);
        }

        // periodic task case
        if (ihm_gdt->get_is_periodic())
        {
            periodicity = new int(ihm_gdt->get_periodicity());
        }
        else
        {
            periodicity = new int(0);
        }

        // task addition into DB
        tm->add_task(ihm_gdt->get_name(),
                     ihm_gdt->get_is_important(),
                     ihm_gdt->get_comments(),
                     ihm_gdt->get_is_dated(),
                     *date,
                     ihm_gdt->get_reminder(),
                     *weeks_before_task,
                     ihm_gdt->get_is_periodic(),
                     *periodicity);

        // variables cleaning
        delete date;
        delete weeks_before_task;
        delete periodicity;

        date = nullptr;
        weeks_before_task = nullptr;
        periodicity = nullptr;
    }
    /*
    // Mise-à-jour des tâches importantes / reminder / tâches "classiques"
    if ( ihm_gdt->get_is_important() )
    {
        important_tasks_update();
    }
    else if ( ihm_gdt->get_reminder() )
    {
        reminder_tasks_update();
    }

    current_week_tasks_update();
    */
    // attribute cleaning
    delete ihm_gdt;
    ihm_gdt = nullptr;

}

// Code du SLOT de modification de tâche
void MainWindow::task_modification()
{
    // prior cleaning
    m_data_from_DB->clear();

    // Récupération des infos de la tâche sélectionnée
    tm->data_retrieval(*m_selected_task_number, m_data_from_DB);

    // Initialisation de l'IHM de gestion des tâches
    ihm_gdt = new IHMGestionDesTaches(this);
    ihm_gdt->dialog_initialization_for_task_modification(m_data_from_DB);

    // Lancement de l'IHM de gestion des tâches
    m_choix_gdt = ihm_gdt->exec();

    if (m_choix_gdt == QDialog::Accepted)
    {
        // variables initializations
        QDate *date;
        int *weeks_before_task;
        int *periodicity;

        // is_dated case
        if ( ihm_gdt->get_is_dated() )
        {
            date = new QDate(ihm_gdt->get_date());
        }
        else
        {
            date = new QDate(QDate::currentDate());
        }

        // reminder case
        if (ihm_gdt->get_reminder())
        {
            weeks_before_task = new int(ihm_gdt->get_weeks_before_task());
        }
        else
        {
            weeks_before_task = new int(0);
        }

        // periodic task case
        if (ihm_gdt->get_is_periodic())
        {
            periodicity = new int(ihm_gdt->get_periodicity());
        }
        else
        {
            periodicity = new int(0);
        }

        tm->modify_task(*m_selected_task_number,
                        ihm_gdt->get_name(),
                        ihm_gdt->get_is_important(),
                        ihm_gdt->get_comments(),
                        ihm_gdt->get_is_dated(),
                        *date,
                        ihm_gdt->get_reminder(),
                        *weeks_before_task,
                        ihm_gdt->get_is_periodic(),
                        *periodicity);

        // variables cleaning
        delete date;
        delete weeks_before_task;
        delete periodicity;

        date = nullptr;
        weeks_before_task = nullptr;
        periodicity = nullptr;
    }
    else if (m_choix_gdt == QDialog::Rejected)
    {
        QMessageBox::information(this, "REJET", "X");
    }

    // Attribute cleaning
    delete ihm_gdt;
    ihm_gdt = nullptr;
}
/*
// Code du SLOT qui permet de marquer une tâche comme terminée
void MainWindow::processed_task()
{
    // variable initialization
    int *validation_decision = new int(0);

    // validation decision
    *validation_decision = QMessageBox::warning(this, "Validation d'une tâche", "Es-tu sûr(e) de vouloir valider cette tâche ?", QMessageBox::Yes | QMessageBox::No);

    // decision treatment
    if ( *validation_decision == QMessageBox::Yes )
    {
        tm->validate_task(*m_selected_task_number);
        important_tasks_update();
        current_week_tasks_update();
    }

    // variable cleaning
    delete validation_decision;
    validation_decision = nullptr;
}
*/
/*
//
void MainWindow::important_tasks_update()
{
    // nettoyage préalable
    current_week_important_tasks_cleaning();

    // variables initialization
    vector<int> *current_week_important_tasks_list = new vector<int>();

    // current week important tasks numbers retrieval
    tm->load_current_week_important_tasks_list(m_current_date->year(), m_current_date->weekNumber(), current_week_important_tasks_list);

    // Chargement des tâches de la semaine courante
    for (vector<int>::iterator it = current_week_important_tasks_list->begin(); it != current_week_important_tasks_list->end(); it++)
    {
        tm->load_current_week_important_tasks(*it);
    }

    // Tasks creation (i.e. the buttons)
    map<int, Task*> *important_task_list = new map<int, Task*>(tm->get_important_task_list());

    for (map<int, Task*>::iterator itl_iterator = important_task_list->begin(); itl_iterator != important_task_list->end(); itl_iterator++)
    {
        ui->gb_important_tasks_layout->addWidget(new TaskButton(itl_iterator->second->get_task_name(), itl_iterator->first, itl_iterator->second->get_task_importance(), itl_iterator->second->get_comments(), itl_iterator->second->get_reminder_state(), false, false ,this, true));
    }

    // Spacer addition
    ui->gb_important_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // variables cleaning
    delete important_task_list;
    delete current_week_important_tasks_list;

    important_task_list = nullptr;
    current_week_important_tasks_list = nullptr;
}
*/

/*
//
void MainWindow::reminder_tasks_update()
{
    // variables initialization
    vector<int> *reminder_tasks_numbers_list = new vector<int>();

    // reminder tasks numbers list retrieval
    tm->load_reminder_tasks_numbers_list(m_current_date->year(), m_current_date->weekNumber(), reminder_tasks_numbers_list);

    // load tasks from reminder tasks numbers list
    for (vector<int>::iterator rtnl_iterator = reminder_tasks_numbers_list->begin(); rtnl_iterator != reminder_tasks_numbers_list->end(); rtnl_iterator++)
    {
        tm->load_reminder_tasks_list(*rtnl_iterator);
    }

    // add button tasks (no checkable)
    map<int, Task*> *reminder_task_list = new map<int, Task*>(tm->get_reminder_task_list());

    for (map<int, Task*>::iterator rtl_iterator = reminder_task_list->begin(); rtl_iterator != reminder_task_list->end(); rtl_iterator++)
    {
        int *current_task_day_of_week = new int(rtl_iterator->second->get_task_date().dayOfWeek());

        switch (*current_task_day_of_week)
        {
            case 1:

                m_layout_taches_lundi->addWidget(new TaskButton(rtl_iterator->second->get_task_name(), rtl_iterator->first, rtl_iterator->second->get_task_importance(), rtl_iterator->second->get_comments(), rtl_iterator->second->get_reminder_state(), false, false, this, false, rtl_iterator->second->get_nbr_weeks_before_task()));
                break;

            case 2:

                m_layout_taches_mardi->addWidget(new TaskButton(rtl_iterator->second->get_task_name(), rtl_iterator->first, rtl_iterator->second->get_task_importance(), rtl_iterator->second->get_comments(), rtl_iterator->second->get_reminder_state(), false, false, this, false, rtl_iterator->second->get_nbr_weeks_before_task()));
                break;

            case 3:

                m_layout_taches_mercredi->addWidget(new TaskButton(rtl_iterator->second->get_task_name(), rtl_iterator->first, rtl_iterator->second->get_task_importance(), rtl_iterator->second->get_comments(), rtl_iterator->second->get_reminder_state(), false, false, this, false, rtl_iterator->second->get_nbr_weeks_before_task()));
                break;

            case 4:

                m_layout_taches_jeudi->addWidget(new TaskButton(rtl_iterator->second->get_task_name(), rtl_iterator->first, rtl_iterator->second->get_task_importance(), rtl_iterator->second->get_comments(), rtl_iterator->second->get_reminder_state(), false, false, this, false, rtl_iterator->second->get_nbr_weeks_before_task()));
                break;

            case 5:

                m_layout_taches_vendredi->addWidget(new TaskButton(rtl_iterator->second->get_task_name(), rtl_iterator->first, rtl_iterator->second->get_task_importance(), rtl_iterator->second->get_comments(), rtl_iterator->second->get_reminder_state(), false, false, this, false, rtl_iterator->second->get_nbr_weeks_before_task()));
                break;

            case 6:

                m_layout_taches_samedi->addWidget(new TaskButton(rtl_iterator->second->get_task_name(), rtl_iterator->first, rtl_iterator->second->get_task_importance(), rtl_iterator->second->get_comments(), rtl_iterator->second->get_reminder_state(), false, false, this, false, rtl_iterator->second->get_nbr_weeks_before_task()));
                break;

            case 7:

                m_layout_taches_dimanche->addWidget(new TaskButton(rtl_iterator->second->get_task_name(), rtl_iterator->first, rtl_iterator->second->get_task_importance(), rtl_iterator->second->get_comments(), rtl_iterator->second->get_reminder_state(), false, false, this, false, rtl_iterator->second->get_nbr_weeks_before_task()));
                break;
        }

        delete current_task_day_of_week;
        current_task_day_of_week = nullptr;
    }

    // variables cleaning
    delete reminder_task_list;
    delete reminder_tasks_numbers_list;

    reminder_task_list = nullptr;
    reminder_tasks_numbers_list = nullptr;
}
*/

// Méthode qui permet d'exécuter une autre méthode après le chargement de l'IHM
/*void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    current_week_tasks_update();
}*/

//
//
// FAUT-IL CONSERVER CETTE METHODE OU PLUTOT MUTUALISER LE NETTOYAGE DANS LA METHODE "current_week_tasks_cleaning" ?
//
//
/*
void MainWindow::current_week_periodic_tasks_cleaning()
{
    QList<TaskButton *> *task_button_list = new QList<TaskButton *>();

    for (vector<QScrollArea *>::iterator sa_list_iterator = m_sa_list->begin(); sa_list_iterator != m_sa_list->end(); sa_list_iterator++)
    {
        task_button_list->append((*sa_list_iterator)->findChildren<TaskButton *>());
    }

    for (QList<TaskButton *>::iterator tb_iterator = task_button_list->begin(); tb_iterator != task_button_list->end(); tb_iterator++)
    {
        if ( (*tb_iterator)->get_task_button_is_periodic_task() )
        {
            delete (*tb_iterator);
            (*tb_iterator) = nullptr;
        }
    }

    //
    m_current_week_periodic_tasks->clear();
    tm->clear_current_week_periodic_tasks();
}
*/

//
/*
void MainWindow::current_week_periodic_task_update()
{
    // nettoyage préalable
    current_week_periodic_tasks_cleaning();

    // Récupération des numéros des tâches périodiques de la semaine courante
    tm->load_current_week_periodic_tasks_numbers(m_current_date->year(), m_current_date->weekNumber(), m_current_week_periodic_tasks);

    // Chargement des tâches de la semaine courante
    for (vector<int>::iterator it = m_current_week_periodic_tasks->begin(); it != m_current_week_periodic_tasks->end(); it++)
    {
        tm->load_current_week_periodic_tasks(*it);
    }

    // Création des tâches (i.e. les boutons)
    map<int, PeriodicTask*> periodic_task_list = tm->get_periodic_task_list();

    for (map<int, PeriodicTask*>::iterator ptl_iterator = periodic_task_list.begin(); ptl_iterator != periodic_task_list.end(); ptl_iterator++)
    {
        int *current_task_day_of_week = new int(ptl_iterator->second->get_task_date().dayOfWeek());

        switch (*current_task_day_of_week)
        {
            case 1:

                m_layout_taches_lundi->addWidget(new TaskButton(ptl_iterator->second->get_task_name(), ptl_iterator->first, ptl_iterator->second->get_task_importance(), ptl_iterator->second->get_comments(), ptl_iterator->second->get_reminder_state(), true, this));
                break;

            case 2:

                m_layout_taches_mardi->addWidget(new TaskButton(ptl_iterator->second->get_task_name(), ptl_iterator->first, ptl_iterator->second->get_task_importance(), ptl_iterator->second->get_comments(), ptl_iterator->second->get_reminder_state(), true, this));
                break;

            case 3:

                m_layout_taches_mercredi->addWidget(new TaskButton(ptl_iterator->second->get_task_name(), ptl_iterator->first, ptl_iterator->second->get_task_importance(), ptl_iterator->second->get_comments(), ptl_iterator->second->get_reminder_state(), true, this));
                break;

            case 4:

                m_layout_taches_jeudi->addWidget(new TaskButton(ptl_iterator->second->get_task_name(), ptl_iterator->first, ptl_iterator->second->get_task_importance(), ptl_iterator->second->get_comments(), ptl_iterator->second->get_reminder_state(), true, this));
                break;

            case 5:

                m_layout_taches_vendredi->addWidget(new TaskButton(ptl_iterator->second->get_task_name(), ptl_iterator->first, ptl_iterator->second->get_task_importance(), ptl_iterator->second->get_comments(), ptl_iterator->second->get_reminder_state(), true, this));
                break;

            case 6:

                m_layout_taches_samedi->addWidget(new TaskButton(ptl_iterator->second->get_task_name(), ptl_iterator->first, ptl_iterator->second->get_task_importance(), ptl_iterator->second->get_comments(), ptl_iterator->second->get_reminder_state(), true, this));
                break;

            case 7:

                m_layout_taches_dimanche->addWidget(new TaskButton(ptl_iterator->second->get_task_name(), ptl_iterator->first, ptl_iterator->second->get_task_importance(), ptl_iterator->second->get_comments(), ptl_iterator->second->get_reminder_state(), true, this));
                break;
        }

        delete current_task_day_of_week;
        current_task_day_of_week = nullptr;
    }

    // Connect the TaskButton signals to Mainwindow slots
    connect_task_button_signals();

    // ...
    reminder_tasks_update();

    // ajout d'un spacer dans chacune des group box des différents jours
    days_spacer_addition();
}
*/
