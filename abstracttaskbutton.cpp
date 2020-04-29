#include "abstracttaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
AbstractTaskButton::AbstractTaskButton(QString const& button_text,
                                       NonDatedTask *task,
                                       Reminder *reminder) : m_button_text(new QString(button_text)),
                                                             m_task(task),
                                                             m_reminder(reminder)
{
    // Provisoire - début
    this->setMinimumSize(85, 85);
    this->setMaximumSize(85, 85);
    // Provisoire - fin
}


// Destructor
AbstractTaskButton::~AbstractTaskButton()
{
    delete m_button_text;
    delete m_task;
    delete m_reminder;

    m_button_text = nullptr;
    m_task = nullptr;
    m_reminder = nullptr;
}


// Getters
// -------

int AbstractTaskButton::get_task_number()
{
    return m_task->get_number();
}


// Methods
// -------

// Action linked to the double click on the button
void AbstractTaskButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    QDialog *dialog_display_selected_task_data = new QDialog(this);
    QString *window_title = new QString();
    if ( m_task == nullptr )
    {
        *window_title = QString::number(m_reminder->get_number());
    }
    else
    {
        *window_title = QString::number(m_task->get_number());
    }
    dialog_display_selected_task_data->setWindowTitle("Tâche n°" + *window_title);

    QLabel *l_task_name = new QLabel("Nom de la tâche");
    QString *task_name = new QString();
    if ( m_task == nullptr )
    {
        *task_name = m_reminder->get_name();
    }
    else
    {
        *task_name = m_task->get_name();
    }
    QLineEdit *le_task_name = new QLineEdit(*task_name);
    le_task_name->setReadOnly(true);
    QHBoxLayout *task_name_layout = new QHBoxLayout();
    task_name_layout->addWidget(l_task_name);
    task_name_layout->addWidget(le_task_name);

    QLabel *l_task_comments = new QLabel("Commentaires");
    l_task_comments->setFixedWidth(l_task_name->fontMetrics().boundingRect(l_task_name->text()).width());
    l_task_comments->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QString *task_comments = new QString();
    if ( m_task == nullptr )
    {
        *task_comments = m_reminder->get_comments();
    }
    else
    {
        *task_comments = m_task->get_comments();
    }
    QTextEdit *te_task_name = new QTextEdit(*task_comments);
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

    delete l_task_name;
    delete task_name;
    delete le_task_name;
    delete task_name_layout;
    delete l_task_comments;
    delete task_comments;
    delete te_task_name;
    delete task_comments_layout;
    delete close_button;
    delete dialog_layout;
    delete window_title;
    delete dialog_display_selected_task_data;

    l_task_name = nullptr;
    task_name = nullptr;
    le_task_name = nullptr;
    task_name_layout = nullptr;
    l_task_comments = nullptr;
    task_comments = nullptr;
    te_task_name = nullptr;
    task_comments_layout = nullptr;
    close_button = nullptr;
    dialog_layout = nullptr;
    window_title = nullptr;
    dialog_display_selected_task_data = nullptr;
}


// Method that enables to modify the button text
void AbstractTaskButton::adapt_button_text()
{
    // variable initialization
    QString *new_texte = new QString();

    // definition of the button text
    new_texte->append("Tâche n°");
    new_texte->append(QString::number(m_task->get_number()));
    new_texte->append("\n\n");
    new_texte->append(*m_button_text);

    /*if ( text.size() > 10 )
    {
        // compliqué, à terminer plus tard...
        // il faudra gérer la hauteur du bouton pour l'adapter aux nombres de lignes du texte du bouton
        // il faudra couper le texte à la fonction de la longueur du texte
        // il ne faudra pas couper de mots
        new_texte->append(text);
    }
    else
    {
        new_texte->append(text);
    }*/

    // modification of the button text
    this->setText(*new_texte);

    // variable cleaning
    delete new_texte;
    new_texte = nullptr;
}
