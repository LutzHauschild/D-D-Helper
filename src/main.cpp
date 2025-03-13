#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

/**
 * @brief Hauptfunktion der Anwendung
 * 
 * Initialisiert die Qt-Anwendung, richtet Übersetzungen ein und
 * zeigt das Hauptfenster an.
 * 
 * @param argc Anzahl der Kommandozeilenargumente
 * @param argv Array der Kommandozeilenargumente
 * @return Rückgabewert der Anwendung (0 bei normalem Beenden)
 */
int main(int argc, char *argv[])
{
    // QApplication-Objekt erstellen (verwaltet die Hauptereignisschleife)
    QApplication a(argc, argv);
    
    // Übersetzungen einrichten
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    
    // Versuchen, eine passende Übersetzungsdatei zu laden
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DnDInitiativeTracker_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    
    // Hauptfenster erstellen und anzeigen
    MainWindow w;
    w.show();
    
    // Hauptereignisschleife starten und auf Beenden warten
    return a.exec();
} 