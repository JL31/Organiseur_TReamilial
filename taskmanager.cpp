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


// Other methods
// -------------

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
    // variable initialization
    int last_added_task_number;

    // addition of the task into the DB
    last_added_task_number = m_db_handler.DB_task_addition(name.toUtf8().constData(),
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

    if ( is_periodic )
    {
        m_db_handler.DB_periodic_sub_tasks_table_creation(to_string(last_added_task_number));
    }
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

    if ( is_periodic )
    {
        m_db_handler.DB_periodic_sub_tasks_table_creation(to_string(number));
    }
}


// Method to retrieve data from database
void TaskManager::data_retrieval(int const& number, map<string, string> *data_from_DB)
{
    m_db_handler.DB_task_data_retrieval(number, data_from_DB);
}

// Method to validate a task into the database
void TaskManager::validate_task(int const& task_number, int const& sub_task_number)
{
    if ( sub_task_number == 0 )
    {
        m_db_handler.DB_task_validation(to_string(task_number));
    }
    else
    {
        m_db_handler.DB_sub_task_validation(to_string(task_number),
                                            to_string(sub_task_number));
    }
}

// Method to load othe several types of tasks for the current week
void TaskManager::load_current_week_data(QDate const& current_week_date)
{
    // variables initialization
    int current_year = current_week_date.year();
    int current_week_number = current_week_date.weekNumber();

    // prior cleaning
    task_lists_cleaning();

    // several types tasks loading
    load_non_dated_tasks();
    load_normal_tasks(current_year, current_week_number);
    load_periodic_tasks(current_week_date);
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

    // normal tasks list cleaning
    m_periodic_tasks_list.clear();

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

// Method to add, into memory, a task and its sub tasks
void TaskManager::add_periodic_task_and_sub_tasks(map<string, string> periodic_task_data,
                                                  QDate const& current_week_date,
                                                  QDate const& recalculated_date)
{
    // variable initialization
    vector<map<string, string>>* sub_tasks_extraction_from_DB = new vector<map<string, string>>();

    // data retrieval into database
    m_db_handler.DB_get_periodic_task_sub_tasks(periodic_task_data["NUMBER"], sub_tasks_extraction_from_DB);

    // there are NO sub tasks associated to the current task
    if ( sub_tasks_extraction_from_DB->empty() )
    {
        // variable initialization
        int last_added_sub_task_number;

        last_added_sub_task_number = m_db_handler.add_sub_task_to_periodic_task(to_string(recalculated_date.day()),
                                                                                to_string(recalculated_date.month()),
                                                                                to_string(recalculated_date.year()),
                                                                                to_string(recalculated_date.weekNumber()),
                                                                                periodic_task_data["NUMBER"]);

        m_periodic_tasks_list.insert(make_pair(stoi(periodic_task_data["NUMBER"]),
                                               PeriodicTask(stoi(periodic_task_data["NUMBER"]),
                                                            QString::fromStdString(periodic_task_data["NAME"]),
                                                            QString::fromStdString(periodic_task_data["COMMENTS"]),
                                                            stoi(periodic_task_data["IS_IMPORTANT"]),
                                                            stoi(periodic_task_data["IS_PROCESSED"]),
                                                            stoi(periodic_task_data["IS_DATED"]),
                                                            QDate(stoi(periodic_task_data["YEAR"]),
                                                                  stoi(periodic_task_data["MONTH"]),
                                                                  stoi(periodic_task_data["DAY"])),
                                                            stoi(periodic_task_data["REMINDER"]),
                                                            stoi(periodic_task_data["WEEKS_BEFORE_TASK"]),
                                                            stoi(periodic_task_data["IS_PERIODIC"]),
                                                            stoi(periodic_task_data["PERIODICITY"])
                                                           )
                                              )
                                    );

        m_periodic_tasks_list[stoi(periodic_task_data["NUMBER"])].add_sub_task(last_added_sub_task_number,
                                                                               recalculated_date,
                                                                               0);
    }
    // there are sub tasks associated to the current task
    else
    {
        // variable initialization
        bool current_periodic_sub_task_exists(false);

        // iteration over the sub tasks
        for ( auto st_it = sub_tasks_extraction_from_DB->begin(); st_it != sub_tasks_extraction_from_DB->end(); st_it++ )
        {
            // sub task date definition
            QDate sub_task_date = QDate(stoi((*st_it)["YEAR"]),
                                        stoi((*st_it)["MONTH"]),
                                        stoi((*st_it)["DAY"]));

            if ( ( sub_task_date.weekNumber() == current_week_date.weekNumber() ) and ( stoi((*st_it)["IS_PROCESSED"]) != 1 ) )
            {
                m_periodic_tasks_list.insert(make_pair(stoi(periodic_task_data["NUMBER"]),
                                                       PeriodicTask(stoi(periodic_task_data["NUMBER"]),
                                                                    QString::fromStdString(periodic_task_data["NAME"]),
                                                                    QString::fromStdString(periodic_task_data["COMMENTS"]),
                                                                    stoi(periodic_task_data["IS_IMPORTANT"]),
                                                                    stoi(periodic_task_data["IS_PROCESSED"]),
                                                                    stoi(periodic_task_data["IS_DATED"]),
                                                                    QDate(stoi(periodic_task_data["YEAR"]),
                                                                          stoi(periodic_task_data["MONTH"]),
                                                                          stoi(periodic_task_data["DAY"])),
                                                                    stoi(periodic_task_data["REMINDER"]),
                                                                    stoi(periodic_task_data["WEEKS_BEFORE_TASK"]),
                                                                    stoi(periodic_task_data["IS_PERIODIC"]),
                                                                    stoi(periodic_task_data["PERIODICITY"])
                                                                   )
                                                      )
                                            );

                m_periodic_tasks_list[stoi(periodic_task_data["NUMBER"])].add_sub_task(stoi((*st_it)["NUMBER"]),
                                                                          sub_task_date,
                                                                          0);

                current_periodic_sub_task_exists = true;
            }
        }

        // if there are no sub tasks with the current date so it must be created
        if ( not current_periodic_sub_task_exists )
        {
            // variable initialization
            int last_added_sub_task_number;

            last_added_sub_task_number = m_db_handler.add_sub_task_to_periodic_task(to_string(recalculated_date.day()),
                                                                                    to_string(recalculated_date.month()),
                                                                                    to_string(recalculated_date.year()),
                                                                                    to_string(recalculated_date.weekNumber()),
                                                                                    periodic_task_data["NUMBER"]);

            m_periodic_tasks_list.insert(make_pair(stoi(periodic_task_data["NUMBER"]),
                                                   PeriodicTask(stoi(periodic_task_data["NUMBER"]),
                                                                QString::fromStdString(periodic_task_data["NAME"]),
                                                                QString::fromStdString(periodic_task_data["COMMENTS"]),
                                                                stoi(periodic_task_data["IS_IMPORTANT"]),
                                                                stoi(periodic_task_data["IS_PROCESSED"]),
                                                                stoi(periodic_task_data["IS_DATED"]),
                                                                QDate(stoi(periodic_task_data["YEAR"]),
                                                                      stoi(periodic_task_data["MONTH"]),
                                                                      stoi(periodic_task_data["DAY"])),
                                                                stoi(periodic_task_data["REMINDER"]),
                                                                stoi(periodic_task_data["WEEKS_BEFORE_TASK"]),
                                                                stoi(periodic_task_data["IS_PERIODIC"]),
                                                                stoi(periodic_task_data["PERIODICITY"])
                                                               )
                                                  )
                                        );

            m_periodic_tasks_list[stoi(periodic_task_data["NUMBER"])].add_sub_task(last_added_sub_task_number,
                                                                                   recalculated_date,
                                                                                   0);
        }
    }

    // variable cleaning;
    delete sub_tasks_extraction_from_DB;
    sub_tasks_extraction_from_DB = nullptr;
}

// Method to load the periodic tasks
void TaskManager::load_periodic_tasks(QDate const& current_week_date)
{
    // attribute initialization
    m_data_extraction_from_DB = new vector<map<string, string>>();

    // data retrieval into database
    m_db_handler.DB_load_periodic_tasks(m_data_extraction_from_DB);

    // iteration over the periodic tasks
    for ( auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++ )
    {
        // current periodic task computation
        QDate current_periodic_task_date = QDate(stoi((*it)["YEAR"]), stoi((*it)["MONTH"]), stoi((*it)["DAY"]));

        // current date is after periodic task date
        if ( ( current_week_date.weekNumber() > current_periodic_task_date.weekNumber() ) and ( current_week_date.year() == current_periodic_task_date.year() ) )
        {
            // variables initialization
            int i = 1;
            QDate recalculated_date;
            int recalculated_week_number;
            int recalculated_year ;

            // date recalculations
            recalculated_date = current_periodic_task_date.addDays(7 * stoi((*it)["PERIODICITY"]) * i);
            recalculated_week_number = recalculated_date.weekNumber();
            recalculated_year = recalculated_date.year();

            // Iteration while recalculated date is lower than current date
            while ( recalculated_week_number <= current_week_date.weekNumber() )
            {
                if ( recalculated_week_number == current_week_date.weekNumber() )
                {
                    add_periodic_task_and_sub_tasks(*it, current_week_date, recalculated_date);
                }

                // index incrementation and date recalculations
                i++;
                recalculated_date = current_periodic_task_date.addDays(7 * stoi((*it)["PERIODICITY"]) * i);
                recalculated_week_number = recalculated_date.weekNumber();
                recalculated_year = recalculated_date.year();
            }
        }
        // current date is periodic task date
        else if ( ( current_week_date.weekNumber() == current_periodic_task_date.weekNumber() ) and ( current_week_date.year() == current_periodic_task_date.year() ) )
        {
            QDate recalculated_date = current_periodic_task_date;
            add_periodic_task_and_sub_tasks(*it, current_week_date, recalculated_date);
        }
    }
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

    // iteration over the DB extracted data
    for (auto it = m_data_extraction_from_DB->begin(); it != m_data_extraction_from_DB->end(); it++)
    {
        QDate tmp_date = QDate(stoi((*it)["YEAR"]), stoi((*it)["MONTH"]), stoi((*it)["DAY"]));
        tmp_date = tmp_date.addDays(-7 * stoi((*it)["WEEKS_BEFORE_TASK"]));

        if ( tmp_date.year() == current_year and tmp_date.weekNumber() == current_week_number )
        {
            tmp_reminder_tasks_numbers.push_back(stoi((*it)["NUMBER"]));
        }
    }

    // iteration over the reminder tasks numbers to add items to reminder tasks list
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

    /*
     * // ...
    for (auto it = m_periodic_tasks_list.begin(); it != m_periodic_tasks_list.end(); it++)
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
    */

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

map<int, PeriodicTask>& TaskManager::get_periodic_tasks_list()
{
    return m_periodic_tasks_list;
}

vector<Reminder>& TaskManager::get_reminder_tasks_list()
{
    return m_reminder_tasks_list;
}

vector<BaseTask>& TaskManager::get_important_tasks_list()
{
    return m_important_tasks_list;
}
