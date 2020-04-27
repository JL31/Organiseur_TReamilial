#include "abstracttaskbutton.h"

// Constructor
AbstractTaskButton::AbstractTaskButton(QString const& button_text,
                                       NonDatedTask const& task) : m_button_text(new QString(button_text)),
                                                                   m_task(new NonDatedTask(task))
{
    // Provisoire - début
    this->setMinimumSize(85, 85);
    this->setMaximumSize(85, 85);
    // Provisoire - fin
    adapt_button_text();
}


// Destructor
AbstractTaskButton::~AbstractTaskButton()
{
    delete m_button_text;
    delete m_task;

    m_button_text = nullptr;
    m_task = nullptr;
}


// Getters
int AbstractTaskButton::get_task_number()
{
    return m_task->get_number();
}


// Action linked to the double click on the button
void AbstractTaskButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    QDialog *m_dialog_display_selected_task_data = new QDialog(this);
    m_dialog_display_selected_task_data->setWindowTitle("Tâche n°" + QString::number(m_task->get_number()));

    QLabel *m_l_task_name = new QLabel("Nom de la tâche");
    QLineEdit *m_le_task_name = new QLineEdit(m_task->get_name());
    m_le_task_name->setReadOnly(true);
    QHBoxLayout *m_task_name_layout = new QHBoxLayout();
    m_task_name_layout->addWidget(m_l_task_name);
    m_task_name_layout->addWidget(m_le_task_name);

    QLabel *m_l_task_comments = new QLabel("Commentaires");
    m_l_task_comments->setFixedWidth(m_l_task_name->fontMetrics().boundingRect(m_l_task_name->text()).width());
    m_l_task_comments->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QTextEdit *m_te_task_name = new QTextEdit(m_task->get_comments());
    m_te_task_name->setReadOnly(true);
    QHBoxLayout *m_task_comments_layout = new QHBoxLayout();
    m_task_comments_layout->addWidget(m_l_task_comments);
    m_task_comments_layout->addWidget(m_te_task_name);

    QVBoxLayout *m_dialog_layout = new QVBoxLayout();
    m_dialog_layout->addLayout(m_task_name_layout);
    m_dialog_layout->addLayout(m_task_comments_layout);
    QPushButton *m_close_button = new QPushButton("Fermer la fenêtre");
    m_dialog_layout->addWidget(m_close_button);
    connect(m_close_button, SIGNAL(clicked()), m_dialog_display_selected_task_data, SLOT(accept()));

    m_dialog_display_selected_task_data->setLayout(m_dialog_layout);

    // pour éviter que la fenêtre n'hérite de la couleur de la fenêtre parente (dans le cas d'une tâche Importante ou d'un rappel)
    //
    //
    // Encore utile avec cette nouvelle architecture ?
    //
    //
    m_dialog_display_selected_task_data->setStyleSheet("background-color:None");

    m_dialog_display_selected_task_data->exec();

    delete m_l_task_name;
    delete m_le_task_name;
    delete m_task_name_layout;
    delete m_l_task_comments;
    delete m_te_task_name;
    delete m_task_comments_layout;
    delete m_close_button;
    delete m_dialog_layout;
    delete m_dialog_display_selected_task_data;

    m_l_task_name = nullptr;
    m_le_task_name = nullptr;
    m_task_name_layout = nullptr;
    m_l_task_comments = nullptr;
    m_te_task_name = nullptr;
    m_task_comments_layout = nullptr;
    m_close_button = nullptr;
    m_dialog_layout = nullptr;
    m_dialog_display_selected_task_data = nullptr;
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
