#include "databasehandler.h"

#include <QtWidgets>

using namespace std;

// Constructor and destructor
// --------------------------

// Constructor
DatabaseHandler::DatabaseHandler(string const& database_complete_path) : m_database_name(database_complete_path + "/ot_databases.db")
{}


// Destructor
DatabaseHandler::~DatabaseHandler()
{}


// Other methods
// -------------

// Dummy callback method
static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   return 0;
}


// TASKS table initialization method
void DatabaseHandler::DB_initialization()
{
    // request initialization
    m_sql_request = string();

    m_sql_request.append("CREATE TABLE TASKS("
                         "NUMBER INTEGER PRIMARY KEY AUTOINCREMENT,"
                         "NAME TEXT NOT NULL,"
                         "IS_IMPORTANT INTEGER NOT NULL,"
                         "COMMENTS TEXT,"
                         "IS_DATED INTEGER NOT NULL,"
                         "DAY INTEGER NOT NULL,"
                         "MONTH INTEGER NOT NULL,"
                         "YEAR INTEGER NOT NULL,"
                         "WEEK_NUMBER INTEGER NOT NULL,"
                         "REMINDER INTEGER NOT NULL,"
                         "WEEKS_BEFORE_TASK INTEGER NOT NULL,"
                         "IS_PERIODIC INTEGER NOT NULL,"
                         "PERIODICITY INTEGER NOT NULL,"
                         "IS_PROCESSED INTEGER NOT NULL);");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), dummy_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Création de la table \"TASKS\" dans la DB", "Problème lors de la création de la table \"TASKS\"");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Database opening
void DatabaseHandler::DB_opening()
{
    m_rc = sqlite3_open(m_database_name.c_str(), &m_database);

    if (m_rc != SQLITE_OK)
    {
        QMessageBox::information(nullptr, "Ouverture de la DB", "Problème lors de l'ouverture de la DB");
    }
}


// DB_get_number_of_items_callback method callback
static int DB_get_number_of_items_callback(void *number_of_items, int argc, char **argv, char **azColName)
{
    int *c = (int *) number_of_items;
    *c = atoi(argv[0]);

    return 0;
}


// Method that returns the number of the last added task
int DatabaseHandler::DB_get_number_of_tasks()
{
    // variables initialization
    int number_of_tasks;

    // request initialization
    m_sql_request = string();
    m_sql_request.append("SELECT MAX(NUMBER) FROM TASKS;");

    // DB opening
    DB_opening();

    // DB counting
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_get_number_of_items_callback, &number_of_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération du nombre de tâches", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // method return
    return number_of_tasks;
}


// Function to replace the single quote by two quotes
void string_replacement(string& database_attribute)
{
    //
    //
    // voir comment adapter cette fonction :
    // l'idéal serait de passer aux méthodes "DB_task_addition" et "DB_task_modification"
    // les variables "name" et "comments" en références non constante (par exemple : string& name)
    // pour ce faire il faudra sans doute remonter jusqu'au getter de ces variables dans la classe
    // "IHMGestionDesTaches" afin de modifier le type de retour des getters associés à ces variables
    // car pour le moment ce sont des QString
    // il faudra voir comment on pourrait retourner des QString& (des références sur des QString)
    // il faudra peut-être passer par des variables intermédiaires
    //
    //
    //

    size_t index = 0;

    while (true)
    {
        index = database_attribute.find("'", index);

        if ( index == string::npos )
        {
            break;
        }

         database_attribute.replace(index, 1, "''", 2);

         index += 2;
    }
}


