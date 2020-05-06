
// Includes
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ihm_gestion_des_taches.h"
#include "checkabletaskbutton.h"
#include "importanttaskbutton.h"
#include "remindertaskbutton.h"

using namespace std;

// Constructor and destructor
// --------------------------

// Constructor
MainWindow::MainWindow(QWidget *parent) :
                       QMainWindow(parent),
                       ui(new Ui::MainWindow),
                       m_data_from_DB(new map<string, string>()),
                       tm(TaskManager()),
                       m_choix_gdt(0),
                       m_selected_task_number(0),
                       m_current_date(QDate(QDate::currentDate())),
                       m_monday_tasks_layout(new QVBoxLayout()),
                       m_tuesday_tasks_layout(new QVBoxLayout()),
                       m_wednesday_tasks_layout(new QVBoxLayout()),
                       m_thursday_tasks_layout(new QVBoxLayout()),
                       m_friday_tasks_layout(new QVBoxLayout()),
                       m_saturday_tasks_layout(new QVBoxLayout()),
                       m_sunday_tasks_layout(new QVBoxLayout()),
                       m_non_dated_tasks_layout(new QVBoxLayout()),
                       m_important_tasks_layout(new QHBoxLayout()),
                       m_monday_tasks_sa_base_widget(new QWidget()),
                       m_tuesday_tasks_sa_base_widget(new QWidget()),
                       m_wednesday_tasks_sa_base_widget(new QWidget()),
                       m_thursday_tasks_sa_base_widget(new QWidget()),
                       m_friday_tasks_sa_base_widget(new QWidget()),
                       m_saturday_tasks_sa_base_widget(new QWidget()),
                       m_sunday_tasks_sa_base_widget(new QWidget()),
                       m_non_dated_tasks_sa_base_widget(new QWidget()),
                       m_important_tasks_sa_base_widget(new QWidget()),
                       m_sa_list(vector<QWidget *>())              // fuite mémoire ou pas lors de la fermeture du pgm ?
{
    // HMI global initializations
    ui->setupUi(this);
    ui->action_Quitter->setShortcuts({ tr("Alt+Q"), tr("Esc") });

    // months mapping initialization
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

    // others initializations
    scroll_areas_initialization();
    actions_connection();
    calendar_week_update();
    tasks_update();
    this->showMaximized();
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete m_data_from_DB;

    ui = nullptr;
    m_data_from_DB = nullptr;
}


// Methods
// -------

// Method that defines the layout for the several days group boxes
void MainWindow::scroll_areas_initialization()
{
    // Setting base widget for all scroll areas
    ui->sa_lundi->setWidget(m_monday_tasks_sa_base_widget);
    ui->sa_mardi->setWidget(m_tuesday_tasks_sa_base_widget);
    ui->sa_mercredi->setWidget(m_wednesday_tasks_sa_base_widget);
    ui->sa_jeudi->setWidget(m_thursday_tasks_sa_base_widget);
    ui->sa_vendredi->setWidget(m_friday_tasks_sa_base_widget);
    ui->sa_samedi->setWidget(m_saturday_tasks_sa_base_widget);
    ui->sa_dimanche->setWidget(m_sunday_tasks_sa_base_widget);
    ui->sa_non_dated_tasks->setWidget(m_non_dated_tasks_sa_base_widget);
    ui->sa_important_tasks->setWidget(m_important_tasks_sa_base_widget);

    // Setting layouts to the scroll areas base widgets
    m_monday_tasks_sa_base_widget->setLayout(m_monday_tasks_layout);
    m_tuesday_tasks_sa_base_widget->setLayout(m_tuesday_tasks_layout);
    m_wednesday_tasks_sa_base_widget->setLayout(m_wednesday_tasks_layout);
    m_thursday_tasks_sa_base_widget->setLayout(m_thursday_tasks_layout);
    m_friday_tasks_sa_base_widget->setLayout(m_friday_tasks_layout);
    m_saturday_tasks_sa_base_widget->setLayout(m_saturday_tasks_layout);
    m_sunday_tasks_sa_base_widget->setLayout(m_sunday_tasks_layout);
    m_non_dated_tasks_sa_base_widget->setLayout(m_non_dated_tasks_layout);
    m_important_tasks_sa_base_widget->setLayout(m_important_tasks_layout);

    // Filling-in of the "sa_list" class member
    m_sa_list.push_back(m_monday_tasks_sa_base_widget);
    m_sa_list.push_back(m_tuesday_tasks_sa_base_widget);
    m_sa_list.push_back(m_wednesday_tasks_sa_base_widget);
    m_sa_list.push_back(m_thursday_tasks_sa_base_widget);
    m_sa_list.push_back(m_friday_tasks_sa_base_widget);
    m_sa_list.push_back(m_saturday_tasks_sa_base_widget);
    m_sa_list.push_back(m_sunday_tasks_sa_base_widget);
    m_sa_list.push_back(m_non_dated_tasks_sa_base_widget);
}


