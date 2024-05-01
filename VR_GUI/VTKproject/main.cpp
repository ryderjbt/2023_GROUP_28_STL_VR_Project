#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize translator for internationalization
    QTranslator translator;

    // Get the system's preferred UI languages
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    // Iterate through the preferred UI languages to find the appropriate translation file
    for (const QString &locale : uiLanguages) {
        const QString baseName = "VTKproject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            // If translation file is found, install the translator and break the loop
            a.installTranslator(&translator);
            break;
        }
    }

    // Create and show the main window
    MainWindow w;
    w.show();

    // Start the application event loop
    return a.exec();
}
