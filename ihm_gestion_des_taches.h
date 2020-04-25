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
        // Constructor et destructor
            IHMGestionDesTaches(QWidget *parent = nullptr);
            virtual ~IHMGestionDesTaches();

        // connections
            void connexions() const;

        // modifications de certains widgets
            void dialog_initialization_for_task_modification(std::map<std::string, std::string> *data_from_DB);

        // Getters
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

    protected:
        Ui::FenetreGestionDesTaches *ui;
};
#endif // IHMGestionDesTaches_H
