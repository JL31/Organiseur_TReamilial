#ifndef TASK_HANDLING_HMI_H
#define TASK_HANDLING_HMI_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>

namespace Ui
{
    class task_handling_hmi;
}

class TaskHandlingHMI : public QWidget
{
    Q_OBJECT

    public:
        // Constructor and destructor
            explicit TaskHandlingHMI(QWidget *parent = nullptr);
            virtual ~TaskHandlingHMI();

        // Methods
            void connections() const;
            void dialog_initialization_for_task_modification(std::map<std::string, std::string> *data_from_DB);

        // Getters
            QPushButton* get_cancel_validate_button();
            QPushButton* get_add_modify_button();
            QString get_name() const;
            bool get_is_important() const;
            bool get_is_dated() const;
            QDate get_date() const;
            bool get_is_periodic()const;
            int get_periodicity() const;
            bool get_reminder() const;
            int get_weeks_before_task() const;
            QString get_comments() const;

    public slots:
        void dated_task_selection();
        void name_content_length_check(QString const& text);
        void task_periodicity_value_check();

    private:
        Ui::task_handling_hmi *ui;
};

#endif // TASK_HANDLING_HMI_H