// Method that adds a spacer at the end of the several days group boxes
void MainWindow::days_spacer_addition()
{
    m_monday_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_tuesday_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_wednesday_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_thursday_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_friday_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_saturday_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_sunday_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
}


// Actions connection
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


// Method to update current week calendar days, days number...
void MainWindow::calendar_week_update()
{
    // Variables initialization
    int day_of_the_week = int(m_current_date.dayOfWeek());
    QDate tmp_date = QDate(m_current_date);

    // Current week and year display update
    ui->l_infos_date_courante->setText("Semaine " + QString::number(m_current_date.weekNumber()) + " - " + QString::number(m_current_date.year()));

    // Calendar current week date retrieval
    map<string, pair<int, QString>> day_and_month_values;

    if ( day_of_the_week != 1)
    {
        tmp_date = tmp_date.addDays(- (day_of_the_week) + 1);
    }

    day_and_month_values.insert(make_pair("lundi", make_pair(tmp_date.day(), i_to_s_month[tmp_date.month()])));
    tmp_date = tmp_date.addDays(1);
    day_and_month_values.insert(make_pair("mardi", make_pair(tmp_date.day(), i_to_s_month[tmp_date.month()])));
    tmp_date = tmp_date.addDays(1);
    day_and_month_values.insert(make_pair("mercredi", make_pair(tmp_date.day(), i_to_s_month[tmp_date.month()])));
    tmp_date = tmp_date.addDays(1);
    day_and_month_values.insert(make_pair("jeudi", make_pair(tmp_date.day(), i_to_s_month[tmp_date.month()])));
    tmp_date = tmp_date.addDays(1);
    day_and_month_values.insert(make_pair("vendredi", make_pair(tmp_date.day(), i_to_s_month[tmp_date.month()])));
    tmp_date = tmp_date.addDays(1);
    day_and_month_values.insert(make_pair("samedi", make_pair(tmp_date.day(), i_to_s_month[tmp_date.month()])));
    tmp_date = tmp_date.addDays(1);
    day_and_month_values.insert(make_pair("dimanche", make_pair(tmp_date.day(), i_to_s_month[tmp_date.month()])));

    // Calendar display update
    ui->l_lundi->setText("Lundi\n" + QString::number((day_and_month_values)["lundi"].first) + " " + (day_and_month_values)["lundi"].second);
    ui->l_mardi->setText("Mardi\n" + QString::number((day_and_month_values)["mardi"].first) + " " + (day_and_month_values)["mardi"].second);
    ui->l_mercredi->setText("Mercredi\n" + QString::number((day_and_month_values)["mercredi"].first) + " " + (day_and_month_values)["mercredi"].second);
    ui->l_jeudi->setText("Jeudi\n" + QString::number((day_and_month_values)["jeudi"].first) + " " + (day_and_month_values)["jeudi"].second);
    ui->l_vendredi->setText("Vendredi\n" + QString::number((day_and_month_values)["vendredi"].first) + " " + (day_and_month_values)["vendredi"].second);
    ui->l_samedi->setText("Samedi\n" + QString::number((day_and_month_values)["samedi"].first) + " " + (day_and_month_values)["samedi"].second);
    ui->l_dimanche->setText("Dimanche\n" + QString::number((day_and_month_values)["dimanche"].first) + " " + (day_and_month_values)["dimanche"].second);
}


