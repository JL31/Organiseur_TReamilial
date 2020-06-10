#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "databasehandler.h"
#include "basetask.h"
#include "nondatedtask.h"
#include "normaltask.h"
#include "periodictask.h"
#include "reminder.h"

#include <map>
#include <vector>
#include <cstring>
#include <QString>
#include <QDate>

class TaskManager
{
    public:
        // Constructor and destructor
            TaskManager(QString const& database_complete_path,
                        bool const& database_folder_automatically_initiated);
            virtual ~TaskManager();

        // Methods
            // Methods to interact with database for basic purposes : addition, update and validation of task into database
                void add_task(QString const& name,
                              bool const& is_important,
                              QString const& comments,
                              bool const& is_dated,
                              QDate const& date,
                              bool const& reminder,
                              int const& weeks_before_task,
                              bool const& is_periodic,
                              int const& periodicity);

                void modify_task(int const& number,
                                 QString const& name,
                                 bool const& is_important,
                                 QString const& comments,
                                 bool const& is_dated,
                                 QDate const& date,
                                 bool const& reminder,
                                 int const& weeks_before_task,
                                 bool const& is_periodic,
                                 int const& periodicity);

                void validate_task(int const& task_number, int const& sub_task_number);

                void data_retrieval(int const& number, std::map<std::string, std::string> *data_from_DB);

            // Other methods
                //void load_current_week_data(int const& current_year, int const& current_week_number);
                void load_current_week_data(QDate const& current_week_date);
                void task_lists_cleaning();
                void load_non_dated_tasks();
                void load_normal_tasks(int const& current_year, int const& current_week_number);
                void add_periodic_task_and_sub_tasks(std::map<std::string, std::string> periodic_task_data,
                                                     QDate const& current_week_date,
                                                     QDate const& recalculated_date);
                void load_periodic_tasks(QDate const& current_week_date);
                void load_reminders_list(int const& current_year,
                                         int const& current_week_number);
                void load_important_tasks_list(int const& current_year,
                                               int const& current_week_number);

            // Getters
                std::map<int, NonDatedTask>& get_non_dated_tasks_list();
                std::map<int, NormalTask>& get_normal_tasks_list();
                std::map<int, PeriodicTask>& get_periodic_tasks_list();
                std::vector<Reminder>& get_reminder_tasks_list();
                std::vector<BaseTask>& get_important_tasks_list();

    protected:
        // Attributes
            std::map<int, NonDatedTask> m_non_dated_tasks_list;
            std::map<int, NormalTask> m_normal_tasks_list;
            std::map<int, PeriodicTask> m_periodic_tasks_list;
            std::vector<Reminder> m_reminder_tasks_list;
            std::vector<BaseTask> m_important_tasks_list;

            DatabaseHandler m_db_handler;
            std::vector<std::map<std::string, std::string>> *m_data_extraction_from_DB;

            bool m_database_folder_automatically_initiated;
};

#endif // TASKMANAGER_H
