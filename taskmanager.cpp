#include "taskmanager.h"

#include <QtWidgets>

using namespace std;

// Constructor
TaskManager::TaskManager() :
                         m_one_time_task_list(new map<int, Task*>()),
                         m_periodic_task_list(new map<int, PeriodicTask*>()),
                         m_important_task_list(new map<int, Task*>()),
                         m_reminder_task_list(new map<int, Task*>()),
                         m_number_of_tasks(new int(0)),
                         m_db_handler(new DatabaseHandler()),
                         m_week_number(new int(0))
{}

// Destructor
TaskManager::~TaskManager()
{
    // free memory
    delete m_one_time_task_list;
    delete m_periodic_task_list;
    delete m_important_task_list;
    delete m_reminder_task_list;
    delete m_number_of_tasks;
    delete m_db_handler;

    // ...
    m_one_time_task_list = nullptr;
    m_periodic_task_list = nullptr;
    m_important_task_list = nullptr;
    m_reminder_task_list = nullptr;
    m_number_of_tasks = nullptr;
    m_db_handler = nullptr;
}

// Getters
int TaskManager::get_number_of_tasks() const
{
    return *m_number_of_tasks;
}

// DB task addition
void TaskManager::add_task(QString const& task_name,
                           QString const& task_importance,
                           QDate const& task_date,
                           QString const& comments,
                           bool const& reminder,
                           int const& nbr_weeks_before_task,
                           bool const& periodic_task_state,
                           int const& task_periodicity
                           )
{
    // calculation of the task number
    int *new_nbr_of_tasks;
    new_nbr_of_tasks = new int(m_db_handler->DB_get_number_of_tasks() + 1);

    // week number calculation
    *m_week_number = task_date.weekNumber();

    m_db_handler->DB_task_addition(to_string(*new_nbr_of_tasks),
                                   task_name.toUtf8().constData(),
                                   task_importance.toUtf8().constData(),
                                   to_string(task_date.year()),
                                   to_string(task_date.month()),
                                   to_string(task_date.day()),
                                   to_string(*m_week_number),
                                   comments.toUtf8().constData(),
                                   to_string(reminder),
                                   to_string(nbr_weeks_before_task),
                                   to_string(periodic_task_state),
                                   to_string(task_periodicity));

    //cleaning
    delete new_nbr_of_tasks;
    new_nbr_of_tasks = nullptr;
}

// DB task addition
void TaskManager::modify_task(int const& task_number,
                              QString const& task_name,
                              QString const& task_importance,
                              QDate const& task_date,
                              QString const& comments,
                              bool const& reminder,
                              int const& nbr_weeks_before_task,
                              bool const& periodic_task_state,
                              int const& task_periodicity)
{
    // week number calculation
    *m_week_number = task_date.weekNumber();

    m_db_handler->DB_task_modification(to_string(task_number),
                                       task_name.toUtf8().constData(),
                                       task_importance.toUtf8().constData(),
                                       to_string(task_date.year()),
                                       to_string(task_date.month()),
                                       to_string(task_date.day()),
                                       to_string(*m_week_number),
                                       comments.toUtf8().constData(),
                                       to_string(reminder),
                                       to_string(nbr_weeks_before_task),
                                       to_string(periodic_task_state),
                                       to_string(task_periodicity));
}

//
void TaskManager::data_retrieval(int const& task_number, map<string, string> *data_from_DB)
{
    m_db_handler->DB_task_data_retrieval(task_number, data_from_DB);
}

//
void TaskManager::validate_task(int const& task_number)
{
    m_db_handler->DB_task_validation(to_string(task_number));
}

//
void TaskManager::load_current_week_tasks_numbers(int const& current_year, int const& current_week_number, vector<int> *current_week_tasks)
{
    m_db_handler->DB_get_current_week_tasks_numbers(to_string(current_year), to_string(current_week_number), current_week_tasks);
}

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
        m_important_task_list->insert(make_pair(stoi((*m_task_data_extraction_from_DB)["NUMBER"]), new Task(stoi((*m_task_data_extraction_from_DB)["NUMBER"]),
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
    return *m_important_task_list;
}

//
void TaskManager::clear_current_week_important_tasks()
{
    m_important_task_list->clear();
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

        m_reminder_task_list->insert(make_pair(stoi((*m_task_data_extraction_from_DB)["NUMBER"]), new Task(stoi((*m_task_data_extraction_from_DB)["NUMBER"]),
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
    return *m_reminder_task_list;
}

//
void TaskManager::clear_current_week_reminder_tasks()
{
    m_reminder_task_list->clear();
}

//
void TaskManager::load_current_week_periodic_tasks_numbers(int const& current_year, int const& current_week_number, std::vector<int> *current_week_periodic_tasks)
{
    m_db_handler->DB_get_current_week_periodic_tasks_numbers(current_week_periodic_tasks);
}

//
void TaskManager::clear_current_week_periodic_tasks()
{
    m_periodic_task_list->clear();
}

//
void TaskManager::load_current_week_periodic_tasks(int const& task_number)
{
    m_task_data_extraction_from_DB = new map<string, string>();

    m_db_handler->DB_task_data_retrieval(task_number, m_task_data_extraction_from_DB);

    if ( QString::fromStdString((*m_task_data_extraction_from_DB)["PERIODICITY_TYPE"]) == "" )
    {

    }

    m_periodic_task_list->insert(make_pair(stoi((*m_task_data_extraction_from_DB)["NUMBER"]), new PeriodicTask(stoi((*m_task_data_extraction_from_DB)["NUMBER"]),
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
    return *m_periodic_task_list;
}
