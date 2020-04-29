#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "nondatedtask.h"
#include "normaltask.h"
#include "periodictask.h"
#include "databasehandler.h"
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
            TaskManager();
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

                void validate_task(int const& number);

            void data_retrieval(int const& number, std::map<std::string, std::string> *data_from_DB);

            // REFONTE --- Début

            void load_current_week_data(int const& current_year, int const& current_week_number);
            void task_lists_cleaning();
            void load_non_dated_tasks();
            void load_normal_tasks(int const& current_year, int const& current_week_number);
            void load_reminders_list(int const& current_year, int const& current_week_number);
            void load_important_tasks_list(int const& current_year, int const& current_week_number);

            // REFONTE --- Fin


            //
            void load_current_week_tasks_numbers(int const& current_year, int const& current_week_number, std::vector<int> *current_week_tasks);
            void load_current_week_tasks(int const& task_number);
            void clear_current_week_tasks();

            void load_current_week_important_tasks_list(int const& current_year, int const& current_week_number, std::vector<int> *current_week_important_tasks_list);
            void load_current_week_important_tasks(int const& task_number);
            void clear_current_week_important_tasks();

            void load_reminder_tasks_numbers_list(int const& current_year, int const& current_week_number, std::vector<int> *reminder_tasks_numbers_list);
            void load_reminder_tasks_list(int const& task_number);
            void clear_current_week_reminder_tasks();

            void load_current_week_periodic_tasks_numbers(int const& current_year, int const& current_week_number, std::vector<int> *current_week_periodic_tasks);
            void load_current_week_periodic_tasks(int const& task_number);
            void clear_current_week_periodic_tasks();

        // Getters
            const std::map<int, NonDatedTask*>& get_non_dated_tasks_list() const;
            const std::map<int, NormalTask*>& get_normal_tasks_list() const;
            const std::map<int, PeriodicTask*>& get_periodic_tasks_list() const;
            const std::vector<Reminder*>& get_reminder_tasks_list() const;
            const std::vector<NonDatedTask*>& get_important_tasks_list() const;

            int get_number_of_tasks() const;

    protected:
        // Attributes
            std::map<int, NonDatedTask*> *m_non_dated_tasks_list;
            std::map<int, NormalTask*> *m_normal_tasks_list;
            std::map<int, PeriodicTask*> *m_periodic_tasks_list;
            std::vector<Reminder*> *m_reminder_tasks_list;
            std::vector<NonDatedTask*> *m_important_tasks_list;

            int *m_number_of_tasks;

            DatabaseHandler *m_db_handler;
            std::vector<std::map<std::string, std::string>> *m_data_extraction_from_DB;
};

#endif // TASKMANAGER_H
