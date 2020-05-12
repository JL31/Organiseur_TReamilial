#include "taskmanager.h"

using namespace std;

// Constructor and destructor
// --------------------------

// Constructor
TaskManager::TaskManager(QString const& database_complete_path,
                         bool const& database_folder_automatically_initiated) : m_non_dated_tasks_list(map<int, NonDatedTask>()),
                                                                                m_normal_tasks_list(map<int, NormalTask>()),
                                                                                m_periodic_tasks_list(map<int, PeriodicTask>()),
                                                                                m_reminder_tasks_list(vector<Reminder>()),
                                                                                m_important_tasks_list(vector<BaseTask>()),
                                                                                m_db_handler(DatabaseHandler(database_complete_path.toStdString())),
                                                                                m_database_folder_automatically_initiated(database_folder_automatically_initiated)
{
    // if the database has been automatically initialiazed by the user some tables must be created
    if ( m_database_folder_automatically_initiated )
    {
        m_db_handler.DB_initialization();
    }
}

// Destructor
TaskManager::~TaskManager()
{}


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
    // addition of the task into the DB
    m_db_handler.DB_task_addition(name.toUtf8().constData(),
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
    m_db_handler.DB_task_modification(to_string(number),
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
    m_db_handler.DB_task_data_retrieval(number, data_from_DB);
}

//
void TaskManager::validate_task(int const& number)
{
    m_db_handler.DB_task_validation(to_string(number));
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
    // non dated tasks list cleaning
    m_non_dated_tasks_list.clear();

    // normal tasks list cleaning
    m_normal_tasks_list.clear();

    //m_periodic_tasks_list->clear();

    // important tasks list cleaning
    m_important_tasks_list.clear();

    // reminder tasks list cleaning
    m_reminder_tasks_list.clear();
}


// Method to load the non dated tasks
void TaskManager::load_non_dated_tasks()
{
    // attribute initialization
    m_data_extraction_from_DB = new vector<map<string, string>>();

    // data retrieval into database
    m_db_handler.DB_load_non_dated_tasks(m_data_extraction_from_DB);

    // non dated tasks list filling-in with data extracted from database
    for (auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++)
    {
        m_non_dated_tasks_list.insert(make_pair(stoi((*it)["NUMBER"]),
                                                NonDatedTask(stoi((*it)["NUMBER"]),
                                                             QString::fromStdString((*it)["NAME"]),
                                                             QString::fromStdString((*it)["COMMENTS"]),
                                                             stoi((*it)["IS_IMPORTANT"]),
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
    m_db_handler.DB_load_normal_tasks(to_string(current_year), to_string(current_week_number), m_data_extraction_from_DB);

    // non dated tasks list filling-in with data extracted from database
    for (auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++)
    {
        m_normal_tasks_list.insert(make_pair(stoi((*it)["NUMBER"]),
                                             NormalTask(stoi((*it)["NUMBER"]),
                                                        QString::fromStdString((*it)["NAME"]),
                                                        QString::fromStdString((*it)["COMMENTS"]),
                                                        stoi((*it)["IS_IMPORTANT"]),
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
    m_db_handler.DB_prior_step_for_reminder_tasks_loading(m_data_extraction_from_DB);

    // variables initialization
    vector<int> tmp_reminder_tasks_numbers;

    // ...
    for (auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++)
    {
        QDate tmp_date = QDate(stoi((*it)["YEAR"]), stoi((*it)["MONTH"]), stoi((*it)["DAY"]));
        tmp_date = tmp_date.addDays(-7 * stoi((*it)["WEEKS_BEFORE_TASK"]));

        if ( tmp_date.year() == current_year and tmp_date.weekNumber() == current_week_number )
        {
            tmp_reminder_tasks_numbers.push_back(stoi((*it)["NUMBER"]));
        }
    }

    // ...
    for ( auto it = tmp_reminder_tasks_numbers.begin(); it != tmp_reminder_tasks_numbers.end(); it++ )
    {
        // variables initialization
        map<string, string> *m_reminder_data_extraction_from_DB = new map<string, string>();

        // reminder data retrieval into database
        m_db_handler.DB_reminder_task_loading_from_reminder_task_number(to_string(*it), m_reminder_data_extraction_from_DB);

        // creation of a Reminder instance and addition of this instance to the reminder tasks list attribute
        m_reminder_tasks_list.push_back(Reminder(stoi((*m_reminder_data_extraction_from_DB)["NUMBER"]),
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

    // attributes cleaning
    delete m_data_extraction_from_DB;
    m_data_extraction_from_DB = nullptr;
}

// Method to load the important tasks
void TaskManager::load_important_tasks_list(int const& current_year, int const& current_week_number)
{
    // creation of a BaseTask instance and addition of this instance to the important tasks list attribute
    for (auto it = m_normal_tasks_list.begin(); it != m_normal_tasks_list.end(); it++)
    {
        if ( ( it->second.get_date().year() == current_year ) and
             ( it->second.get_date().weekNumber() == current_week_number ) and
               it->second.get_is_important() )
        {
            m_important_tasks_list.push_back(BaseTask(it->second.get_number(),
                                                      it->second.get_name(),
                                                      it->second.get_comments()
                                                     )
                                            );
        }
    }

    // important tasks list filling-in with important non dated tasks
    for (auto it = m_non_dated_tasks_list.begin(); it != m_non_dated_tasks_list.end(); it++)
    {
        if ( it->second.get_is_important() )
        {
            m_important_tasks_list.push_back(BaseTask(it->second.get_number(),
                                                      it->second.get_name(),
                                                      it->second.get_comments()
                                                     )
                                            );
        }
    }
}


// Getters
// -------

map<int, NonDatedTask>& TaskManager::get_non_dated_tasks_list()
{
    return m_non_dated_tasks_list;
}

map<int, NormalTask>& TaskManager::get_normal_tasks_list()
{
    return m_normal_tasks_list;
}

vector<Reminder>& TaskManager::get_reminder_tasks_list()
{
    return m_reminder_tasks_list;
}

vector<BaseTask>& TaskManager::get_important_tasks_list()
{
    return m_important_tasks_list;
}