// Method to update the tasks of the several parts of the HMI
void MainWindow::tasks_update()
{
    prior_cleaning_and_initialization();

    non_dated_tasks_update();
    calendar_tasks_update();
    important_tasks_update();

    connect_task_button_signals();  // laisser cette méthode ici ? avec ce nom ?
}


// Method to clean the tasks, i.e. the buttons, into the differents parts of the HMI
void MainWindow::prior_cleaning_and_initialization()
{
    // CheckableTaskButton list variable initialization
    QList<CheckableTaskButton *> task_button_list = QList<CheckableTaskButton *>();

    // retrieval of the CheckableTaskButton instances of the calendar and non dated tasks parts of the HMI
    for (auto it = m_sa_list.begin(); it != m_sa_list.end(); it++)
    {
        task_button_list.append((*it)->findChildren<CheckableTaskButton *>());
    }

    // deletion of the CheckableTaskButton instances
    for (auto it = task_button_list.begin(); it != task_button_list.end(); it++)
    {
        delete (*it);
        (*it) = nullptr;
    }

    // ... pris sur le net : https://stackoverflow.com/questions/23461614/delete-all-qspaceritem-from-a-layout
    for ( auto it = m_sa_list.begin(); it != m_sa_list.end(); it++ )
    {
        for (int i = 0; i < (*it)->layout()->count(); ++i)
        {
            if ( (*it)->layout()->itemAt(i)->spacerItem() )
            {
                (*it)->layout()->removeItem((*it)->layout()->itemAt(i));

                i--;
            }
        }
    }

    // Reminders buttons list variable initialization
    QList<ReminderTaskButton *> reminder_button_list = QList<ReminderTaskButton *>();

    // retrieval of the reminder_button_list instances of the calendar part of the HMI
    for (auto it = m_sa_list.begin(); it != m_sa_list.end(); it++)
    {
        reminder_button_list.append((*it)->findChildren<ReminderTaskButton *>());
    }

    // deletion of the reminder_button_list instances
    for (auto it = reminder_button_list.begin(); it != reminder_button_list.end(); it++)
    {
        delete (*it);
        (*it) = nullptr;
    }

    // ImportantTaskButton list variable initialization
    QList<ImportantTaskButton *> important_task_button_list = QList<ImportantTaskButton *>();

    // retrieval of the ImportantTaskButton instances of the important tasks part of the HMI
    important_task_button_list.append(ui->sa_important_tasks->findChildren<ImportantTaskButton *>());

    // deletion of the ImportantTaskButton instances
    for (auto it = important_task_button_list.begin(); it != important_task_button_list.end(); it++)
    {
        delete (*it);
        (*it) = nullptr;
    }

    // ... pris sur le net : https://stackoverflow.com/questions/23461614/delete-all-qspaceritem-from-a-layout
    for (int i = 0; i < m_important_tasks_layout->count(); ++i)
    {
        if ( m_important_tasks_layout->itemAt(i)->spacerItem() )
        {
            m_important_tasks_layout->removeItem(m_important_tasks_layout->itemAt(i));

            i--;
        }
    }

    // loading of the current week data through the TaskManager instance
    tm.load_current_week_data(m_current_date.year(), m_current_date.weekNumber());
}


// Method to update the tasks of the non dated tasks part of the HMI
void MainWindow::non_dated_tasks_update()
{
    // tasks creation (i.e. the buttons)
    for (auto it = tm.get_non_dated_tasks_list().begin(); it != tm.get_non_dated_tasks_list().end(); it++)
    {
        m_non_dated_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
    }

    // spacer addition
    m_non_dated_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
}


// Method to update the tasks of the important tasks part of the HMI
void MainWindow::important_tasks_update()
{
    // tasks creation (i.e. the buttons)
    for (auto it = tm.get_important_tasks_list().begin(); it != tm.get_important_tasks_list().end(); it++)
    {
        m_important_tasks_layout->addWidget(new ImportantTaskButton((*it).get_name(), &(*it)));
    }

    // spacer addition
    m_important_tasks_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
}


