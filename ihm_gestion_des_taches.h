#ifndef IHMGestionDesTaches_H
#define IHMGestionDesTaches_H

// Includes
#include <QDialog>
#include <map>
#include <cstring>

#include "ui_fenetre_gestion_des_taches.h"

// Définition de la classe "IHMGestionDesTaches"
class IHMGestionDesTaches : public QDialog, public Ui::FenetreGestionDesTaches
{
    Q_OBJECT

    public:
        // Constructeurs et destructeur
            IHMGestionDesTaches(QWidget *parent = nullptr);
            virtual ~IHMGestionDesTaches();

        // connexions
            void connexions() const;

        // modifications de certains widgets
            void dialog_initialization_for_task_modification(std::map<std::string, std::string> *data_from_DB);

        // accesseurs
            QString get_task_name() const;
            QString get_task_importance() const;
            QDate get_task_date() const;
            bool get_periodic_task_state()const;
            int get_task_periodicity() const;
            bool get_reminder_state() const;
            int get_nbr_weeks_before_task() const;
            QString get_comments() const;

    public slots:

    protected:
        Ui::FenetreGestionDesTaches *ui;
};
#endif // IHMGestionDesTaches_H
