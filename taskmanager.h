#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "task.h"
#include "periodictask.h"
#include "databasehandler.h"

#include <map>
#include <vector>
#include <cstring>
#include <QString>
#include <QDate>

class TaskManager
{
    public:
        TaskManager();
        virtual ~TaskManager();

        void add_task(QString const& task_name,
                      QString const& task_importance,
                      QDate const& task_date,
                      QString const& comments,
                      bool const& reminder,
                      int const& nbr_weeks_before_task,
                      bool const& periodic_task_state,
                      int const& task_periodicity);
        void modify_task(int const& task_number,
                         QString const& task_name,
                         QString const& task_importance,
                         QDate const& task_date,
                         QString const& comments,
                         bool const& reminder,
                         int const& nbr_weeks_before_task,
                         bool const& periodic_task_state,
                         int const& task_periodicity);
        void validate_task(int const& task_number);
        void data_retrieval(int const& task_number, std::map<std::string, std::string> *data_from_DB);
        int get_number_of_tasks() const;
        void load_current_week_tasks_numbers(int const& current_year, int const& current_week_number, std::vector<int> *current_week_tasks);
        void load_current_week_tasks(int const& task_number);
        const std::map<int, Task*>& get_one_time_task_list() const;
        void clear_current_week_tasks();
        void load_current_week_important_tasks_list(int const& current_year, int const& current_week_number, std::vector<int> *current_week_important_tasks_list);
        void load_current_week_important_tasks(int const& task_number);
        const std::map<int, Task*>& get_important_task_list() const;
        void clear_current_week_important_tasks();
        void load_reminder_tasks_numbers_list(int const& current_year, int const& current_week_number, std::vector<int> *reminder_tasks_numbers_list);
        void load_reminder_tasks_list(int const& task_number);
        const std::map<int, Task*>& get_reminder_task_list() const;
        void clear_current_week_reminder_tasks();
        void load_current_week_periodic_tasks_numbers(int const& current_year, int const& current_week_number, std::vector<int> *current_week_periodic_tasks);
        void load_current_week_periodic_tasks(int const& task_number);
        void clear_current_week_periodic_tasks();
        const std::map<int, PeriodicTask*>& get_periodic_task_list() const;

    protected:
        std::map<int, Task*> *m_one_time_task_list;
        std::map<int, PeriodicTask*> *m_periodic_task_list;
        std::map<int, Task*> *m_important_task_list;
        std::map<int, Task*> *m_reminder_task_list;
        int *m_number_of_tasks;
        DatabaseHandler *m_db_handler;
        int *m_week_number;
        std::map<std::string, std::string> *m_task_data_extraction_from_DB;
};

#endif // TASKMANAGER_H