// DB_task_addition method
int DatabaseHandler::DB_task_addition(string const& name,
                                      string const& is_important,
                                      string const& comments,
                                      string const& is_dated,
                                      string const& day,
                                      string const& month,
                                      string const& year,
                                      string const& week_number,
                                      string const& reminder,
                                      string const& weeks_before_task,
                                      string const& is_periodic,
                                      string const& periodicity)
{
    // variables initialization
    int last_added_task_number;

    string modified_name;
    string modified_comments;

    modified_name.append(name);
    modified_comments.append(comments);

    string_replacement(modified_name);
    string_replacement(modified_comments);

    // request initialization
    m_sql_request = string();

    m_sql_request.append("INSERT INTO TASKS (NAME, IS_IMPORTANT, COMMENTS, IS_DATED, DAY, MONTH, YEAR, WEEK_NUMBER, REMINDER,  WEEKS_BEFORE_TASK, IS_PERIODIC, PERIODICITY, IS_PROCESSED)\n");
    m_sql_request.append("VALUES ('");

    m_sql_request.append(modified_name);
    m_sql_request.append("\', ");

    m_sql_request.append(is_important);
    m_sql_request.append(", \"");

    m_sql_request.append(modified_comments);
    m_sql_request.append("\", ");

    m_sql_request.append(is_dated);
    m_sql_request.append(", ");

    m_sql_request.append(day);
    m_sql_request.append(", ");

    m_sql_request.append(month);
    m_sql_request.append(", ");

    m_sql_request.append(year);
    m_sql_request.append(", ");

    m_sql_request.append(week_number);
    m_sql_request.append(", ");

    m_sql_request.append(reminder);
    m_sql_request.append(", ");

    m_sql_request.append(weeks_before_task);
    m_sql_request.append(", ");

    m_sql_request.append(is_periodic);
    m_sql_request.append(", ");

    m_sql_request.append(periodicity);
    m_sql_request.append(", 0);");         // IS_PROCESSED is 0 for task creation

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), dummy_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Ajout d'une tâche dans la DB", "Problème écriture dans la DB");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
        last_added_task_number = 0;
    }
    else
    {
        QMessageBox::information(nullptr, "Ajout d'une tâche dans la DB", "La tâche a bien été ajoutée à la DB");
        last_added_task_number = DB_get_number_of_tasks();
    }

    // DB closing
    DB_closing();

    // Function return
    return last_added_task_number;
}

// DB_task_modification method
void DatabaseHandler::DB_task_modification(string const& number,
                                           string const& name,
                                           string const& is_important,
                                           string const& comments,
                                           string const& is_dated,
                                           string const& day,
                                           string const& month,
                                           string const& year,
                                           string const& week_number,
                                           string const& reminder,
                                           string const& weeks_before_task,
                                           string const& is_periodic,
                                           string const& periodicity)
{
    // variables initialization
    string modified_name = string();
    string modified_comments = string();

    modified_name.append(name);
    modified_comments.append(comments);

    string_replacement(modified_name);
    string_replacement(modified_comments);

    // request initialization
    m_sql_request = string();

    m_sql_request.append("UPDATE TASKS\n");
    m_sql_request.append("SET NAME = \'");
    m_sql_request.append(modified_name);
    m_sql_request.append("\',\n");

    m_sql_request.append("    IS_IMPORTANT = ");
    m_sql_request.append(is_important);
    m_sql_request.append(",\n");

    m_sql_request.append("    COMMENTS = \'");
    m_sql_request.append(modified_comments);
    m_sql_request.append("\',\n");

    m_sql_request.append("    IS_DATED = ");
    m_sql_request.append(is_dated);
    m_sql_request.append(",\n");

    m_sql_request.append("    DAY = ");
    m_sql_request.append(day);
    m_sql_request.append(",\n");

    m_sql_request.append("    MONTH = ");
    m_sql_request.append(month);
    m_sql_request.append(",\n");

    m_sql_request.append("    YEAR = ");
    m_sql_request.append(year);
    m_sql_request.append(",\n");

    m_sql_request.append("    WEEK_NUMBER = ");
    m_sql_request.append(week_number);
    m_sql_request.append(",\n");

    m_sql_request.append("    REMINDER = ");
    m_sql_request.append(reminder);
    m_sql_request.append(",\n");

    m_sql_request.append("    WEEKS_BEFORE_TASK = ");
    m_sql_request.append(weeks_before_task);
    m_sql_request.append(",\n");

    m_sql_request.append("    IS_PERIODIC = ");
    m_sql_request.append(is_periodic);
    m_sql_request.append(",\n");

    m_sql_request.append("    PERIODICITY = ");
    m_sql_request.append(periodicity);
    m_sql_request.append(",\n");

    m_sql_request.append("    IS_PROCESSED = 0\n");    // IS_PROCESSED is 0 for task modification

    m_sql_request.append("WHERE\n");
    m_sql_request.append("    NUMBER = ");
    m_sql_request.append(number);
    m_sql_request.append(";");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), dummy_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Modification d'une tâche dans la DB", "Problème de mise-à-jour de la DB");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method that enables to create the sub_tasks table associated to the periodic table (if not already exists)
