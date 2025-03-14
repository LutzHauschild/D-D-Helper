#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QPushButton>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QFileDialog>
#include <QCloseEvent>
#include <QColor>
#include <QBrush>
#include <QFont>

// Füge die Namespace-Deklaration für die UI-Klasse hinzu
namespace Ui {
    class MainWindow;
}

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
    
    // Erstelle das Datenmodell für die Tabelle
    m_model = new QStandardItemModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    
    // Setze die Spaltenüberschriften
    QStringList headers;
    headers << "Name" 
            << "Initiative Mod" << "Initiative Ergebnis" << "Würfeln"
            << "Willenskraft" << "Will Ergebnis" << "Würfeln"
            << "Reflex" << "Reflex Ergebnis" << "Würfeln"
            << "Konstitution" << "Konst. Ergebnis" << "Würfeln";
    m_model->setHorizontalHeaderLabels(headers);
    // Konfiguriere die TableView
    ui->characterTableView->setModel(m_proxyModel);
    ui->characterTableView->setSortingEnabled(true);
    ui->characterTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->characterTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->characterTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->characterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->characterTableView->horizontalHeader()->setStretchLastSection(true);
    ui->characterTableView->setAlternatingRowColors(true);
    
    // Verbinde Signale und Slots
    connect(&m_initiativeTracker, &InitiativeTracker::charactersChanged, this, &MainWindow::onCharactersChanged);
    connect(&m_initiativeTracker, &InitiativeTracker::initiativeRolled, this, &MainWindow::onInitiativeRolled);
    connect(&m_initiativeTracker, &InitiativeTracker::savesRolled, this, &MainWindow::onSavesRolled);
    
    // Lade gespeicherte Charaktere, falls vorhanden
    if (m_initiativeTracker.loadFromFile()) {
        qDebug() << "Charakterdaten erfolgreich geladen.";
    }
    
    // Aktualisiere die Tabelle
    updateTable();
}

/**
 * @brief Destruktor für das Hauptfenster
 * 
 * Gibt die von der UI verwendeten Ressourcen frei.
 */
MainWindow::~MainWindow()
{
    // Speichere die Charaktere beim Beenden
    if (m_initiativeTracker.saveToFile()) {
        qDebug() << "Charakterdaten erfolgreich gespeichert.";
    }
    
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
    // Hole die Werte aus den Eingabefeldern
    QString name = ui->nameLineEdit->text().trimmed();
    int initiativeModifier = ui->initiativeSpinBox->value();
    int willSave = ui->willSpinBox->value();
    int reflexSave = ui->reflexSpinBox->value();
    int fortitudeSave = ui->fortitudeSpinBox->value();
    
    // Überprüfe, ob ein Name eingegeben wurde
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Fehler", "Bitte geben Sie einen Namen ein.");
        return;
    }
    
    // Erstelle einen neuen Charakter und füge ihn hinzu
    Character character(name, initiativeModifier, willSave, reflexSave, fortitudeSave);
    m_initiativeTracker.addCharacter(character);
    
    // Leere die Eingabefelder
    ui->nameLineEdit->clear();
    ui->initiativeSpinBox->setValue(0);
    ui->willSpinBox->setValue(0);
    ui->reflexSpinBox->setValue(0);
    ui->fortitudeSpinBox->setValue(0);
    ui->nameLineEdit->setFocus();
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Entfernen"-Button geklickt wird
 * 
 * Überprüft, ob eine Zeile ausgewählt ist, und entfernt dann den entsprechenden
 * Charakter aus dem InitiativeTracker.
 */
void MainWindow::on_removeButton_clicked()
{
    // Hole den ausgewählten Index
    QModelIndex proxyIndex = ui->characterTableView->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "Fehler", "Bitte wählen Sie einen Charakter aus.");
        return;
    }
    
    // Konvertiere den Proxy-Index in den Quellindex
    QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);
    int row = sourceIndex.row();
    
    // Entferne den Charakter
    m_initiativeTracker.removeCharacter(row);
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Alle löschen"-Button geklickt wird
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
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Bestätigung",
                                                             "Möchten Sie wirklich alle Charaktere entfernen?",
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Alle Charaktere entfernen
        m_initiativeTracker.clearCharacters();
    }
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Initiative würfeln"-Button geklickt wird
 * 
 * Überprüft, ob Charaktere vorhanden sind, und würfelt dann die Initiative
 * für alle Charaktere im InitiativeTracker.
 */
