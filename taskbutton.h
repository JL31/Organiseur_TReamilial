#ifndef TASKBUTTON_H
#define TASKBUTTON_H

#include <QtWidgets>

class TaskButton : public QPushButton
{
    Q_OBJECT

    public:
        TaskButton(QString const& text, int const& task_number, QString const& task_importance, QString const& task_comments, bool const& reminder_state, bool const& is_checkable, bool const& is_periodic, QWidget *parent = nullptr, bool const& panel_task = false, int const& nbr_weeks_before_tasks = 0);
        virtual ~TaskButton();
        int get_task_button_task_number() const;
        bool get_task_button_is_periodic_task() const;

    public slots:
        void emit_specific_signal(bool const& button_state);

    signals:

        void button_state_and_task_number(int const & m_task_number);
        void button_task_number(int const& m_task_number);

    protected:
        // methods
            void mouseDoubleClickEvent(QMouseEvent *event);
            void adapt_button_text(QString const& text, bool const& reminder_state, int const& nbr_weeks_before_tasks, bool const& panel_task);
            void set_button_color(QString const& task_importance, bool const& reminder_state, bool const& panel_task);

        // attributes
            int *m_taille;
            int *m_task_number;
            bool *m_is_task_periodic;
            QString *m_task_name;
            QString *m_task_comments;
};

#endif // TASKBUTTON_H
