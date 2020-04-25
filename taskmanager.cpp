#include "taskmanager.h"

#include <QtWidgets>

using namespace std;

// Constructor
TaskManager::TaskManager() :
                             m_non_dated_tasks_list(new map<int, NonDatedTask*>()),
                             m_normal_tasks_list(new map<int, NormalTask*>()),
                             m_periodic_tasks_list(new map<int, PeriodicTask*>()),
                             m_reminder_tasks_list(new vector<Reminder*>()),
                             m_important_tasks_list(new vector<NormalTask*>()),
                             m_number_of_tasks(new int(0)),
                             m_db_handler(new DatabaseHandler())
{}

// Destructor
TaskManager::~TaskManager()
{
    // free memory
    delete m_non_dated_tasks_list;
    delete m_normal_tasks_list;
    delete m_periodic_tasks_list;
    delete m_important_tasks_list;
    delete m_reminder_tasks_list;
    delete m_number_of_tasks;
    delete m_db_handler;

    // ...
    m_non_dated_tasks_list = nullptr;
    m_normal_tasks_list = nullptr;
    m_periodic_tasks_list = nullptr;
    m_important_tasks_list = nullptr;
    m_reminder_tasks_list = nullptr;
    m_number_of_tasks = nullptr;
    m_db_handler = nullptr;
}

// Getters
int TaskManager::get_number_of_tasks()
const
{
    return *m_number_of_tasks;
}

// Method to add a task into the DB
void TaskManager::add_task(QString const& name,
                           bool const& is_important,
                           QString const& comments,
                           bool const& is_dated,
                           QDate const& date,
                           bool const& reminder,
                           int const& weeks_before_task,
                           bool const& is_periodic,
                           int const& periodicity)
{
    // calculation of the task number
    m_db_handler->DB_task_addition(name.toUtf8().constData(),
                                   to_string(is_important),
                                   comments.toUtf8().constData(),
                                   to_string(is_dated),
                                   to_string(date.day()),
                                   to_string(date.month()),
                                   to_string(date.year()),
                                   to_string(date.weekNumber()),
                                   to_string(reminder),
                                   to_string(weeks_before_task),
                                   to_string(is_periodic),
                                   to_string(periodicity));
}


// Method to modify an existing task into the DB
void TaskManager::modify_task(int const& number,
                              QString const& name,
                              bool const& is_important,
                              QString const& comments,
                              bool const& is_dated,
                              QDate const& date,
                              bool const& reminder,
                              int const& weeks_before_task,
                              bool const& is_periodic,
                              int const& periodicity)
{
    m_db_handler->DB_task_modification(to_string(number),
                                       name.toUtf8().constData(),
                                       to_string(is_important),
                                       comments.toUtf8().constData(),
                                       to_string(is_dated),
                                       to_string(date.day()),
                                       to_string(date.month()),
                                       to_string(date.year()),
                                       to_string(date.weekNumber()),
                                       to_string(reminder),
                                       to_string(weeks_before_task),
                                       to_string(is_periodic),
                                       to_string(periodicity));
}


//
void TaskManager::data_retrieval(int const& number, map<string, string> *data_from_DB)
{
    m_db_handler->DB_task_data_retrieval(number, data_from_DB);
}

//
void TaskManager::validate_task(int const& number)
{
    m_db_handler->DB_task_validation(to_string(number));
}


// Method to load othe several types of tasks for the current week
void TaskManager::load_current_week_data(int const& current_year, int const& current_week_number)
{
    // prior cleaning
    task_lists_cleaning();

    // several types tasks loading
    load_non_dated_tasks();
    load_normal_tasks(current_year, current_week_number);
    //load_periodic_tasks(current_year, current_week_number);
    load_reminders_list(current_year, current_week_number);
    load_important_tasks_list(current_year, current_week_number);
}


// Method to clean the several attributes that contain the several types tasks instances
void TaskManager::task_lists_cleaning()
{
    m_non_dated_tasks_list->clear();
    m_normal_tasks_list->clear();
    m_periodic_tasks_list->clear();
    m_important_tasks_list->clear();
    m_reminder_tasks_list->clear();
}