void DatabaseHandler::DB_periodic_sub_tasks_table_creation(string const& last_added_task_number)
{
    // request initialization
    string sub_tasks_table_name = "PERIODIC_TASK_" + last_added_task_number +  "_SUB_TASKS";

    m_sql_request = string();
    m_sql_request.append("CREATE TABLE IF NOT EXISTS ");
    m_sql_request.append(sub_tasks_table_name);
    m_sql_request.append("(\n");

    m_sql_request.append("    NUMBER INTEGER PRIMARY KEY AUTOINCREMENT,\n");
    m_sql_request.append("    DAY INTEGER NOT NULL,\n");
    m_sql_request.append("    MONTH INTEGER NOT NULL,\n");
    m_sql_request.append("    YEAR INTEGER NOT NULL,\n");
    m_sql_request.append("    WEEK_NUMBER INTEGER NOT NULL,\n");
    m_sql_request.append("    IS_PROCESSED INTEGER NOT NULL,\n");
    m_sql_request.append("    TASK_NUMBER INTEGER NOT NULL,\n");
    m_sql_request.append("    CONSTRAINT FK_TASK_NUMBER_FROM_TASKS\n");
    m_sql_request.append("        FOREIGN KEY (TASK_NUMBER)\n");
    m_sql_request.append("        REFERENCES TASKS(NUMBER)\n");
    m_sql_request.append(");");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), dummy_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr,
                                 "Création de la table \"" + QString::fromStdString(sub_tasks_table_name) + "\" dans la DB",
                                 "Problème lors de la création de la table \"" + QString::fromStdString(sub_tasks_table_name) + "\"");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to update sub tasks table during periodic task modification
void DatabaseHandler::DB_update_sub_tasks_table(string const& task_number)
{
    // request initialization
    string sub_tasks_table_name = "PERIODIC_TASK_" + task_number +  "_SUB_TASKS";

    m_sql_request = string();
    m_sql_request.append("DELETE FROM ");
    m_sql_request.append(sub_tasks_table_name);
    m_sql_request.append(" WHERE IS_PROCESSED = 0;");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), dummy_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr,
                                 "Suppression des sous-tâches non validées asociée à la tâche n°" + QString::fromStdString(task_number) + " dans la DB",
                                 "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// DB_task_data_retrieval_callback callback function
static int DB_task_data_retrieval_callback(void *data_from_DB, int argc, char **argv, char **azColName)
{
    map<string, string> *c = (map<string, string> *) data_from_DB;

    for (int i(0); i < argc; i++)
    {
        c->insert(make_pair(azColName[i], argv[i]));
    }

    return 0;
}


// Method to retrieve data from DB
void DatabaseHandler::DB_task_data_retrieval(int const& task_number,
                                             map<string, string> *data_from_DB)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT * FROM TASKS WHERE NUMBER = ");
    m_sql_request.append(to_string(task_number));
    m_sql_request.append(";");

    // DB data retrieval
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_task_data_retrieval_callback, (void*) data_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération de données dans la DB", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to validate a task in the DB (IS_PROCESSED = 1)
void DatabaseHandler::DB_task_validation(string const& task_number)
{
    // request initialization
    m_sql_request = string();

    m_sql_request.append("UPDATE TASKS\n");
    m_sql_request.append("SET IS_PROCESSED = 1\n");
    m_sql_request.append("WHERE\n");
    m_sql_request.append("    NUMBER = ");
    m_sql_request.append(task_number);
    m_sql_request.append(";");

    // DB opening
    DB_opening();

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), dummy_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Validation d'une tâche", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }
    else
    {
        QMessageBox::information(nullptr, "Validation d'une tâche", "La tâche a bien été validée");
    }

    // DB closing
    DB_closing();
}


