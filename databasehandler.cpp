#include "databasehandler.h"

#include <string>

#include <QtWidgets>

using namespace std;

// Constructeur
DatabaseHandler::DatabaseHandler() :
                                 m_database_name("/home/tream/Bureau/Developpements/Langage_cpp/Test_organiseur/database/test.db"),
                                 m_counter(new int(0))
{}

// Destructeur
DatabaseHandler::~DatabaseHandler()
{
    delete m_zErrMsg;
    delete m_counter;

    m_zErrMsg = nullptr;
    m_counter = nullptr;
}

// DB_get_number_of_tasks_callback method callback
static int DB_get_number_of_tasks_callback(void *counter, int argc, char **argv, char **azColName)
{
    int *c = (int *)counter;
    *c = atoi(argv[0]);

    return 0;
}

// Count the number of tasks
int DatabaseHandler::DB_get_number_of_tasks()
{
    // DB opening
    DB_opening();

    // DB counting
    m_sql_request = new string("SELECT COUNT(*) FROM TASK_TABLE;");

    m_rc = sqlite3_exec(m_database, m_sql_request->c_str(), DB_get_number_of_tasks_callback, m_counter, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération du numéro des tâches", "Problème");
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;

    // method return
    return *m_counter;
}

// Ouverture DB
void DatabaseHandler::DB_opening()
{
    m_rc = sqlite3_open(m_database_name.c_str(), &m_database);

    if (m_rc != SQLITE_OK)
    {
        QMessageBox::information(nullptr, "Ouverture de la DB", "Problème lors de l'ouverture de la DB");
    }
}

// DB_task_addition method callback
static int DB_task_addition_callback(void *dummy, int argc, char **argv, char **azColName)
{
    return 0;
}

// DB_task_addition method
void DatabaseHandler::DB_task_addition(std::string const& new_nbr_of_tasks,
                                       std::string const& task_name,
                                       std::string const& task_importance,
                                       std::string const& year,
                                       std::string const& month,
                                       std::string const& day,
                                       std::string const& week_number,
                                       std::string const& comments,
                                       std::string const& reminder,
                                       std::string const& nbr_weeks_before_task,
                                       std::string const& periodic_task_state,
                                       std::string const& task_periodicity)
{
    // request initialization
    m_sql_request = new string();

    m_sql_request->append("INSERT INTO TASK_TABLE (NUMBER, NAME, IMPORTANCE, YEAR, MONTH, DAY, WEEK_NUMBER, COMMENTS, PROCESSED, REMINDER, WEEKS_BEFORE_TASK, PERIODIC_TASK, PERIODICITY)\n");
    m_sql_request->append("VALUES (");
    m_sql_request->append(new_nbr_of_tasks);
    m_sql_request->append(", \'");
    m_sql_request->append(task_name);
    m_sql_request->append("\', \'");
    m_sql_request->append(task_importance);
    m_sql_request->append("\', ");
    m_sql_request->append(year);
    m_sql_request->append(", ");
    m_sql_request->append(month);
    m_sql_request->append(", ");
    m_sql_request->append(day);
    m_sql_request->append(", ");
    m_sql_request->append(week_number);
    m_sql_request->append(", \'");
    m_sql_request->append(comments);
    m_sql_request->append("\', 0, ");   // processed is 0 for task creation
    m_sql_request->append(reminder);
    m_sql_request->append(", ");
    m_sql_request->append(nbr_weeks_before_task);
    m_sql_request->append(", ");
    m_sql_request->append(periodic_task_state);
    m_sql_request->append(", \'");
    m_sql_request->append(task_periodicity);
    m_sql_request->append("\');");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request->c_str(), DB_task_addition_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Ajout d'une tâche dans la DB", "Problème écriture dans la DB");
        sqlite3_free(m_zErrMsg);
    }
    else
    {
        QMessageBox::information(nullptr, "Ajout d'une tâche dans la DB", "La tâche a bien été ajoutée à la DB");
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// DB_task_modification method
void DatabaseHandler::DB_task_modification(string const& task_number,
                                           string const& task_name,
                                           string const& task_importance,
                                           string const& year,
                                           string const& month,
                                           string const& day,
                                           string const& week_number,
                                           string const& comments,
                                           string const& reminder,
                                           string const& nbr_weeks_before_task,
                                           string const& periodic_task_state,
                                           string const& task_periodicity)
{
    // request initialization
    m_sql_request = new string();

    m_sql_request->append("UPDATE TASK_TABLE\n");
    m_sql_request->append("SET NAME = \'");
    m_sql_request->append(task_name);
    m_sql_request->append("\',\n");
    m_sql_request->append("    IMPORTANCE = \'");
    m_sql_request->append(task_importance);
    m_sql_request->append("\',\n");
    m_sql_request->append("    YEAR = ");
    m_sql_request->append(year);
    m_sql_request->append(",\n");
    m_sql_request->append("    MONTH = ");
    m_sql_request->append(month);
    m_sql_request->append(",\n");
    m_sql_request->append("    DAY = ");
    m_sql_request->append(day);
    m_sql_request->append(",\n");
    m_sql_request->append("    WEEK_NUMBER = ");
    m_sql_request->append(week_number);
    m_sql_request->append(",\n");
    m_sql_request->append("    COMMENTS = \'");
    m_sql_request->append(comments);
    m_sql_request->append("\',\n");
    m_sql_request->append("    PROCESSED = 0,\n");    // processed is 0 for task modification
    m_sql_request->append("    REMINDER = ");
    m_sql_request->append(reminder);
    m_sql_request->append(",\n");
    m_sql_request->append("    WEEKS_BEFORE_TASK = ");
    m_sql_request->append(nbr_weeks_before_task);
    m_sql_request->append(",\n");
    m_sql_request->append("    PERIODIC_TASK = ");
    m_sql_request->append(periodic_task_state);
    m_sql_request->append(",\n");
    m_sql_request->append("    PERIODICITY = \'");
    m_sql_request->append(task_periodicity);
    m_sql_request->append("\'\n");
    m_sql_request->append("WHERE\n");
    m_sql_request->append("    NUMBER = ");
    m_sql_request->append(task_number);
    m_sql_request->append(";");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, (*m_sql_request).c_str(), DB_task_addition_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Modification d'une tâche dans la DB", "Problème de mise-à-jour de la DB");
        sqlite3_free(m_zErrMsg);
    }
    else
    {
        QMessageBox::information(nullptr, "Modification d'une tâche dans la DB", "Les données ont bien été mise-à-jour");
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// DB_task_data_retrieval_callback method callback
static int DB_task_data_retrieval_callback(void *data_from_DB, int argc, char **argv, char **azColName)
{
    map<string, string> *c = (map<string, string> *) data_from_DB;

    for (int i(0); i < argc; i++)
    {
        //c->operator[](azColName[i]) = argv[i];
        c->insert(make_pair(azColName[i], argv[i]));
    }

    return 0;
}

// Method to retrieve data from DB
void DatabaseHandler::DB_task_data_retrieval(int const& task_number, map<string, string> *data_from_DB)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = new string();

    m_sql_request->append("SELECT * FROM TASK_TABLE WHERE NUMBER = ");
    m_sql_request->append(to_string(task_number));
    m_sql_request->append(";");

    // DB data retrieval
    m_rc = sqlite3_exec(m_database, m_sql_request->c_str(), DB_task_data_retrieval_callback, (void*) data_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération de données dans la DB", "Problème");
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// Method to validate a task in the DB (PROCESSED = 1)
void DatabaseHandler::DB_task_validation(string const& task_number)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = new string();

    m_sql_request->append("UPDATE TASK_TABLE\n");
    m_sql_request->append("SET PROCESSED = 1\n");
    m_sql_request->append("WHERE\n");
    m_sql_request->append("    NUMBER = ");
    m_sql_request->append(task_number);
    m_sql_request->append(";");

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, (*m_sql_request).c_str(), DB_task_addition_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Validation d'une tâche", "Problème");
        sqlite3_free(m_zErrMsg);
    }
    else
    {
        QMessageBox::information(nullptr, "Validation d'une tâche", "La tâche a bien été validée");
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// DB_get_current_week_tasks_numbers method callback
static int DB_get_current_week_tasks_numbers_callback(void *current_week_tasks, int argc, char **argv, char **azColName)
{
    vector<int> *c = (vector<int> *) current_week_tasks;

    for (int i(0); i < argc; i++)
    {
        c->push_back(stoi(argv[i]));
    }

    return 0;
}

//
void DatabaseHandler::DB_get_current_week_tasks_numbers(string const& current_year, string const& current_week_number, vector<int> *current_week_tasks)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = new string();

    m_sql_request->append("SELECT NUMBER FROM TASK_TABLE\n");
    m_sql_request->append("WHERE\n");
    m_sql_request->append("    YEAR = ");
    m_sql_request->append(current_year);
    m_sql_request->append("\n");
    m_sql_request->append("AND WEEK_NUMBER = ");
    m_sql_request->append(current_week_number);
    m_sql_request->append(";");

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, (*m_sql_request).c_str(), DB_get_current_week_tasks_numbers_callback, (void*) current_week_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches de la semaine courante", "Problème");
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// DB_get_current_week_tasks_numbers method callback
static int DB_get_current_week_important_tasks_numbers_callback(void *current_week_important_tasks, int argc, char **argv, char **azColName)
{
    vector<int> *c = (vector<int> *) current_week_important_tasks;

    for (int i(0); i < argc; i++)
    {
        c->push_back(stoi(argv[i]));
    }

    return 0;
}

//
void DatabaseHandler::DB_get_current_week_important_tasks_numbers(std::string const& current_year, std::string const& current_week_number, std::vector<int> *current_week_important_tasks)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = new string();

    m_sql_request->append("SELECT NUMBER FROM TASK_TABLE\n");
    m_sql_request->append("WHERE\n");
    m_sql_request->append("    YEAR = ");
    m_sql_request->append(current_year);
    m_sql_request->append("\n");
    m_sql_request->append("AND WEEK_NUMBER = ");
    m_sql_request->append(current_week_number);
    m_sql_request->append("\n");
    m_sql_request->append("AND IMPORTANCE = 'Importante';");

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, (*m_sql_request).c_str(), DB_get_current_week_important_tasks_numbers_callback, (void*) current_week_important_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches importantes de la semaine courante", "Problème");
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// DB_get_current_week_tasks_numbers method callback
static int DB_get_reminder_tasks_numbers_callback(void *reminder_tasks_data, int argc, char **argv, char **azColName)
{
    map<int, map<string, string>> *c = (map<int, map<string, string>> *) reminder_tasks_data;
    map<string, string> tmp_map;

    for (int i(1); i < argc; i++)   // starts at 1 and not 0 because we consider that 0 corresponds to the NUMBER database entry
    {
        tmp_map.insert(make_pair(azColName[i], argv[i]));
    }

    c->insert(make_pair(stoi(argv[0]), tmp_map));

    return 0;
}

//
void DatabaseHandler::DB_get_reminder_tasks_numbers(map<int, map<string, string>> *reminder_tasks_data)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = new string();
    m_sql_request->append("SELECT NUMBER, YEAR, MONTH, DAY, WEEK_NUMBER, WEEKS_BEFORE_TASK FROM TASK_TABLE WHERE REMINDER = 1;");

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, (*m_sql_request).c_str(), DB_get_reminder_tasks_numbers_callback, (void*) reminder_tasks_data, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches possédant un rappel", "Problème");
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// DB_get_current_week_tasks_numbers method callback
static int DB_get_current_week_periodic_tasks_numbers_callback(void *current_week_periodic_tasks, int argc, char **argv, char **azColName)
{
    vector<int> *c = (vector<int> *) current_week_periodic_tasks;

    for (int i(0); i < argc; i++)
    {
        c->push_back(stoi(argv[i]));
    }

    return 0;
}

//
void DatabaseHandler::DB_get_current_week_periodic_tasks_numbers(vector<int> *current_week_periodic_tasks)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = new string();

    m_sql_request->append("SELECT NUMBER FROM TASK_TABLE WHERE PERIODIC_TASK = 1;");

    // ...
    m_rc = sqlite3_exec(m_database, (*m_sql_request).c_str(), DB_get_current_week_periodic_tasks_numbers_callback, (void*) current_week_periodic_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches périodiques de la semaine courante", "Problème");
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // attributes cleaning
    delete m_sql_request;
    m_sql_request = nullptr;
}

// Fermeture DB
void DatabaseHandler::DB_closing()
{
    sqlite3_close(m_database);
}
