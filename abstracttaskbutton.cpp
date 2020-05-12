#include "abstracttaskbutton.h"

// Constructor and destructor
// --------------------------

// Constructor
AbstractTaskButton::AbstractTaskButton(QString const& button_text,
                                       BaseTask *task) : m_button_text(button_text),
                                                         m_task(task)
{
    // Provisoire - début
    int taille = 95;
    this->setMinimumSize(taille, taille);
    this->setMaximumSize(taille, taille);
    this->setContentsMargins(5, 5, 5, 5);

    //
    //
    // penser à limiter le nombre de caractères pour la description de la tâche,
    // i.e. NAME, à environ 14
    //
    //

    // Provisoire - fin
}


// Destructor
AbstractTaskButton::~AbstractTaskButton()
{}


// Methods
// -------

// Action linked to the double click on the button
void AbstractTaskButton::mouseDoubleClickEvent(QMouseEvent *event)
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


// Method that enables to modify the button text
void AbstractTaskButton::adapt_button_text()
{
    // variable initialization
    QString new_texte;

    // definition of the button text
    new_texte.append("Tâche n°");
    new_texte.append(QString::number(m_task->get_number()));
    new_texte.append("\n\n");
    new_texte.append(m_button_text);

    // modification of the button text
    this->setText(new_texte);
}


// Getters
// -------

int AbstractTaskButton::get_task_number() const
{
    return m_task->get_number();
}