// Method to load the non dated tasks
void TaskManager::load_non_dated_tasks()
{
    // attribute initialization
    m_data_extraction_from_DB = new vector<map<string, string>>();

    // data retrieval into database
    m_db_handler->DB_load_non_dated_tasks(m_data_extraction_from_DB);

    // non dated tasks list filling-in with data extracted from database
    for (auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++)
    {
        m_non_dated_tasks_list->insert(make_pair(stoi((*it)["NUMBER"]),
                                                 new NonDatedTask(stoi((*it)["NUMBER"]),
                                                                  QString::fromStdString((*it)["NAME"]),
                                                                  stoi((*it)["IS_IMPORTANT"]),
                                                                  QString::fromStdString((*it)["COMMENTS"]),
                                                                  stoi((*it)["IS_PROCESSED"])
                                                                 )
                                                 )
                                       );
    }

    // attribute cleaning
    delete m_data_extraction_from_DB;
    m_data_extraction_from_DB = nullptr;
}


// Method to load the normal tasks
void TaskManager::load_normal_tasks(int const& current_year, int const& current_week_number)
{
    // attribute initialization
    m_data_extraction_from_DB = new vector<map<string, string>>();

    // data retrieval into database
    m_db_handler->DB_load_normal_tasks(to_string(current_year), to_string(current_week_number), m_data_extraction_from_DB);

    // non dated tasks list filling-in with data extracted from database
    for (auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++)
    {
        m_normal_tasks_list->insert(make_pair(stoi((*it)["NUMBER"]),
                                              new NormalTask(stoi((*it)["NUMBER"]),
                                                             QString::fromStdString((*it)["NAME"]),
                                                             stoi((*it)["IS_IMPORTANT"]),
                                                             QString::fromStdString((*it)["COMMENTS"]),
                                                             stoi((*it)["IS_PROCESSED"]),
                                                             stoi((*it)["IS_DATED"]),
                                                             QDate(stoi((*it)["YEAR"]),
                                                                   stoi((*it)["MONTH"]),
                                                                   stoi((*it)["DAY"])),
                                                             stoi((*it)["REMINDER"]),
                                                             stoi((*it)["WEEKS_BEFORE_TASK"])
                                                            )
                                             )
                                   );
    }

    // attribute cleaning
    delete m_data_extraction_from_DB;
    m_data_extraction_from_DB = nullptr;
}


// Method to load the reminder tasks
void TaskManager::load_reminders_list(int const& current_year, int const& current_week_number)
{
    // attributes initialization
    m_data_extraction_from_DB = new vector<map<string, string>>();

    // priori reminder data retrieval into database
    m_db_handler->DB_prior_step_for_reminder_tasks_loading(m_data_extraction_from_DB);

    // variables initialization
    vector<int> *tmp_reminder_tasks_numbers = new vector<int>();

    // ...
    for (auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++)
    {
        QDate *tmp_date = new QDate(stoi((*it)["YEAR"]), stoi((*it)["MONTH"]), stoi((*it)["DAY"]));
        *tmp_date = tmp_date->addDays(-7 * stoi((*it)["WEEKS_BEFORE_TASK"]));

        if ( tmp_date->year() == current_year and tmp_date->weekNumber() == current_week_number )
        {
            tmp_reminder_tasks_numbers->push_back(stoi((*it)["NUMBER"]));
        }

        delete tmp_date;
        tmp_date = nullptr;
    }

    // ...
    for ( auto it = tmp_reminder_tasks_numbers->begin(); it != tmp_reminder_tasks_numbers->end(); it++ )
    {
        // variables initialization
        map<string, string> *m_reminder_data_extraction_from_DB = new map<string, string>();

        // reminder data retrieval into database
        m_db_handler->DB_reminder_task_loading_from_reminder_task_number(to_string(*it), m_reminder_data_extraction_from_DB);

        // ...
        m_reminder_tasks_list->push_back(new Reminder(stoi((*m_reminder_data_extraction_from_DB)["NUMBER"]),
                                                     QString::fromStdString((*m_reminder_data_extraction_from_DB)["NAME"]),
                                                     QString::fromStdString((*m_reminder_data_extraction_from_DB)["COMMENTS"]),
                                                     QDate(stoi((*m_reminder_data_extraction_from_DB)["YEAR"]),
                                                           stoi((*m_reminder_data_extraction_from_DB)["MONTH"]),
                                                           stoi((*m_reminder_data_extraction_from_DB)["DAY"])),
                                                     stoi((*m_reminder_data_extraction_from_DB)["WEEKS_BEFORE_TASK"])));

        // variables cleaning
        delete m_reminder_data_extraction_from_DB;
        m_reminder_data_extraction_from_DB = nullptr;
    }

    // variables cleaning
    delete tmp_reminder_tasks_numbers;
    tmp_reminder_tasks_numbers = nullptr;

    // attributes cleaning
    delete m_data_extraction_from_DB;
    m_data_extraction_from_DB = nullptr;
}


