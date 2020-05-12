#include "mainwindow.h"
#include "datainitializations.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    // Application initialization
    QApplication app(argc, argv);

    // The following lines enable to have the buttons text in french
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    // DataInitialization object instance creation
    DataInitializations data_initialization_instance;

    // Check if the "database" folder, containing program data, exists
    // if this folder does not exist the user can choose to create it automatically
    if ( data_initialization_instance.database_folder_existency_and_creation() )
    {
        MainWindow main_window(data_initialization_instance.get_database_complete_path(),
                               data_initialization_instance.get_database_folder_automatically_initiated());

        main_window.show();

        return app.exec();
    }
    else
    {
        return 0;
    }
}
