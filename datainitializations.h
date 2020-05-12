#ifndef DATAINITIALIZATIONS_H
#define DATAINITIALIZATIONS_H

#include <string>
#include <QString>

class DataInitializations
{
    public:
        // Constructor and destructor
            DataInitializations();
            virtual ~DataInitializations();

        // Getters
            QString get_database_complete_path();
            bool get_database_folder_automatically_initiated();

        // Methods
            void retrieve_execution_path();
            int retrieve_last_limiter_position(std::string const& complete_path);
            void initialize_database_path();
            bool database_folder_existency_and_creation();

    protected:
        // Attributes
            QString m_complete_exeuctable_path;
            QString m_database_complete_path;
            bool m_database_folder_automatically_initiated;
};

#endif // DATAINITIALIZATIONS_H
