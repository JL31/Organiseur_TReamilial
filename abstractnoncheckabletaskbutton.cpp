#include "abstractnoncheckabletaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
AbstractNonCheckableTaskButton::AbstractNonCheckableTaskButton(QString const& button_text,
                                                               BaseTask *task) : AbstractTaskButton(button_text, task)
{}


// Destructor
AbstractNonCheckableTaskButton::~AbstractNonCheckableTaskButton()
{}


// Methods
// -------

// Action linked to the double click on the button
void AbstractNonCheckableTaskButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    QDialog *dialog_display_selected_task_data = new QDialog(this);
    dialog_display_selected_task_data->setWindowTitle("Tâche n°" + QString::number(m_task->get_number()));

    QLabel *l_task_name = new QLabel("Nom de la tâche");
    QLineEdit *le_task_name = new QLineEdit(m_task->get_name());
    le_task_name->setReadOnly(true);
    QHBoxLayout *task_name_layout = new QHBoxLayout();
    task_name_layout->addWidget(l_task_name);
    task_name_layout->addWidget(le_task_name);

    QLabel *l_task_comments = new QLabel("Commentaires");
    l_task_comments->setFixedWidth(l_task_name->fontMetrics().boundingRect(l_task_name->text()).width());
    l_task_comments->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QTextEdit *te_task_name = new QTextEdit(m_task->get_comments());
    te_task_name->setReadOnly(true);
    QHBoxLayout *task_comments_layout = new QHBoxLayout();
    task_comments_layout->addWidget(l_task_comments);
    task_comments_layout->addWidget(te_task_name);

    QVBoxLayout *dialog_layout = new QVBoxLayout();
    dialog_layout->addLayout(task_name_layout);
    dialog_layout->addLayout(task_comments_layout);
    QPushButton *close_button = new QPushButton("Fermer la fenêtre");
    dialog_layout->addWidget(close_button);
    connect(close_button, SIGNAL(clicked()), dialog_display_selected_task_data, SLOT(accept()));

    dialog_display_selected_task_data->setLayout(dialog_layout);

    // pour éviter que la fenêtre n'hérite de la couleur de la fenêtre parente (dans le cas d'une tâche Importante ou d'un rappel)
    //
    //
    // Encore utile avec cette nouvelle architecture ?
    //
    //
    dialog_display_selected_task_data->setStyleSheet("background-color:None");

    dialog_display_selected_task_data->exec();

    delete dialog_display_selected_task_data;
    dialog_display_selected_task_data = nullptr;
}

// Method that enables to define if the current button can be checkable or not
void AbstractNonCheckableTaskButton::set_checkable_state()
{
    this->setCheckable(false);
}