// Method to update the tasks of the calendar tasks part of the HMI
void MainWindow::calendar_tasks_update()
{
    // tasks creation (i.e. the buttons)
    for (auto it = tm.get_normal_tasks_list().begin(); it != tm.get_normal_tasks_list().end(); it++)
    {
        // variable initialization
        int current_task_day_of_week = int(it->second.get_date().dayOfWeek());

        // switch on the day of the week to create the task button on the appropriate day
        switch (current_task_day_of_week)
        {
            case 1:

                m_monday_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
                break;

            case 2:

                m_tuesday_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
                break;

            case 3:

                m_wednesday_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
                break;

            case 4:

                m_thursday_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
                break;

            case 5:

                m_friday_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
                break;

            case 6:

                m_saturday_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
                break;

            case 7:

                m_sunday_tasks_layout->addWidget(new CheckableTaskButton(it->second.get_name(), &(it->second)));
                break;
        }
    }

    // creation of the reminder tasks (i.e. the buttons)
    reminder_tasks_update();

    // spacers addition
    days_spacer_addition();
}


// Method to update the reminder tasks of the calendar tasks part of the HMI
void MainWindow::reminder_tasks_update()
{
    // tasks creation (i.e. the buttons)
    for ( auto it = tm.get_reminder_tasks_list().begin(); it != tm.get_reminder_tasks_list().end(); it++ )
    {
        // variable initialization
        int current_task_day_of_week = int((*it).get_date().dayOfWeek());

        // switch on the day of the week to create the reminder task button on the appropriate day
        switch (current_task_day_of_week)
        {
            case 1:

                m_monday_tasks_layout->addWidget(new ReminderTaskButton((*it).get_name(), &(*it)));
                break;

            case 2:

                m_tuesday_tasks_layout->addWidget(new ReminderTaskButton((*it).get_name(), &(*it)));
                break;

            case 3:

                m_wednesday_tasks_layout->addWidget(new ReminderTaskButton((*it).get_name(), &(*it)));
                break;

            case 4:

                m_thursday_tasks_layout->addWidget(new ReminderTaskButton((*it).get_name(), &(*it)));
                break;

            case 5:

                m_friday_tasks_layout->addWidget(new ReminderTaskButton((*it).get_name(), &(*it)));
                break;

            case 6:

                m_saturday_tasks_layout->addWidget(new ReminderTaskButton((*it).get_name(), &(*it)));
                break;

            case 7:

                m_sunday_tasks_layout->addWidget(new ReminderTaskButton((*it).get_name(), &(*it)));
                break;
        }
    }
}


// Method to connect the CheckableTaskButton instance signals to the corresponding slots
void MainWindow::connect_task_button_signals()
{
    // variable initialization
    QList<CheckableTaskButton *> task_button_list = QList<CheckableTaskButton *>();

    // retrieval of the CheckableTaskButton instances of the calendar part of the HMI
    for (auto it = m_sa_list.begin(); it != m_sa_list.end(); it++)
    {
        task_button_list.append((*it)->findChildren<CheckableTaskButton *>());
    }

    // connection of the CheckableTaskButton instances signals to the current class slots
    for (auto it = task_button_list.begin(); it != task_button_list.end(); it++)
    {
        connect(*it, SIGNAL(button_state_and_task_number(int)), this, SLOT(change_task_number_and_selected_button(int)));
        connect(*it, SIGNAL(button_task_number(int)), this, SLOT(change_task_number(int)));
    }
}


// SLOTS
// -----

// Method to change the current task number and to uncheck the previously checked button
void MainWindow::change_task_number_and_selected_button(int const& task_number)
{
    QList<CheckableTaskButton *> task_button_list = QList<CheckableTaskButton *>();

    for (auto it = m_sa_list.begin(); it != m_sa_list.end(); it++)
    {
        task_button_list.append((*it)->findChildren<CheckableTaskButton *>());
    }

    for (auto it = task_button_list.begin(); it != task_button_list.end(); it++)
    {
        if ( (*it)->get_task_number() != task_number and (*it)->isChecked() )
        {
            (*it)->setChecked(false);
        }
    }

    m_selected_task_number = task_number;
}


// Method to change the current task number
void MainWindow::change_task_number(int const& task_number)
{
    if ( m_selected_task_number != task_number )
    {
        m_selected_task_number = task_number;
    }
    else
    {
        m_selected_task_number = 0;
    }
}


// SLOT code to go to previous week
void MainWindow::go_to_previous_week()
{
    // current date modification
    QDate updated_date = QDate(m_current_date);
    updated_date = updated_date.addDays(-7);
    m_current_date = updated_date;

    //
    calendar_week_update();

    // tasks update
    tasks_update();
}


