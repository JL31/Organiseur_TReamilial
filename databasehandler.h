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

        int DB_get_number_of_tasks();

        int DB_task_addition(std::string const& name,
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

        void DB_periodic_sub_tasks_table_creation(std::string const& last_added_task_number);

        void DB_task_validation(std::string const& task_number);
        void DB_sub_task_validation(std::string const& task_number,
                                    std::string const& sub_task_number);

        void DB_load_non_dated_tasks(std::vector<std::map<std::string, std::string>> *data_extraction_from_DB);
        void DB_load_normal_tasks(std::string const& current_year,
                                  std::string const& current_week_number,
                                  std::vector<std::map<std::string, std::string>> *data_extraction_from_DB);
        void DB_load_periodic_tasks(std::vector<std::map<std::string, std::string>> *data_extraction_from_DB);
        void DB_prior_step_for_reminder_tasks_loading(std::vector<std::map<std::string, std::string>> *data_extraction_from_DB);
        void DB_prior_step_for_reminder_periodic_tasks_loading(std::vector<std::map<std::string, std::string>> *periodic_data_extraction_from_DB);
        void DB_reminder_task_loading_from_reminder_task_number(std::string const& reminder_task_number,
                                                                std::map<std::string, std::string> *data_extraction_from_DB);

        void DB_task_data_retrieval(int const& task_number,
                                    std::map<std::string, std::string> *data_from_DB);
        void DB_get_current_week_tasks_numbers(std::string const& current_year,
                                               std::string const& current_week_number,
                                               std::vector<int> *current_week_tasks);
        void DB_get_current_week_important_tasks_numbers(std::string const& current_year,
                                                         std::string const& current_week_number,
                                                         std::vector<int> *current_week_important_tasks);
        void DB_get_reminder_tasks_numbers(std::map<int, std::map<std::string, std::string>> *reminder_tasks_data);

        void DB_get_current_week_periodic_tasks_numbers(std::vector<int> *current_week_periodic_tasks); // toujours utile ?

        void DB_get_periodic_task_sub_tasks(std::string task_number,
                                            std::vector<std::map<std::string, std::string>> *sub_tasks_extraction_from_DB);
        int DB_get_number_of_sub_tasks(std::string sub_tasks_table_name,
                                       std::string associated_periodic_task_number);
        int add_sub_task_to_periodic_task(std::string current_day,
                                          std::string current_month,
                                          std::string current_year,
                                          std::string current_week_number,
                                          std::string associated_periodic_task_number);
        void DB_closing();

    private:
        // Attributes
            sqlite3 *m_database;
            std::string m_database_name;
            char* m_zErrMsg;
            int m_rc;
            std::string m_sql_request;
};

#endif // DATABASEHANDLER_H
