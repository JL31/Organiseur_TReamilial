#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <sqlite3.h>
#include <string>
#include <map>
#include <vector>

class DatabaseHandler
{
    public:
        // Constructors and destructor
            DatabaseHandler();
            virtual ~DatabaseHandler();

        void DB_opening();
        int DB_get_number_of_tasks();
        void DB_task_addition(std::string const& new_nbr_of_tasks,
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
                              std::string const& task_periodicity);
        void DB_task_modification(std::string const& task_number,
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
                                  std::string const& task_periodicity);
        void DB_task_validation(std::string const& task_number);
        void DB_task_data_retrieval(int const& task_number, std::map<std::string, std::string> *data_from_DB);
        void DB_get_current_week_tasks_numbers(std::string const& current_year, std::string const& current_week_number, std::vector<int> *current_week_tasks);
        void DB_get_current_week_important_tasks_numbers(std::string const& current_year, std::string const& current_week_number, std::vector<int> *current_week_important_tasks);
        void DB_get_reminder_tasks_numbers(std::map<int, std::map<std::string, std::string>> *reminder_tasks_data);
        void DB_get_current_week_periodic_tasks_numbers(std::vector<int> *current_week_periodic_tasks);
        void DB_closing();

    private:
        sqlite3 *m_database;
        std::string m_database_name;
        char *m_zErrMsg;
        int m_rc;
        std::string *m_sql_request;
        int *m_counter;
};

#endif // DATABASEHANDLER_H