// SLOT code to go to next week
void MainWindow::go_to_next_week()
{
    // current date modification
    QDate updated_date = QDate(m_current_date);
    updated_date = updated_date.addDays(7);
    m_current_date = updated_date;

    //
    calendar_week_update();

    // tasks update
    tasks_update();
}


// SLOT code to add a task
void MainWindow::task_addition()
{
    ihm_gdt = new IHMGestionDesTaches(this);
    m_choix_gdt = ihm_gdt->exec();

    if (m_choix_gdt == QDialog::Accepted)
    {
        // variables initialization
        QDate date;
        int weeks_before_task;
        int periodicity;

        // is_dated case
        if ( ihm_gdt->get_is_dated() )
        {
            date = QDate(ihm_gdt->get_date());
        }
        else
        {
            date = QDate(1900, 1, 1);
        }

        // reminder case
        if (ihm_gdt->get_reminder())
        {
            weeks_before_task = int(ihm_gdt->get_weeks_before_task());
        }
        else
        {
            weeks_before_task = int(0);
        }

        // periodic task case
        if (ihm_gdt->get_is_periodic())
        {
            periodicity = int(ihm_gdt->get_periodicity());
        }
        else
        {
            periodicity = int(0);
        }

        // task addition into DB
        tm.add_task(ihm_gdt->get_name(),
                    ihm_gdt->get_is_important(),
                    ihm_gdt->get_comments(),
                    ihm_gdt->get_is_dated(),
                    date,
                    ihm_gdt->get_reminder(),
                    weeks_before_task,
                    ihm_gdt->get_is_periodic(),
                    periodicity);
    }

    // Mise-à-jour des tâches
    tasks_update();

    // attribute cleaning
    delete ihm_gdt;
    ihm_gdt = nullptr;

}


// SLOT code to modify an existing task
void MainWindow::task_modification()
{
    // prior cleaning
    m_data_from_DB->clear();

    // Récupération des infos de la tâche sélectionnée
    tm.data_retrieval(m_selected_task_number, m_data_from_DB);

    // Initialisation de l'IHM de gestion des tâches
    ihm_gdt = new IHMGestionDesTaches(this);
    ihm_gdt->dialog_initialization_for_task_modification(m_data_from_DB);

    // Lancement de l'IHM de gestion des tâches
    m_choix_gdt = ihm_gdt->exec();

    if (m_choix_gdt == QDialog::Accepted)
    {
        // variables initializations
        QDate date;
        int weeks_before_task;
        int periodicity;

        // is_dated case
        if ( ihm_gdt->get_is_dated() )
        {
            date = QDate(ihm_gdt->get_date());
        }
        else
        {
            date = QDate(QDate::currentDate());
        }

        // reminder case
        if (ihm_gdt->get_reminder())
        {
            weeks_before_task = int(ihm_gdt->get_weeks_before_task());
        }
        else
        {
            weeks_before_task = int(0);
        }

        // periodic task case
        if (ihm_gdt->get_is_periodic())
        {
            periodicity = int(ihm_gdt->get_periodicity());
        }
        else
        {
            periodicity = int(0);
        }

        tm.modify_task(m_selected_task_number,
                       ihm_gdt->get_name(),
                       ihm_gdt->get_is_important(),
                       ihm_gdt->get_comments(),
                       ihm_gdt->get_is_dated(),
                       date,
                       ihm_gdt->get_reminder(),
                       weeks_before_task,
                       ihm_gdt->get_is_periodic(),
                       periodicity);
    }

    // Attribute cleaning
    delete ihm_gdt;
    ihm_gdt = nullptr;
}


// SLOT code to set a task as processed
void MainWindow::processed_task()
{
    // variable initialization
    int validation_decision = int(0);

    // validation decision
    validation_decision = QMessageBox::warning(this, "Validation d'une tâche", "Es-tu sûr(e) de vouloir valider cette tâche ?", QMessageBox::Yes | QMessageBox::No);

    // decision treatment
    if ( validation_decision == QMessageBox::Yes )
    {
        tm.validate_task(m_selected_task_number);
        tasks_update();
    }
}

