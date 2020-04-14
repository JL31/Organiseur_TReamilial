#include "taskbutton.h"

// Constructeur
TaskButton::TaskButton(QString const& text, int const& task_number, QString const& task_importance, QString const& task_comments, bool const& reminder_state, bool const& is_checkable, bool const& is_periodic, QWidget *parent, bool const& panel_task, int const& nbr_weeks_before_tasks):
                       QPushButton(text, parent),
                       m_taille(new int(85)),
                       m_task_number(new int(task_number)),
                       m_is_task_periodic(new bool(is_periodic)),
                       m_task_name(new QString(text)),
                       m_task_comments(new QString(task_comments))
{
    this->setMinimumSize(*m_taille, *m_taille);
    this->setMaximumSize(*m_taille, *m_taille);

    if ( is_checkable )
    {
        this->setCheckable(true);
    }
    else
    {
        this->setCheckable(false);
    }

    adapt_button_text(text, reminder_state, nbr_weeks_before_tasks, panel_task);
    set_button_color(task_importance, reminder_state, panel_task);

    connect(this, SIGNAL(toggled(bool)), this, SLOT(emit_specific_signal(bool)));
}

// Destructeur
TaskButton::~TaskButton()
{
    delete m_taille;
    delete m_task_number;
    delete m_is_task_periodic;
    delete m_task_name;
    delete m_task_comments;

    m_taille = nullptr;
    m_task_number = nullptr;
    m_is_task_periodic = nullptr;
    m_task_name = nullptr;
    m_task_comments = nullptr;
}

// Double-clic sur le bouton
void TaskButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    QDialog *m_dialog_display_selected_task_data = new QDialog(this);
    m_dialog_display_selected_task_data->setWindowTitle("Tâche n°" + QString::number(*m_task_number));

    QLabel *m_l_task_name = new QLabel("Nom de la tâche");
    QLineEdit *m_le_task_name = new QLineEdit(*m_task_name);
    m_le_task_name->setReadOnly(true);
    QHBoxLayout *m_task_name_layout = new QHBoxLayout();
    m_task_name_layout->addWidget(m_l_task_name);
    m_task_name_layout->addWidget(m_le_task_name);

    QLabel *m_l_task_comments = new QLabel("Commentaires");
    m_l_task_comments->setFixedWidth(m_l_task_name->fontMetrics().boundingRect(m_l_task_name->text()).width());
    m_l_task_comments->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QTextEdit *m_te_task_name = new QTextEdit(*m_task_comments);
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
    m_dialog_display_selected_task_data->setStyleSheet("background-color:None");    // pour éviter que la fenêtre n'hérite de la couleur de la fenêtre parente (dans le cas d'une tâche Importante ou d'un rappel)
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

// Adapter texte du bouton
void TaskButton::adapt_button_text(QString const & text, bool const& reminder_state, int const& nbr_weeks_before_tasks, bool const& panel_task)
{
    QString *new_texte = new QString();

    if ( reminder_state and not this->isCheckable() and not panel_task )
    {
        new_texte->append("Rappel de la\n");
        new_texte->append("tâche n°");
        new_texte->append(QString::number(*m_task_number));
        new_texte->append("\n\n");
        new_texte->append("Prévue dans\n");
        new_texte->append(QString::number(nbr_weeks_before_tasks));
        new_texte->append(" semaine");

        if ( nbr_weeks_before_tasks > 1)
        {
            new_texte->append("s");
        }
    }
    else
    {
        new_texte->append("Tâche n°");
        new_texte->append(QString::number(*m_task_number));
        new_texte->append("\n\n");
        new_texte->append(text);
    }

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

    this->setText(*new_texte);

    delete new_texte;
    new_texte = nullptr;
}

// SLOT
void TaskButton::emit_specific_signal(bool const& button_state)
{
    if ( button_state )
    {
        emit button_state_and_task_number(*m_task_number);
    }
    else
    {
        emit button_task_number(*m_task_number);
    }
}

// Accessor of the "m_task_number" attribute
int TaskButton::get_task_button_task_number() const
{
    return *m_task_number;
}

// Accessor of the "m_is_task_periodic" attribute
bool TaskButton::get_task_button_is_periodic_task() const
{
    return *m_is_task_periodic;
}

//
void TaskButton::set_button_color(QString const& task_importance, bool const& reminder_state, bool const& panel_task)
{
    QDate *tmp_date = new QDate(QDate::currentDate());

    if ( reminder_state and not this->isCheckable() and not panel_task )
    {
        this->setStyleSheet("background-color:rgb(0, 150, 220)");
    }
    else if ( task_importance == "Importante" )
    {
        this->setStyleSheet("background-color:orange");
    }

    delete tmp_date;
    tmp_date = nullptr;
}
