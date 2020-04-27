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
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void days_layout_definition();
        void actions_connection() const;
        void current_week_tasks_update();
        void calendar_week_update();

        // tmp - début
        void tasks_update();
        void non_dated_tasks_update();
        void calendar_tasks_update();
        // tmp - fin

        void days_spacer_addition();
        void current_week_tasks_cleaning();
        void important_tasks_update();
        void current_week_important_tasks_cleaning();
        void reminder_tasks_update();
        void current_week_periodic_task_update();
        void current_week_periodic_tasks_cleaning();

    public slots:
        void task_addition();
        void task_modification();
        //void processed_task();
        //void showEvent(QShowEvent *ev);
        //void go_to_previous_week();
        //void go_to_next_week();
        void change_task_number_and_selected_button(int const& task_number);
        void change_task_number(int const& task_number);
        void connect_task_button_signals();

    private:
        Ui::MainWindow *ui;
        TaskManager *tm;
        IHMGestionDesTaches *ihm_gdt;
        int m_choix_gdt;
        std::map<std::string, std::string> *m_data_from_DB;
        QDate *m_current_date;
        std::vector<int> *m_current_week_tasks;
        std::vector<int> *m_current_week_periodic_tasks;
        std::map<int, QString> i_to_s_month;
        QVBoxLayout *m_layout_taches_lundi;
        QVBoxLayout *m_layout_taches_mardi;
        QVBoxLayout *m_layout_taches_mercredi;
        QVBoxLayout *m_layout_taches_jeudi;
        QVBoxLayout *m_layout_taches_vendredi;
        QVBoxLayout *m_layout_taches_samedi;
        QVBoxLayout *m_layout_taches_dimanche;
        QVBoxLayout *m_non_dated_tasks_layout;
        std::vector<QScrollArea*> *m_sa_list;
        int *m_selected_task_number;
};
#endif // MAINWINDOW_H
