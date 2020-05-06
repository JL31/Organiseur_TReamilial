#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Includes
#include <QMainWindow>

#include "taskmanager.h"
#include "ihm_gestion_des_taches.h"
#include "taskbutton.h"
#include "task.h"

#include <map>
#include <vector>
#include <cstring>

#include <QtWidgets>

// Définition d'un namespace
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE


// Définition de la classe "MainWindow"
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        // Constructor and destructor
            MainWindow(QWidget *parent = nullptr);
            ~MainWindow();

        // Methods
            void scroll_areas_initialization();
            void days_spacer_addition();
            void actions_connection() const;
            void calendar_week_update();
            void tasks_update();
            void prior_cleaning_and_initialization();
            void non_dated_tasks_update();
            void important_tasks_update();
            void calendar_tasks_update();
            void reminder_tasks_update();
            void connect_task_button_signals();

    public slots:
        void change_task_number_and_selected_button(int const& task_number);
        void change_task_number(int const& task_number);
        void go_to_previous_week();
        void go_to_next_week();
        void task_addition();
        void task_modification();
        void processed_task();

    private:
        Ui::MainWindow *ui;
        IHMGestionDesTaches *ihm_gdt;
        std::map<std::string, std::string> *m_data_from_DB;
        TaskManager tm;
        int m_choix_gdt;
        int m_selected_task_number;
        QDate m_current_date;
        std::map<int, QString> i_to_s_month;

        QVBoxLayout *m_monday_tasks_layout;
        QVBoxLayout *m_tuesday_tasks_layout;
        QVBoxLayout *m_wednesday_tasks_layout;
        QVBoxLayout *m_thursday_tasks_layout;
        QVBoxLayout *m_friday_tasks_layout;
        QVBoxLayout *m_saturday_tasks_layout;
        QVBoxLayout *m_sunday_tasks_layout;
        QVBoxLayout *m_non_dated_tasks_layout;
        QHBoxLayout *m_important_tasks_layout;

        QWidget *m_monday_tasks_sa_base_widget;
        QWidget *m_tuesday_tasks_sa_base_widget;
        QWidget *m_wednesday_tasks_sa_base_widget;
        QWidget *m_thursday_tasks_sa_base_widget;
        QWidget *m_friday_tasks_sa_base_widget;
        QWidget *m_saturday_tasks_sa_base_widget;
        QWidget *m_sunday_tasks_sa_base_widget;
        QWidget *m_non_dated_tasks_sa_base_widget;
        QWidget *m_important_tasks_sa_base_widget;

        std::vector<QWidget *> m_sa_list;
};
#endif // MAINWINDOW_H