// Method to load the important tasks
// This method will actually fills-in the important tasks list vector with existing objects of the NormalTask class
void TaskManager::load_important_tasks_list(int const& current_year, int const& current_week_number)
{
    // important tasks list filling-in with important normal tasks of the current week
    for (auto it = m_normal_tasks_list->begin(); it != m_normal_tasks_list->end(); it++)
    {
        if ( ( it->second->get_date().year() == current_year ) and
             ( it->second->get_date().weekNumber() == current_week_number ) and
              it->second->get_is_important() )
        {
            m_important_tasks_list->push_back(it->second);
        }
    }
}


// Getter of the non dated tasks list
const map<int, NonDatedTask*>& TaskManager::get_non_dated_tasks_list() const
{
    return *m_non_dated_tasks_list;
}


// Getter of the normal tasks list
const map<int, NormalTask*>& TaskManager::get_normal_tasks_list() const
{
    return *m_normal_tasks_list;
}


// Getter of the reminder tasks list
const vector<Reminder*>& TaskManager::get_reminder_tasks_list() const
{
    return *m_reminder_tasks_list;
}


// Getter of the important tasks list
const vector<NormalTask*>& TaskManager::get_important_tasks_list() const
{
    return *m_important_tasks_list;
}


//
void TaskManager::load_current_week_tasks_numbers(int const& current_year, int const& current_week_number, vector<int> *current_week_tasks)
{
    m_db_handler->DB_get_current_week_tasks_numbers(to_string(current_year), to_string(current_week_number), current_week_tasks);
}
/*
//
void TaskManager::load_current_week_tasks(int const& task_number)
{
    m_task_data_extraction_from_DB = new map<string, string>();

    m_db_handler->DB_task_data_retrieval(task_number, m_task_data_extraction_from_DB);

    if ( stoi((*m_task_data_extraction_from_DB)["PROCESSED"]) == 0 )
    {
        if ( stoi((*m_task_data_extraction_from_DB)["PERIODIC_TASK"]) == 0 )
        {
            m_one_time_task_list->insert(make_pair(stoi((*m_task_data_extraction_from_DB)["NUMBER"]), new Task(stoi((*m_task_data_extraction_from_DB)["NUMBER"]),
                                                                                                               QString::fromStdString((*m_task_data_extraction_from_DB)["NAME"]),
                                                                                                               QString::fromStdString((*m_task_data_extraction_from_DB)["IMPORTANCE"]),
                                                                                                               QDate(stoi((*m_task_data_extraction_from_DB)["YEAR"]),
                                                                                                                     stoi((*m_task_data_extraction_from_DB)["MONTH"]),
                                                                                                                     stoi((*m_task_data_extraction_from_DB)["DAY"])),
                                                                                                               stoi((*m_task_data_extraction_from_DB)["REMINDER"]),
                                                                                                               stoi((*m_task_data_extraction_from_DB)["WEEKS_BEFORE_TASK"]),
                                                                                                               QString::fromStdString((*m_task_data_extraction_from_DB)["COMMENTS"]))));
        }
    }

    delete m_task_data_extraction_from_DB;
    m_task_data_extraction_from_DB = nullptr;
}

//
const map<int, Task*>& TaskManager::get_one_time_task_list() const
{
    return *m_one_time_task_list;
}

//
void TaskManager::clear_current_week_tasks()
{
    m_one_time_task_list->clear();
}

//
void TaskManager::load_current_week_important_tasks_list(int const& current_year, int const& current_week_number, vector<int> *current_week_important_tasks_list)
{
    m_db_handler->DB_get_current_week_important_tasks_numbers(to_string(current_year), to_string(current_week_number), current_week_important_tasks_list);
}

//
void TaskManager::load_current_week_important_tasks(int const& task_number)
{
    m_task_data_extraction_from_DB = new map<string, string>();

    m_db_handler->DB_task_data_retrieval(task_number, m_task_data_extraction_from_DB);

    if ( stoi((*m_task_data_extraction_from_DB)["PROCESSED"]) == 0 )
    {
        m_important_tasks_list->insert(make_pair(stoi((*m_task_data_extraction_from_DB)["NUMBER"]), new Task(stoi((*m_task_data_extraction_from_DB)["NUMBER"]),
                                                                                                           QString::fromStdString((*m_task_data_extraction_from_DB)["NAME"]),
                                                                                                           QString::fromStdString((*m_task_data_extraction_from_DB)["IMPORTANCE"]),
                                                                                                           QDate(stoi((*m_task_data_extraction_from_DB)["YEAR"]),
                                                                                                                 stoi((*m_task_data_extraction_from_DB)["MONTH"]),
                                                                                                                 stoi((*m_task_data_extraction_from_DB)["DAY"])),
                                                                                                           stoi((*m_task_data_extraction_from_DB)["REMINDER"]),
                                                                                                           stoi((*m_task_data_extraction_from_DB)["WEEKS_BEFORE_TASK"]),
                                                                                                           QString::fromStdString((*m_task_data_extraction_from_DB)["COMMENTS"]))));
    }

    delete m_task_data_extraction_from_DB;
    m_task_data_extraction_from_DB = nullptr;
}

//
const map<int, Task*>& TaskManager::get_important_task_list() const
{
    return *m_important_tasks_list;
}

//
void TaskManager::clear_current_week_important_tasks()
{
    m_important_tasks_list->clear();
}

//
void TaskManager::load_reminder_tasks_numbers_list(int const& current_year, int const& current_week_number, vector<int> *reminder_tasks_numbers_list)
{
    map<int, map<string, string>> *reminder_tasks_data = new map<int, map<string, string>>();

    m_db_handler->DB_get_reminder_tasks_numbers(reminder_tasks_data);

    for (map<int, map<string, string>>::iterator rtd_iterator = reminder_tasks_data->begin(); rtd_iterator != reminder_tasks_data->end(); rtd_iterator++ )
    {
        if ( stoi(rtd_iterator->second["YEAR"]) != current_year or stoi(rtd_iterator->second["WEEK_NUMBER"]) != current_week_number )
        {
            QDate *tmp_date;

            tmp_date = new QDate(stoi(rtd_iterator->second["YEAR"]),
                                 stoi(rtd_iterator->second["MONTH"]),
                                 stoi(rtd_iterator->second["DAY"]));

            *tmp_date = tmp_date->addDays(-7 * stoi(rtd_iterator->second["WEEKS_BEFORE_TASK"]));

            if ( tmp_date->year() == current_year and tmp_date->weekNumber() == current_week_number )
            {
                reminder_tasks_numbers_list->push_back(rtd_iterator->first);
            }

            delete tmp_date;
            tmp_date = nullptr;
        }
    }

    delete reminder_tasks_data;
    reminder_tasks_data = nullptr;
}

//
void TaskManager::load_reminder_tasks_list(int const& task_number)
{
    m_task_data_extraction_from_DB = new map<string, string>();

    m_db_handler->DB_task_data_retrieval(task_number, m_task_data_extraction_from_DB);

    if ( stoi((*m_task_data_extraction_from_DB)["PROCESSED"]) == 0 )
    {
        QDate *tmp_date = new QDate(stoi((*m_task_data_extraction_from_DB)["YEAR"]),
                                    stoi((*m_task_data_extraction_from_DB)["MONTH"]),
                                    stoi((*m_task_data_extraction_from_DB)["DAY"]));

        *tmp_date = tmp_date->addDays(-7 * stoi((*m_task_data_extraction_from_DB)["WEEKS_BEFORE_TASK"]));

        m_reminder_tasks_list->insert(make_pair(stoi((*m_task_data_extraction_from_DB)["NUMBER"]), new Task(stoi((*m_task_data_extraction_from_DB)["NUMBER"]),
                                                                                                           QString::fromStdString((*m_task_data_extraction_from_DB)["NAME"]),
                                                                                                           QString::fromStdString((*m_task_data_extraction_from_DB)["IMPORTANCE"]),
                                                                                                           QDate(tmp_date->year(), tmp_date->month(), tmp_date->day()),
                                                                                                           stoi((*m_task_data_extraction_from_DB)["REMINDER"]),
                                                                                                           stoi((*m_task_data_extraction_from_DB)["WEEKS_BEFORE_TASK"]),
                                                                                                           QString::fromStdString((*m_task_data_extraction_from_DB)["COMMENTS"]))));

        delete tmp_date;
        tmp_date = nullptr;
    }

    delete m_task_data_extraction_from_DB;
    m_task_data_extraction_from_DB = nullptr;
}

//
const map<int, Task*>& TaskManager::get_reminder_task_list() const
{
    return *m_reminder_tasks_list;
}

//
void TaskManager::clear_current_week_reminder_tasks()
{
    m_reminder_tasks_list->clear();
}

//
void TaskManager::load_current_week_periodic_tasks_numbers(int const& current_year, int const& current_week_number, std::vector<int> *current_week_periodic_tasks)
{
    m_db_handler->DB_get_current_week_periodic_tasks_numbers(current_week_periodic_tasks);
}

//
void TaskManager::clear_current_week_periodic_tasks()
{
    m_periodic_tasks_list->clear();
}

//
void TaskManager::load_current_week_periodic_tasks(int const& task_number)
{
    m_task_data_extraction_from_DB = new map<string, string>();

    m_db_handler->DB_task_data_retrieval(task_number, m_task_data_extraction_from_DB);

    if ( QString::fromStdString((*m_task_data_extraction_from_DB)["PERIODICITY_TYPE"]) == "" )
    {

    }

    m_periodic_tasks_list->insert(make_pair(stoi((*m_task_data_extraction_from_DB)["NUMBER"]), new PeriodicTask(stoi((*m_task_data_extraction_from_DB)["NUMBER"]),
                                                                                                               QString::fromStdString((*m_task_data_extraction_from_DB)["NAME"]),
                                                                                                               QString::fromStdString((*m_task_data_extraction_from_DB)["IMPORTANCE"]),
                                                                                                               QDate(stoi((*m_task_data_extraction_from_DB)["YEAR"]),
                                                                                                                     stoi((*m_task_data_extraction_from_DB)["MONTH"]),
                                                                                                                     stoi((*m_task_data_extraction_from_DB)["DAY"])),
                                                                                                               stoi((*m_task_data_extraction_from_DB)["REMINDER"]),
                                                                                                               stoi((*m_task_data_extraction_from_DB)["WEEKS_BEFORE_TASK"]),
                                                                                                               QString::fromStdString((*m_task_data_extraction_from_DB)["COMMENTS"]),
                                                                                                               stoi((*m_task_data_extraction_from_DB)["PERIODICITY"]))));

    delete m_task_data_extraction_from_DB;
    m_task_data_extraction_from_DB = nullptr;
}

//
const map<int, PeriodicTask*>& TaskManager::get_periodic_task_list() const
{
    return *m_periodic_tasks_list;
}
*/
