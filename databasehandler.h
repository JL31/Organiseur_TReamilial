#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <sqlite3.h>
#include <string>
#include <map>
#include <vector>

#include "nondatedtask.h"


class DatabaseHandler
{
    public:
        // Constructors and destructor
            DatabaseHandler(std::string const& database_complete_path);
            virtual ~DatabaseHandler();

        void DB_initialization();

        void DB_opening();

        int DB_get_number_of_tasks();   // utilisée ???

        void DB_task_addition(std::string const& name,
                              std::string const& is_important,
                              std::string const& comments,
                              std::string const& is_dated,
                              std::string const& day,
                              std::string const& month,
                              std::string const& year,
                              std::string const& week_number,
                              std::string const& reminder,
                              std::string const& weeks_before_task,
                              std::string const& is_periodic,
                              std::string const& periodicity);

        void DB_task_modification(std::string const& number,
                                  std::string const& name,
                                  std::string const& is_important,
                                  std::string const& comments,
                                  std::string const& is_dated,
                                  std::string const& day,
                                  std::string const& month,
                                  std::string const& year,
                                  std::string const& week_number,
                                  std::string const& reminder,
                                  std::string const& weeks_before_task,
                                  std::string const& is_periodic,
                                  std::string const& periodicity);

        void DB_task_validation(std::string const& task_number);

        void DB_load_non_dated_tasks(std::vector<std::map<std::string, std::string>> *data_extraction_from_DB);
        void DB_load_normal_tasks(std::string const& current_year, std::string const& current_week_number, std::vector<std::map<std::string, std::string>> *data_extraction_from_DB);
        void DB_prior_step_for_reminder_tasks_loading(std::vector<std::map<std::string, std::string>> *data_extraction_from_DB);
        void DB_reminder_task_loading_from_reminder_task_number(std::string const&  reminder_task_number, std::map<std::string, std::string> *data_extraction_from_DB);

        void DB_task_data_retrieval(int const& task_number, std::map<std::string, std::string> *data_from_DB);
        void DB_get_current_week_tasks_numbers(std::string const& current_year, std::string const& current_week_number, std::vector<int> *current_week_tasks);
        void DB_get_current_week_important_tasks_numbers(std::string const& current_year, std::string const& current_week_number, std::vector<int> *current_week_important_tasks);
        void DB_get_reminder_tasks_numbers(std::map<int, std::map<std::string, std::string>> *reminder_tasks_data);
        void DB_get_current_week_periodic_tasks_numbers(std::vector<int> *current_week_periodic_tasks);
        void DB_closing();

    private:
        // Attributes
            sqlite3 *m_database;
            std::string m_database_name;
            //char** m_zErrMsg;
            char* m_zErrMsg;
            int m_rc;
            std::string m_sql_request;
};

#endif // DATABASEHANDLER_H
