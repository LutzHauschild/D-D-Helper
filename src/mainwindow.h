#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include "initiativetracker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Die MainWindow-Klasse stellt die Hauptbenutzeroberfläche der Anwendung dar.
 * 
 * Diese Klasse ist für die Darstellung der Benutzeroberfläche und die Interaktion
 * mit dem Benutzer verantwortlich. Sie verwaltet die UI-Elemente und verbindet sie
 * mit dem InitiativeTracker, der die eigentliche Logik enthält.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor für das Hauptfenster.
     * 
     * Initialisiert die UI und richtet die Verbindungen zwischen den UI-Elementen
     * und den Slots ein. Lädt auch die gespeicherten Charakterdaten, falls vorhanden.
     * 
     * @param parent Das Elternobjekt für die Qt-Objekthierarchie
     */
    MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Destruktor für das Hauptfenster.
     * 
     * Gibt die von der UI verwendeten Ressourcen frei.
     */
    ~MainWindow();

protected:
    /**
     * @brief Ereignishandler für das Schließen des Fensters.
     * 
     * Wird aufgerufen, wenn der Benutzer das Fenster schließt.
     * Speichert die Charakterdaten, bevor das Fenster geschlossen wird.
     * 
     * @param event Das Schließen-Ereignis
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Hinzufügen"-Button geklickt wird.
     * 
     * Liest die Eingabefelder aus und fügt einen neuen Charakter zum InitiativeTracker hinzu.
     */
    void on_addButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Initiative würfeln"-Button geklickt wird.
     * 
     * Würfelt die Initiative für alle Charaktere im InitiativeTracker.
     */
    void on_rollButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Liste leeren"-Button geklickt wird.
     * 
     * Entfernt alle Charaktere aus dem InitiativeTracker nach Bestätigung durch den Benutzer.
     */
    void on_clearButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Ausgewählten entfernen"-Button geklickt wird.
     * 
     * Entfernt den ausgewählten Charakter aus dem InitiativeTracker.
     */
    void on_removeButton_clicked();
    
    /**
     * @brief Aktualisiert die Charaktertabelle mit den aktuellen Daten aus dem InitiativeTracker.
     * 
     * Wird aufgerufen, wenn sich die Charakterliste ändert oder die Initiative gewürfelt wurde.
     */
    void updateCharacterTable();

private:
    Ui::MainWindow *ui;                   ///< Zeiger auf die UI-Elemente
    InitiativeTracker m_initiativeTracker; ///< Der InitiativeTracker, der die Charaktere verwaltet
    
    /**
     * @brief Lädt die gespeicherten Charakterdaten.
     * 
     * Wird beim Start der Anwendung aufgerufen, um die zuvor gespeicherten
     * Charakterdaten zu laden.
     */
    void loadCharacters();
    
    /**
     * @brief Speichert die aktuellen Charakterdaten.
     * 
     * Wird beim Beenden der Anwendung aufgerufen, um die Charakterdaten
     * für den nächsten Start zu speichern.
     */
    void saveCharacters();
};
#endif // MAINWINDOW_H 