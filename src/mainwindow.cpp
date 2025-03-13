#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

/**
 * @brief Konstruktor für das Hauptfenster
 * 
 * Initialisiert die UI, richtet die Verbindungen zwischen den Signalen des
 * InitiativeTrackers und den Slots des Hauptfensters ein und konfiguriert
 * die Tabelle für die Charakteranzeige.
 * 
 * @param parent Das Elternobjekt für die Qt-Objekthierarchie
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_initiativeTracker(this)
{
    // Lädt und initialisiert die UI aus der .ui-Datei
    ui->setupUi(this);
    
    // Verbindungen zwischen den Signalen des InitiativeTrackers und den Slots des Hauptfensters herstellen
    connect(&m_initiativeTracker, &InitiativeTracker::charactersChanged, this, &MainWindow::updateCharacterTable);
    connect(&m_initiativeTracker, &InitiativeTracker::initiativeRolled, this, &MainWindow::updateCharacterTable);
    
    // Tabelle konfigurieren
    ui->characterTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // Ganze Zeilen auswählen
    ui->characterTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Bearbeitung in der Tabelle deaktivieren
    ui->characterTableWidget->horizontalHeader()->setStretchLastSection(true);      // Letzte Spalte dehnen
    ui->characterTableWidget->verticalHeader()->setVisible(false);                  // Zeilennummern ausblenden
    
    // Gespeicherte Charaktere laden
    loadCharacters();
    
    // Anfangszustand der Tabelle aktualisieren
    updateCharacterTable();
}

/**
 * @brief Destruktor für das Hauptfenster
 * 
 * Gibt die von der UI verwendeten Ressourcen frei.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Ereignishandler für das Schließen des Fensters
 * 
 * Wird aufgerufen, wenn der Benutzer das Fenster schließt.
 * Speichert die Charakterdaten, bevor das Fenster geschlossen wird.
 * 
 * @param event Das Schließen-Ereignis
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    // Charaktere speichern
    saveCharacters();
    
    // Standardverhalten beibehalten (Fenster schließen)
    QMainWindow::closeEvent(event);
}

/**
 * @brief Lädt die gespeicherten Charakterdaten
 * 
 * Wird beim Start der Anwendung aufgerufen, um die zuvor gespeicherten
 * Charakterdaten zu laden.
 */
void MainWindow::loadCharacters()
{
    // Versucht, die Charakterdaten aus der Datei zu laden
    bool success = m_initiativeTracker.loadFromFile();
    
    if (success) {
        qDebug() << "Charakterdaten erfolgreich geladen.";
    } else {
        qDebug() << "Keine gespeicherten Charakterdaten gefunden oder Fehler beim Laden.";
    }
}

/**
 * @brief Speichert die aktuellen Charakterdaten
 * 
 * Wird beim Beenden der Anwendung aufgerufen, um die Charakterdaten
 * für den nächsten Start zu speichern.
 */
void MainWindow::saveCharacters()
{
    // Speichert die Charakterdaten in der Datei
    bool success = m_initiativeTracker.saveToFile();
    
    if (success) {
        qDebug() << "Charakterdaten erfolgreich gespeichert.";
    } else {
        qDebug() << "Fehler beim Speichern der Charakterdaten.";
    }
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Hinzufügen"-Button geklickt wird
 * 
 * Liest die Eingabefelder aus, überprüft, ob ein Name eingegeben wurde,
 * und fügt einen neuen Charakter zum InitiativeTracker hinzu.
 */
void MainWindow::on_addButton_clicked()
{
    // Eingabefelder auslesen
    QString name = ui->nameEdit->text().trimmed();  // Leerzeichen am Anfang und Ende entfernen
    int initiativeModifier = ui->initiativeModSpinBox->value();
    
    // Überprüfen, ob ein Name eingegeben wurde
    if (name.isEmpty()) {
        QMessageBox::warning(this, tr("Fehler"), tr("Bitte geben Sie einen Namen ein."));
        return;
    }
    
    // Neuen Charakter erstellen und zum InitiativeTracker hinzufügen
    Character character(name, initiativeModifier);
    m_initiativeTracker.addCharacter(character);
    
    // Eingabefelder zurücksetzen
    ui->nameEdit->clear();
    ui->initiativeModSpinBox->setValue(0);
    ui->nameEdit->setFocus();  // Fokus zurück auf das Namensfeld setzen
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Initiative würfeln"-Button geklickt wird
 * 
 * Überprüft, ob Charaktere vorhanden sind, und würfelt dann die Initiative
 * für alle Charaktere im InitiativeTracker.
 */
void MainWindow::on_rollButton_clicked()
{
    // Überprüfen, ob Charaktere vorhanden sind
    if (m_initiativeTracker.getCharacters().isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Fügen Sie zuerst Charaktere hinzu."));
        return;
    }
    
    // Initiative für alle Charaktere würfeln
    m_initiativeTracker.rollAllInitiatives();
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Liste leeren"-Button geklickt wird
 * 
 * Überprüft, ob Charaktere vorhanden sind, fragt den Benutzer nach Bestätigung
 * und entfernt dann alle Charaktere aus dem InitiativeTracker.
 */
void MainWindow::on_clearButton_clicked()
{
    // Überprüfen, ob Charaktere vorhanden sind
    if (m_initiativeTracker.getCharacters().isEmpty()) {
        return;
    }
    
    // Benutzer nach Bestätigung fragen
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Bestätigung"),
                                                             tr("Möchten Sie wirklich alle Charaktere entfernen?"),
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Alle Charaktere entfernen
        m_initiativeTracker.clearCharacters();
    }
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Ausgewählten entfernen"-Button geklickt wird
 * 
 * Überprüft, ob eine Zeile ausgewählt ist, und entfernt dann den entsprechenden
 * Charakter aus dem InitiativeTracker.
 */
void MainWindow::on_removeButton_clicked()
{
    // Ausgewählte Zeile ermitteln
    int currentRow = ui->characterTableWidget->currentRow();
    if (currentRow >= 0) {
        // Charakter an der ausgewählten Position entfernen
        m_initiativeTracker.removeCharacter(currentRow);
    } else {
        // Fehlermeldung anzeigen, wenn keine Zeile ausgewählt ist
        QMessageBox::information(this, tr("Information"), tr("Bitte wählen Sie einen Charakter aus."));
    }
}

/**
 * @brief Aktualisiert die Charaktertabelle mit den aktuellen Daten aus dem InitiativeTracker
 * 
 * Leert die Tabelle und füllt sie mit den nach Initiative sortierten Charakteren
 * aus dem InitiativeTracker. Wird aufgerufen, wenn sich die Charakterliste ändert
 * oder die Initiative gewürfelt wurde.
 */
void MainWindow::updateCharacterTable()
{
    // Tabelle leeren
    ui->characterTableWidget->setRowCount(0);
    
    // Sortierte Charakterliste abrufen
    QVector<Character> sortedCharacters = m_initiativeTracker.getSortedInitiativeOrder();
    
    // Tabelle mit den sortierten Charakteren füllen
    for (const auto &character : sortedCharacters) {
        int row = ui->characterTableWidget->rowCount();
        ui->characterTableWidget->insertRow(row);
        
        // Charakterdaten in die Tabelle eintragen
        ui->characterTableWidget->setItem(row, 0, new QTableWidgetItem(character.getName()));
        ui->characterTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(character.getInitiativeModifier())));
        ui->characterTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(character.getInitiativeRoll())));
        ui->characterTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(character.getTotalInitiative())));
    }
    
    // Spaltenbreiten an den Inhalt anpassen
    ui->characterTableWidget->resizeColumnsToContents();
} 