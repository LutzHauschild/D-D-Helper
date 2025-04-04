#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "initiativetracker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Die MainWindow-Klasse repräsentiert das Hauptfenster der Anwendung.
 * 
 * Diese Klasse ist für die Benutzeroberfläche und die Interaktion mit dem Benutzer
 * verantwortlich. Sie verbindet die Benutzeroberfläche mit der InitiativeTracker-Klasse.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    /**
     * @brief Konstruktor für das MainWindow.
     * 
     * @param parent Der Eltern-QWidget (für die Qt-Objekthierarchie)
     */
    MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Destruktor für das MainWindow.
     * 
     * Gibt alle Ressourcen frei, die vom MainWindow verwendet werden.
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
     * Fügt einen neuen Charakter zur Liste hinzu, basierend auf den Eingabefeldern.
     */
    void on_addButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Entfernen"-Button geklickt wird.
     * 
     * Entfernt den ausgewählten Charakter aus der Liste.
     */
    void on_removeButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Alle löschen"-Button geklickt wird.
     * 
     * Entfernt alle Charaktere aus der Liste.
     */
    void on_clearButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Initiative würfeln"-Button geklickt wird.
     * 
     * Würfelt die Initiative für alle Charaktere und aktualisiert die Tabelle.
     */
    void on_rollInitiativeButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Speichern"-Button geklickt wird.
     * 
     * Speichert die Charakterliste in einer Datei.
     */
    void on_saveButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Laden"-Button geklickt wird.
     * 
     * Lädt die Charakterliste aus einer Datei.
     */
    void on_loadButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Log anzeigen/ausblenden"-Button geklickt wird.
     * 
     * Zeigt oder versteckt das Logfenster.
     */
    void on_toggleLogButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn sich die Charakterliste ändert.
     * 
     * Aktualisiert die Tabelle mit den aktuellen Charakterdaten.
     */
    void onCharactersChanged();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn die Initiative gewürfelt wurde.
     * 
     * Aktualisiert die Tabelle mit den neuen Initiative-Werten und sortiert sie.
     */
    void onInitiativeRolled();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn ein TaleSpire-Würfel-Button geklickt wird.
     * 
     * Öffnet einen TaleSpire-Würfel-Link im Standard-Browser.
     */
    void onRollDiceButtonClicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Willenskraft würfeln"-Button geklickt wird.
     * 
     * Würfelt Willenskraft-Rettungswürfe für alle Charaktere und aktualisiert die Tabelle.
     */
    void on_rollWillButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Reflex würfeln"-Button geklickt wird.
     * 
     * Würfelt Reflex-Rettungswürfe für alle Charaktere und aktualisiert die Tabelle.
     */
    void on_rollReflexButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn der "Konstitution würfeln"-Button geklickt wird.
     * 
     * Würfelt Konstitution-Rettungswürfe für alle Charaktere und aktualisiert die Tabelle.
     */
    void on_rollFortitudeButton_clicked();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn Rettungswürfe gewürfelt wurden.
     * 
     * Aktualisiert die Tabelle mit den neuen Rettungswurf-Werten.
     */
    void onSavesRolled();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn ein Item im Modell geändert wurde.
     * 
     * Aktualisiert das entsprechende Character-Objekt mit dem neuen Wert.
     * 
     * @param item Das geänderte Item
     */
    void onItemChanged(QStandardItem *item);
    
    /**
     * @brief Slot, der aufgerufen wird, wenn eine neue WebSocket-Verbindung hergestellt wird.
     */
    void onNewWebSocketConnection();
    
    /**
     * @brief Slot, der aufgerufen wird, wenn eine Nachricht von einem WebSocket empfangen wird.
     * 
     * @param message Die empfangene Nachricht
     */
    void processWebSocketMessage(const QString &message);
    
    /**
     * @brief Slot, der aufgerufen wird, wenn eine WebSocket-Verbindung getrennt wird.
     */
    void socketDisconnected();

private:
    /**
     * @brief Aktualisiert die Tabelle mit den aktuellen Charakterdaten.
     * 
     * Diese Methode wird aufgerufen, wenn sich die Charakterliste ändert oder
     * wenn die Initiative gewürfelt wurde.
     */
    void updateTable();
    
    /**
     * @brief Aktualisiert die Würfelwurf-Tabelle mit neuen Daten.
     * 
     * @param playerName Name des Spielers
     * @param diceRoll Beschreibung des Würfelwurfs
     * @param result Ergebnis des Würfelwurfs
     */
    void updateDiceRollTable(const QString &playerName, const QString &diceRoll, int result);
    
    /**
     * @brief Berechnet die Beschreibung des Würfelwurfs aus den Operanden.
     * 
     * @param operands JSON-Array der Operanden
     * @return Beschreibung des Würfelwurfs (z.B. "2d6+1d8+14")
     */
    QString calculateDiceRollDescription(const QJsonArray &operands);
    
    /**
     * @brief Berechnet das Ergebnis des Würfelwurfs aus den Operanden.
     * 
     * @param operands JSON-Array der Operanden
     * @return Gesamtergebnis des Würfelwurfs
     */
    int calculateDiceRollResult(const QJsonArray &operands);
    
    /**
     * @brief Erstellt einen TaleSpire-Würfel-Button für eine Zelle in der Tabelle.
     * 
     * @param row Die Zeile in der Tabelle
     * @param column Die Spalte in der Tabelle
     * @param diceType Der Typ des Würfels (z.B. "d20")
     * @param modifier Der Modifikator für den Würfelwurf
     * @param label Die Beschriftung des Buttons
     */
    void createRollButton(int row, int column, const QString &diceType, int modifier, const QString &label);
    
    Ui::MainWindow *ui;                      ///< Die UI-Komponenten des Hauptfensters
    InitiativeTracker m_initiativeTracker;   ///< Der Initiative-Tracker für die Charaktere
    QStandardItemModel *m_model;             ///< Das Datenmodell für die Tabelle
    QSortFilterProxyModel *m_proxyModel;     ///< Das Proxy-Modell für die Sortierung der Tabelle
    QStandardItemModel *m_diceRollModel;     ///< Das Datenmodell für die Würfelwurf-Tabelle
    
    // Spaltenindizes für die Tabelle
    static const int NAME_COLUMN = 0;
    static const int INITIATIVE_MOD_COLUMN = 1;
    static const int TOTAL_INITIATIVE_COLUMN = 2;
    static const int ROLL_INITIATIVE_COLUMN = 3;
    static const int WILL_SAVE_COLUMN = 4;
    static const int WILL_RESULT_COLUMN = 5;
    static const int ROLL_WILL_COLUMN = 6;
    static const int REFLEX_SAVE_COLUMN = 7;
    static const int REFLEX_RESULT_COLUMN = 8;
    static const int ROLL_REFLEX_COLUMN = 9;
    static const int FORTITUDE_SAVE_COLUMN = 10;
    static const int FORTITUDE_RESULT_COLUMN = 11;
    static const int ROLL_FORTITUDE_COLUMN = 12;
    
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
    
    /**
     * @brief Setzt die WebSocket-Server-Einrichtung auf.
     */
    void setupWebSocketServer();
    
    /**
     * @brief Zeigt eine empfangene Nachricht an.
     * 
     * @param message Die empfangene Nachricht
     */
    void displayReceivedMessage(const QString &message);
    
    // Neue WebSocket-Member
    QWebSocketServer *m_webSocketServer;
    QList<QWebSocket*> m_clients;
    QTextEdit *m_messageDisplay;
};

#endif // MAINWINDOW_H 