// Method to validate a sub task in the DB (IS_PROCESSED = 1 for the sub task)
void DatabaseHandler::DB_sub_task_validation(string const& task_number,
                                             string const& sub_task_number)
{
    // request initialization
    string sub_tasks_table_name = "PERIODIC_TASK_" + task_number +  "_SUB_TASKS";

    m_sql_request = string();

    m_sql_request.append("UPDATE ");
    m_sql_request.append(sub_tasks_table_name);
    m_sql_request.append("\n");
    m_sql_request.append("SET IS_PROCESSED = 1\n");
    m_sql_request.append("WHERE\n");
    m_sql_request.append("    NUMBER = ");
    m_sql_request.append(sub_task_number);
    m_sql_request.append(";");

    // DB opening
    DB_opening();

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), dummy_callback, 0, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Validation d'une sous-tâche", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }
    else
    {
        QMessageBox::information(nullptr, "Validation d'une sous-tâche", "La sous-tâche a bien été validée");
    }

    // DB closing
    DB_closing();
}


// Callback function for data extraction
static int DB_load_data_tasks_callback(void *data_extraction_from_DB, int argc, char **argv, char **azColName)
{
    vector<map<string, string>> *c = (vector<map<string, string>> *) data_extraction_from_DB;
    map<string, string> tmp_map;

    for (int i(0); i < argc; i++)
    {
        tmp_map.insert(make_pair(azColName[i], argv[i]));
    }

    c->push_back(tmp_map);

    return 0;
}


