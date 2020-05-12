#include "datainitializations.h"

#include <unistd.h>
#include <QMessageBox>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <boost/filesystem.hpp>

using namespace std;


// Constructor and destructor
// --------------------------

// Constructor
DataInitializations::DataInitializations() : m_database_complete_path(""),
                                             m_database_folder_automatically_initiated(false)
{
    retrieve_execution_path();
    initialize_database_path();
}

// Destructor
DataInitializations::~DataInitializations()
{}


// Getters
// -------

QString DataInitializations::get_database_complete_path()
{
    return m_database_complete_path;
}

bool DataInitializations::get_database_folder_automatically_initiated()
{
    return m_database_folder_automatically_initiated;
}


// Methods
// -------

// Method that get the current executable path
void DataInitializations::retrieve_execution_path()
{
    char buffer[PATH_MAX];

    int bytes = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

    if ( bytes >= 0 )
    {
        buffer[bytes] = '\0';
    }

    int last_limiter_position = retrieve_last_limiter_position(buffer);

    m_complete_exeuctable_path = buffer;
    m_complete_exeuctable_path = m_complete_exeuctable_path.left(last_limiter_position);
}

// Method to get the last lmiter position in the complete executable path
int DataInitializations::retrieve_last_limiter_position(string const& complete_path)
{
    string researched_string = "/";
    vector<size_t> positions;

    size_t pos = complete_path.find(researched_string, 0);

    while( pos != string::npos )
    {
        positions.push_back(pos);
        pos = complete_path.find(researched_string, pos + 1);
    }

    return positions.back();
}

// Method to define the database complete path
void DataInitializations::initialize_database_path()
{
    m_database_complete_path = m_complete_exeuctable_path + "/database";
}

// Method to check if the database folder exists
bool DataInitializations::database_folder_existency_and_creation()
{
    struct stat st;

    if ( stat(m_database_complete_path.toStdString().c_str(), &st) == 0 and S_ISDIR(st.st_mode) == 1 )
    {
        return true;
    }
    else
    {
        QString message;
        message = "Le dossier \"database\" n'a pas été trouvé au même emplacement que ce programme.\n\n";
        message.append("Voulez-vous initialiser les données ?\n\n"
                       "Cela permettra la création :\n\n"
                       "- du dossier \"database\",\n"
                       "- de la base de données,\n"
                       "- des tables correspondantes dans la base de données ?\n");

        int validation_decision = int(0);

        validation_decision = QMessageBox::warning(nullptr, "Données inexistentes", message, QMessageBox::Yes | QMessageBox::No);

        if ( validation_decision == QMessageBox::Yes )
        {
            bool check_directory_creation;

            check_directory_creation = boost::filesystem::create_directories(m_database_complete_path.toStdString());

            if ( not check_directory_creation )
            {
                QString message = "Il y a eu un problème lors de la création du dossier \"database\" (dossier qui va contenir les données de ce programme).\n\n"
                                  "Ce dossier devait être créé à l'emplacement suivant :\n\n";
                message.append(m_complete_exeuctable_path);
                message.append("\n\n"
                               "Merci de vérifier que vous disposez bien des droits pour créer ce dossier.\n");

                QMessageBox::warning(nullptr, "Problème lors de la création du dossier contenant les données", message);

                return false;
            }
            else
            {
                m_database_folder_automatically_initiated = true;
                return true;
            }
        }
        else
        {
            return false;
        }
    }
}
