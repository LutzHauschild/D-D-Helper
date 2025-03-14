#include "mainwindow.h"  // Einbinden der MainWindow-Klasse

// Qt-Includes für die Anwendung
#include <QApplication>  // Hauptklasse für Qt-Anwendungen mit GUI
#include <QLocale>       // Klasse für Lokalisierungsinformationen
#include <QTranslator>   // Klasse für Übersetzungen

/**
 * @brief Hauptfunktion der Anwendung
 * 
 * Initialisiert die Qt-Anwendung, richtet Übersetzungen ein und
 * zeigt das Hauptfenster an.
 * 
 * C++ Konzept: Einstiegspunkt
 * Die main()-Funktion ist der Einstiegspunkt jedes C++-Programms.
 * Sie wird automatisch aufgerufen, wenn das Programm gestartet wird.
 * 
 * C++ Konzept: Kommandozeilenargumente
 * argc enthält die Anzahl der Kommandozeilenargumente.
 * argv ist ein Array von Zeichenketten, das die Argumente enthält.
 * Diese werden an die QApplication weitergegeben, damit Qt sie verarbeiten kann.
 * 
 * @param argc Anzahl der Kommandozeilenargumente
 * @param argv Array der Kommandozeilenargumente
 * @return Rückgabewert der Anwendung (0 bei normalem Beenden)
 */
int main(int argc, char *argv[])
{
    // QApplication-Objekt erstellen
    // Qt-Konzept: QApplication
    // QApplication verwaltet die Hauptereignisschleife und ist für
    // GUI-Anwendungen erforderlich. Sie initialisiert und bereinigt
    // alle Qt-Ressourcen und verwaltet die Ereignisverarbeitung.
    QApplication a(argc, argv);
    
    // Übersetzungen einrichten
    // Qt-Konzept: Internationalisierung (i18n)
    // Qt bietet umfassende Unterstützung für mehrsprachige Anwendungen.
    // QTranslator lädt Übersetzungsdateien (.qm) und installiert sie in der Anwendung.
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    
    // Versuchen, eine passende Übersetzungsdatei zu laden
    // Qt-Konzept: Ressourcensystem
    // Der Pfad ":/i18n/" verweist auf das Qt-Ressourcensystem, das Dateien
    // direkt in die ausführbare Datei einbettet. Dies macht die Anwendung
    // portabler, da keine externen Dateien benötigt werden.
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DnDInitiativeTracker_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    
    // Hauptfenster erstellen und anzeigen
    // Qt-Konzept: Widgets und Fenster
    // MainWindow ist eine von QMainWindow abgeleitete Klasse, die das Hauptfenster
    // der Anwendung darstellt. Die show()-Methode macht das Fenster sichtbar.
    MainWindow w;
    w.show();
    
    // Hauptereignisschleife starten und auf Beenden warten
    // Qt-Konzept: Ereignisschleife (Event Loop)
    // a.exec() startet die Hauptereignisschleife, die folgende Aufgaben hat:
    // 1. Warten auf Ereignisse (Mausklicks, Tastatureingaben, Timer, etc.)
    // 2. Verteilen der Ereignisse an die entsprechenden Objekte
    // 3. Ausführen von Code als Reaktion auf Ereignisse
    // 4. Aktualisieren der Benutzeroberfläche
    // Die Schleife läuft, bis die Anwendung beendet wird (z.B. durch Schließen des Hauptfensters).
    // Der Rückgabewert von exec() wird als Rückgabewert der main()-Funktion verwendet.
    return a.exec();
} 