// Method to load the non dated tasks data
void DatabaseHandler::DB_load_non_dated_tasks(vector<map<string, string>> *data_extraction_from_DB)
{
    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER, NAME, IS_IMPORTANT, COMMENTS, IS_PROCESSED FROM TASKS\n");
    m_sql_request.append("WHERE IS_DATED = 0\n");
    m_sql_request.append("AND IS_PROCESSED = 0;");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_data_tasks_callback, (void*) data_extraction_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Récupération des tâches non datées dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to load the normal tasks data
void DatabaseHandler::DB_load_normal_tasks(string const& current_year,
                                           string const& current_week_number,
                                           vector<map<string, string>> *data_extraction_from_DB)
{
    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER, NAME, IS_IMPORTANT, COMMENTS, IS_DATED, DAY, MONTH, YEAR, WEEK_NUMBER, REMINDER, WEEKS_BEFORE_TASK, IS_PROCESSED FROM TASKS\n");
    m_sql_request.append("WHERE IS_DATED = 1\n");
    m_sql_request.append("AND YEAR = ");
    m_sql_request.append(current_year);
    m_sql_request.append("\n");
    m_sql_request.append("AND WEEK_NUMBER = ");
    m_sql_request.append(current_week_number);
    m_sql_request.append("\n");
    m_sql_request.append("AND IS_PERIODIC = 0\n");
    m_sql_request.append("AND IS_PROCESSED = 0;");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_data_tasks_callback, (void*) data_extraction_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Récupération des tâches normales dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to load reminders prior data
void DatabaseHandler::DB_prior_step_for_reminder_tasks_loading(vector<map<string, string>> *data_extraction_from_DB)
{
    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER, DAY, MONTH, YEAR, WEEKS_BEFORE_TASK FROM TASKS\n");
    m_sql_request.append("WHERE REMINDER = 1\n");
    m_sql_request.append("AND IS_PERIODIC = 0\n");
    m_sql_request.append("AND IS_PROCESSED = 0;");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_data_tasks_callback, (void*) data_extraction_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Récupération préalables des rappels dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to load reminders prior data for periodic tasks
void DatabaseHandler::DB_prior_step_for_reminder_periodic_tasks_loading(vector<map<string, string>> *periodic_data_extraction_from_DB)
{
    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER, NAME, COMMENTS, DAY, MONTH, YEAR, WEEKS_BEFORE_TASK, PERIODICITY FROM TASKS\n");
    m_sql_request.append("WHERE REMINDER = 1\n");
    m_sql_request.append("AND IS_PERIODIC = 1\n");
    m_sql_request.append("AND IS_PROCESSED = 0;");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_data_tasks_callback, (void*) periodic_data_extraction_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Récupération préalables des rappels des tâches périodiques dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to load periodic tasks data
void DatabaseHandler::DB_load_periodic_tasks(vector<map<string, string>> *data_extraction_from_DB)
{
    // request initialization
    m_sql_request = string();
    m_sql_request.append("SELECT * FROM TASKS WHERE IS_PERIODIC = 1;");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_data_tasks_callback, (void*) data_extraction_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Récupération des tâches périodiques dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Callback function for reminder data loading
static int DB_load_reminder_data_task_callback(void *reminder_data_extraction_from_DB, int argc, char **argv, char **azColName)
{
    map<string, string> *c = (map<string, string> *) reminder_data_extraction_from_DB;

    for (int i(0); i < argc; i++)
    {
        c->insert(make_pair(azColName[i], argv[i]));
    }

    return 0;
}


// Method to get the reminder data from the reminder tasks numbers list
void DatabaseHandler::DB_reminder_task_loading_from_reminder_task_number(string const& reminder_task_number,
                                                                         map<string, string> *reminder_data_extraction_from_DB)
{
    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER, NAME, DAY, MONTH, YEAR, COMMENTS, WEEKS_BEFORE_TASK FROM TASKS\n");
    m_sql_request.append("WHERE NUMBER = ");
    m_sql_request.append(reminder_task_number);
    m_sql_request.append(";");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_reminder_data_task_callback, (void*) reminder_data_extraction_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Récupération des données des rappels dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Callback function to get tasks numbers, used in the following methods :
// - DB_get_current_week_tasks_numbers
// - DB_get_current_week_important_tasks_numbers
// - DB_get_current_week_periodic_tasks_numbers
static int DB_get_tasks_numbers_callback(void *tasks, int argc, char **argv, char **azColName)
{
    vector<int> *c = (vector<int> *) tasks;

    for (int i(0); i < argc; i++)
    {
        c->push_back(stoi(argv[i]));
    }

    return 0;
}


// Method o get the current week tasks numbers
void DatabaseHandler::DB_get_current_week_tasks_numbers(string const& current_year,
                                                        string const& current_week_number,
                                                        vector<int> *current_week_tasks)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER FROM TASK_TABLE\n");
    m_sql_request.append("WHERE\n");
    m_sql_request.append("    YEAR = ");
    m_sql_request.append(current_year);
    m_sql_request.append("\n");
    m_sql_request.append("AND WEEK_NUMBER = ");
    m_sql_request.append(current_week_number);
    m_sql_request.append(";");

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_get_tasks_numbers_callback, (void*) current_week_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches de la semaine courante", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to get the current week important tasks numbers
void DatabaseHandler::DB_get_current_week_important_tasks_numbers(string const& current_year,
                                                                  string const& current_week_number,
                                                                  vector<int> *current_week_important_tasks)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER FROM TASKS\n");
    m_sql_request.append("WHERE\n");
    m_sql_request.append("    YEAR = ");
    m_sql_request.append(current_year);
    m_sql_request.append("\n");
    m_sql_request.append("AND WEEK_NUMBER = ");
    m_sql_request.append(current_week_number);
    m_sql_request.append("\n");
    m_sql_request.append("AND IS_IMPORTANT = 1;");

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_get_tasks_numbers_callback, (void*) current_week_important_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches importantes de la semaine courante", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
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


//Method to get the reminder tasks numbers
void DatabaseHandler::DB_get_reminder_tasks_numbers(map<int, map<string, string>> *reminder_tasks_data)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = string();
    m_sql_request.append("SELECT NUMBER, YEAR, MONTH, DAY, WEEK_NUMBER, WEEKS_BEFORE_TASK FROM TASK_TABLE WHERE REMINDER = 1;");

    // Mark task as processed
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_get_reminder_tasks_numbers_callback, (void*) reminder_tasks_data, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches possédant un rappel", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to get the current week
void DatabaseHandler::DB_get_current_week_periodic_tasks_numbers(vector<int> *current_week_periodic_tasks)
{
    // DB opening
    DB_opening();

    // request initialization
    m_sql_request = string();

    m_sql_request.append("SELECT NUMBER FROM TASK_TABLE WHERE PERIODIC_TASK = 1;");

    // ...
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_get_tasks_numbers_callback, (void*) current_week_periodic_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération des tâches périodiques de la semaine courante", "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Method to get sub tasks data of a periodic task
void DatabaseHandler::DB_get_periodic_task_sub_tasks(string task_number,
                                                     vector<map<string, string>> *sub_tasks_extraction_from_DB)
{
    // request initialization
    string sub_tasks_table_name = "PERIODIC_TASK_" + task_number +  "_SUB_TASKS";

    m_sql_request = string();

    m_sql_request.append("SELECT * FROM ");
    m_sql_request.append(sub_tasks_table_name);
    m_sql_request.append(" WHERE TASK_NUMBER = ");
    m_sql_request.append(task_number);
    m_sql_request.append(";");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_data_tasks_callback, (void*) sub_tasks_extraction_from_DB, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Récupération des sous-tâches associée à la périodique n° " + QString::fromStdString(task_number) + " dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();
}


// Callback function to get the sub tasks number associated to the periodic task number
int DatabaseHandler::DB_get_number_of_sub_tasks(string sub_tasks_table_name,
                                                string associated_periodic_task_number)
{
    // variables initialization
    int number_of_sub_tasks;

    // request initialization
    m_sql_request = string();
    m_sql_request.append("SELECT COUNT(*) FROM ");
    m_sql_request.append(sub_tasks_table_name);
    m_sql_request.append(";");

    // DB opening
    DB_opening();

    // DB counting
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_get_number_of_items_callback, &number_of_sub_tasks, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::information(nullptr, "Récupération du nombre de sous-tâches associées à la tâche n°" + QString::fromStdString(associated_periodic_task_number), "Problème");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
    }

    // DB closing
    DB_closing();

    // method return
    return number_of_sub_tasks;
}


// Method to add a sub task to a periodic task
int DatabaseHandler::add_sub_task_to_periodic_task(string current_day,
                                                   string current_month,
                                                   string current_year,
                                                   string current_week_number,
                                                   string associated_periodic_task_number)
{
    // variables initialization
    int last_added_task_number;

    // request initialization
    string sub_tasks_table_name = "PERIODIC_TASK_" + associated_periodic_task_number +  "_SUB_TASKS";

    m_sql_request = string();

    m_sql_request.append("INSERT INTO ");
    m_sql_request.append(sub_tasks_table_name);
    m_sql_request.append("(DAY, MONTH, YEAR, WEEK_NUMBER, IS_PROCESSED, TASK_NUMBER)\n");
    m_sql_request.append("VALUES(");
    m_sql_request.append(current_day);
    m_sql_request.append(", ");
    m_sql_request.append(current_month);
    m_sql_request.append(", ");
    m_sql_request.append(current_year);
    m_sql_request.append(", ");
    m_sql_request.append(current_week_number);
    m_sql_request.append(", 0, ");              // IS_PROCESSED is set to 0 for sub task creation
    m_sql_request.append(associated_periodic_task_number);
    m_sql_request.append(");");

    // DB opening
    DB_opening();

    // DB insertion
    m_rc = sqlite3_exec(m_database, m_sql_request.c_str(), DB_load_data_tasks_callback, (void*) dummy_callback, &m_zErrMsg);

    if( m_rc != SQLITE_OK )
    {
        QMessageBox::critical(nullptr, "Ajout d'une sous-tâche à la tâche périodique n° " + QString::fromStdString(associated_periodic_task_number) + " dans la DB", "Problème !");
        QMessageBox::warning(nullptr, "Détails du message d'erreur lors du traitement avec la BDD", QString::fromStdString(m_zErrMsg));
        sqlite3_free(m_zErrMsg);
        last_added_task_number = 0;
    }
    else
    {
        last_added_task_number = DB_get_number_of_sub_tasks(sub_tasks_table_name, associated_periodic_task_number);
    }

    // DB closing
    DB_closing();

    // Function return
    return last_added_task_number;
}


// Database closing method
void DatabaseHandler::DB_closing()
{
    sqlite3_close(m_database);
}