void MainWindow::on_rollInitiativeButton_clicked()
{
    // Überprüfen, ob Charaktere vorhanden sind
    if (m_initiativeTracker.getCharacters().isEmpty()) {
        QMessageBox::information(this, "Information", "Fügen Sie zuerst Charaktere hinzu.");
        return;
    }
    
    // Initiative für alle Charaktere würfeln
    m_initiativeTracker.rollAllInitiatives();
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Speichern"-Button geklickt wird
 * 
 * Öffnet einen Datei-Dialog zum Speichern der Charakterdaten.
 */
void MainWindow::on_saveButton_clicked()
{
    // Öffne einen Datei-Dialog zum Speichern
    QString filename = QFileDialog::getSaveFileName(
        this, "Charaktere speichern", "", "JSON-Dateien (*.json);;Alle Dateien (*)");
    
    if (!filename.isEmpty()) {
        if (m_initiativeTracker.saveToFile(filename)) {
            QMessageBox::information(this, "Erfolg", "Charaktere erfolgreich gespeichert.");
        } else {
            QMessageBox::warning(this, "Fehler", "Fehler beim Speichern der Charaktere.");
        }
    }
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Laden"-Button geklickt wird
 * 
 * Öffnet einen Datei-Dialog zum Laden der Charakterdaten.
 */
void MainWindow::on_loadButton_clicked()
{
    // Öffne einen Datei-Dialog zum Laden
    QString filename = QFileDialog::getOpenFileName(
        this, "Charaktere laden", "", "JSON-Dateien (*.json);;Alle Dateien (*)");
    
    if (!filename.isEmpty()) {
        if (m_initiativeTracker.loadFromFile(filename)) {
            QMessageBox::information(this, "Erfolg", "Charaktere erfolgreich geladen.");
        } else {
            QMessageBox::warning(this, "Fehler", "Fehler beim Laden der Charaktere.");
        }
    }
}

void MainWindow::onCharactersChanged()
{
    // Aktualisiere die Tabelle, wenn sich die Charakterliste ändert
    updateTable();
}

void MainWindow::onInitiativeRolled()
{
    // Aktualisiere die Tabelle
    updateTable();
    
    // Sortiere nach Initiative
    ui->characterTableView->sortByColumn(TOTAL_INITIATIVE_COLUMN, Qt::DescendingOrder);
    
    // Aktualisiere die Buttons nach dem Sortieren
    QVector<Character> characters = m_initiativeTracker.getCharacters();
    for (int i = 0; i < characters.size(); ++i) {
        const Character &character = characters[i];
        
        // Finde die tatsächliche Zeile nach dem Sortieren
        QModelIndex sourceIndex = m_model->index(i, 0);
        QModelIndex proxyIndex = m_proxyModel->mapFromSource(sourceIndex);
        int visualRow = proxyIndex.row();
        
        // Erstelle die Buttons für die Würfelwürfe
        createRollButton(visualRow, ROLL_INITIATIVE_COLUMN, "d20", character.getInitiativeModifier(), "Würfeln");
        createRollButton(visualRow, ROLL_WILL_COLUMN, "will", character.getWillSave(), "Würfeln");
        createRollButton(visualRow, ROLL_REFLEX_COLUMN, "reflex", character.getReflexSave(), "Würfeln");
        createRollButton(visualRow, ROLL_FORTITUDE_COLUMN, "fortitude", character.getFortitudeSave(), "Würfeln");
    }
}

void MainWindow::onSavesRolled()
{
    // Aktualisiere die Tabelle, wenn Rettungswürfe gewürfelt wurden
    updateTable();
}

void MainWindow::updateTable()
{
    // Speichere den aktuell ausgewählten Index
    QModelIndex currentIndex = ui->characterTableView->currentIndex();
    
    // Lösche alle Zeilen im Modell
    m_model->removeRows(0, m_model->rowCount());
    
    // Hole die Charakterliste
    QVector<Character> characters = m_initiativeTracker.getCharacters();
    
    // Füge jeden Charakter zur Tabelle hinzu
    for (int i = 0; i < characters.size(); ++i) {
        const Character &character = characters[i];
        
        // Erstelle die Items für die Zeile
        QList<QStandardItem*> rowItems;
        
        // Name
        QStandardItem *nameItem = new QStandardItem(character.getName());
        QFont boldFont = nameItem->font();
        boldFont.setBold(true);
        nameItem->setFont(boldFont);
        rowItems.append(nameItem);
        
        // Initiative Modifier
        QStandardItem *initiativeModItem = new QStandardItem(QString::number(character.getInitiativeModifier()));
        rowItems.append(initiativeModItem);
        
        // Initiative Ergebnis
        QStandardItem *totalInitiativeItem = new QStandardItem();
        if (character.getInitiativeRoll() > 0) {
            totalInitiativeItem->setText(QString("%1 (%2)").arg(character.getTotalInitiative())
                                        .arg(character.getInitiativeRoll()));
            totalInitiativeItem->setForeground(QBrush(QColor(0, 100, 0))); // Dunkelgrün
        } else {
            totalInitiativeItem->setText("-");
        }
        QFont totalFont = totalInitiativeItem->font();
        totalFont.setBold(true);
        totalInitiativeItem->setFont(totalFont);
        rowItems.append(totalInitiativeItem);
        
        // Initiative würfeln (leere Zelle für den Button)
        QStandardItem *rollInitiativeItem = new QStandardItem();
        rowItems.append(rollInitiativeItem);
        
        // Willenskraft Modifikator
        QStandardItem *willSaveItem = new QStandardItem(QString::number(character.getWillSave()));
        rowItems.append(willSaveItem);
        
        // Willenskraft Ergebnis
        QStandardItem *willResultItem = new QStandardItem();
        if (character.getLastWillSaveRoll() > 0) {
            willResultItem->setText(QString("%1 (%2)").arg(character.getLastWillSaveRoll() + character.getWillSave())
                                   .arg(character.getLastWillSaveRoll()));
            willResultItem->setForeground(QBrush(QColor(0, 100, 0))); // Dunkelgrün
        } else {
            willResultItem->setText("-");
        }
        rowItems.append(willResultItem);
        
        // Willenskraft würfeln (leere Zelle für den Button)
        QStandardItem *rollWillItem = new QStandardItem();
        rowItems.append(rollWillItem);
        
        // Reflex Modifikator
        QStandardItem *reflexSaveItem = new QStandardItem(QString::number(character.getReflexSave()));
        rowItems.append(reflexSaveItem);
        
        // Reflex Ergebnis
        QStandardItem *reflexResultItem = new QStandardItem();
        if (character.getLastReflexSaveRoll() > 0) {
            reflexResultItem->setText(QString("%1 (%2)").arg(character.getLastReflexSaveRoll() + character.getReflexSave())
                                     .arg(character.getLastReflexSaveRoll()));
            reflexResultItem->setForeground(QBrush(QColor(0, 100, 0))); // Dunkelgrün
        } else {
            reflexResultItem->setText("-");
        }
        rowItems.append(reflexResultItem);
        
        // Reflex würfeln (leere Zelle für den Button)
        QStandardItem *rollReflexItem = new QStandardItem();
        rowItems.append(rollReflexItem);
        
        // Konstitution Modifikator
        QStandardItem *fortitudeSaveItem = new QStandardItem(QString::number(character.getFortitudeSave()));
        rowItems.append(fortitudeSaveItem);
        
        // Konstitution Ergebnis
        QStandardItem *fortitudeResultItem = new QStandardItem();
        if (character.getLastFortitudeSaveRoll() > 0) {
            fortitudeResultItem->setText(QString("%1 (%2)").arg(character.getLastFortitudeSaveRoll() + character.getFortitudeSave())
                                        .arg(character.getLastFortitudeSaveRoll()));
            fortitudeResultItem->setForeground(QBrush(QColor(0, 100, 0))); // Dunkelgrün
        } else {
            fortitudeResultItem->setText("-");
        }
        rowItems.append(fortitudeResultItem);
        
        // Konstitution würfeln (leere Zelle für den Button)
        QStandardItem *rollFortitudeItem = new QStandardItem();
        rowItems.append(rollFortitudeItem);
        
        // Füge die Zeile zum Modell hinzu
        m_model->appendRow(rowItems);
    }
    
    // Erstelle die Buttons für alle Zeilen
    for (int i = 0; i < characters.size(); ++i) {
        const Character &character = characters[i];
        
        // Erstelle die Buttons für die Würfelwürfe
        createRollButton(i, ROLL_INITIATIVE_COLUMN, "d20", character.getInitiativeModifier(), "Würfeln");
        createRollButton(i, ROLL_WILL_COLUMN, "will", character.getWillSave(), "Würfeln");
        createRollButton(i, ROLL_REFLEX_COLUMN, "reflex", character.getReflexSave(), "Würfeln");
        createRollButton(i, ROLL_FORTITUDE_COLUMN, "fortitude", character.getFortitudeSave(), "Würfeln");
    }
    
    // Stelle die Auswahl wieder her, falls möglich
    if (currentIndex.isValid() && currentIndex.row() < characters.size()) {
        ui->characterTableView->setCurrentIndex(currentIndex);
    }
}

void MainWindow::createRollButton(int row, int column, const QString &diceType, int modifier, const QString &label)
{
    // Erstelle einen Button für den Würfelwurf
    QPushButton *button = new QPushButton(label);
    button->setProperty("row", row);
    button->setProperty("column", column);
    button->setProperty("diceType", diceType);
    button->setProperty("modifier", modifier);
    
    // Verbinde den Button mit dem Slot
    connect(button, &QPushButton::clicked, this, &MainWindow::onRollDiceButtonClicked);
    
    // Setze den Button in die Tabelle
    QModelIndex index = m_proxyModel->index(row, column);
    if (index.isValid()) {
        ui->characterTableView->setIndexWidget(index, button);
    }
}

void MainWindow::onRollDiceButtonClicked()
{
    // Hole den Sender-Button
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    // Hole die Eigenschaften des Buttons
    int row = button->property("row").toInt();
    int column = button->property("column").toInt();
    QString diceType = button->property("diceType").toString();
    
    // Konvertiere den Proxy-Index in den Quellindex
    QModelIndex proxyIndex = m_proxyModel->index(row, 0);
    QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);
    int sourceRow = sourceIndex.row();
    
    // Führe den entsprechenden Würfelwurf durch
    if (diceType == "d20") {
        // Initiative würfeln
        m_initiativeTracker.rollInitiativeForCharacter(sourceRow);
    } else if (diceType == "will") {
        // Willenskraft würfeln
        m_initiativeTracker.rollWillSaveForCharacter(sourceRow);
    } else if (diceType == "reflex") {
        // Reflex würfeln
        m_initiativeTracker.rollReflexSaveForCharacter(sourceRow);
    } else if (diceType == "fortitude") {
        // Konstitution würfeln
        m_initiativeTracker.rollFortitudeSaveForCharacter(sourceRow);
    }
}

void MainWindow::on_rollWillButton_clicked()
{
    // Würfle Willenskraft-Rettungswürfe für alle Charaktere
    m_initiativeTracker.rollAllWillSaves();
}

void MainWindow::on_rollReflexButton_clicked()
{
    // Würfle Reflex-Rettungswürfe für alle Charaktere
    m_initiativeTracker.rollAllReflexSaves();
}

void MainWindow::on_rollFortitudeButton_clicked()
{
    // Würfle Konstitution-Rettungswürfe für alle Charaktere
    m_initiativeTracker.rollAllFortitudeSaves();
} 