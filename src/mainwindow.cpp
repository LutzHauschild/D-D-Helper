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
#include <QSignalBlocker>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>

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
    
    // Verbinde das Signal itemChanged des Modells mit unserem Slot
    connect(m_model, &QStandardItemModel::itemChanged, this, &MainWindow::onItemChanged);
    
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
    
    // Erstelle das TextEdit für die empfangenen Nachrichten
    m_messageDisplay = ui->messageDisplay;
    m_messageDisplay->setReadOnly(true);
    m_messageDisplay->setPlaceholderText("Hier werden empfangene Nachrichten angezeigt...");
    m_messageDisplay->setVisible(true);  // Standardmäßig sichtbar
    
    // Erstelle das Datenmodell für die Würfelwurf-Tabelle
    m_diceRollModel = new QStandardItemModel(this);
    QStringList diceRollHeaders;
    diceRollHeaders << "Zeit" << "Spieler" << "Wurf" << "Ergebnis";
    m_diceRollModel->setHorizontalHeaderLabels(diceRollHeaders);
    
    // Konfiguriere die Würfelwurf-Tabelle
    ui->diceRollTableView->setModel(m_diceRollModel);
    ui->diceRollTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Spaltenbreiten anpassen
    ui->diceRollTableView->setColumnWidth(0, 80);  // Zeit
    ui->diceRollTableView->setColumnWidth(1, 100); // Spieler
    ui->diceRollTableView->setColumnWidth(2, 400); // Wurf
    ui->diceRollTableView->setColumnWidth(3, 80);  // Ergebnis
    
    ui->diceRollTableView->horizontalHeader()->setStretchLastSection(true);
    ui->diceRollTableView->setAlternatingRowColors(true);
    
    // Initialisiere den WebSocket-Server
    setupWebSocketServer();
    
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
    
    // Schließe den WebSocket-Server
    if (m_webSocketServer) {
        m_webSocketServer->close();
        qDeleteAll(m_clients);
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
    qDebug() << "on_addButton_clicked: Start";
    
    // Hole die Werte aus den Eingabefeldern
    QString name = ui->nameLineEdit->text().trimmed();
    qDebug() << "on_addButton_clicked: Name =" << name;
    
    int initiativeModifier = ui->initiativeSpinBox->value();
    qDebug() << "on_addButton_clicked: Initiative =" << initiativeModifier;
    
    int willSave = ui->willSpinBox->value();
    qDebug() << "on_addButton_clicked: Will =" << willSave;
    
    int reflexSave = ui->reflexSpinBox->value();
    qDebug() << "on_addButton_clicked: Reflex =" << reflexSave;
    
    int fortitudeSave = ui->fortitudeSpinBox->value();
    qDebug() << "on_addButton_clicked: Fortitude =" << fortitudeSave;
    
    // Überprüfe, ob ein Name eingegeben wurde
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Fehler", "Bitte geben Sie einen Namen ein.");
        qDebug() << "on_addButton_clicked: Leerer Name, Abbruch";
        return;
    }
    
    qDebug() << "on_addButton_clicked: Erstelle neuen Charakter";
    // Erstelle einen neuen Charakter und füge ihn hinzu
    Character character(name, initiativeModifier, willSave, reflexSave, fortitudeSave);
    
    qDebug() << "on_addButton_clicked: Füge Charakter zum Tracker hinzu";
    m_initiativeTracker.addCharacter(character);
    
    qDebug() << "on_addButton_clicked: Leere Eingabefelder";
    // Leere die Eingabefelder
    ui->nameLineEdit->clear();
    ui->initiativeSpinBox->setValue(0);
    ui->willSpinBox->setValue(0);
    ui->reflexSpinBox->setValue(0);
    ui->fortitudeSpinBox->setValue(0);
    ui->nameLineEdit->setFocus();
    
    qDebug() << "on_addButton_clicked: Ende";
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
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Charaktere laden"), "",
        tr("JSON-Dateien (*.json);;Alle Dateien (*)"));
        
    if (!fileName.isEmpty()) {
        if (m_initiativeTracker.loadFromFile(fileName)) {
            updateTable();
            qDebug() << "Charaktere erfolgreich geladen aus:" << fileName;
        } else {
            QMessageBox::warning(this, tr("Fehler"),
                tr("Fehler beim Laden der Charaktere."));
        }
    }
}

/**
 * @brief Slot, der aufgerufen wird, wenn der "Log anzeigen/ausblenden"-Button geklickt wird.
 * 
 * Zeigt oder versteckt das Logfenster.
 */
void MainWindow::on_toggleLogButton_clicked()
{
    if (m_messageDisplay) {
        m_messageDisplay->setVisible(!m_messageDisplay->isVisible());
        ui->toggleLogButton->setText(m_messageDisplay->isVisible() ? 
            tr("Log ausblenden") : tr("Log anzeigen"));
    }
}

void MainWindow::onCharactersChanged()
{
    static bool isUpdating = false;
    
    qDebug() << "onCharactersChanged: Start";
    
    // Verhindere rekursive Aufrufe
    if (isUpdating) {
        qDebug() << "onCharactersChanged: Bereits beim Aktualisieren, ignoriere";
        return;
    }
    
    isUpdating = true;
    
    // Aktualisiere die Tabelle, wenn sich die Charakterliste ändert
    updateTable();
    
    isUpdating = false;
    
    qDebug() << "onCharactersChanged: Ende";
}

void MainWindow::onInitiativeRolled()
{
    qDebug() << "onInitiativeRolled: Start";
    
    // Aktualisiere die Tabelle
    updateTable();
    
    // Sortiere nach Initiative
    ui->characterTableView->sortByColumn(TOTAL_INITIATIVE_COLUMN, Qt::DescendingOrder);
    qDebug() << "onInitiativeRolled: Nach Initiative sortiert";
    
    // Aktualisiere die Buttons nach dem Sortieren
    QVector<Character> characters = m_initiativeTracker.getCharacters();
    qDebug() << "onInitiativeRolled: Anzahl Charaktere =" << characters.size();
    
    for (int i = 0; i < characters.size(); ++i) {
        const Character &character = characters[i];
        
        // Finde die tatsächliche Zeile nach dem Sortieren
        QModelIndex sourceIndex = m_model->index(i, 0);
        QModelIndex proxyIndex = m_proxyModel->mapFromSource(sourceIndex);
        int visualRow = proxyIndex.row();
        
        qDebug() << "onInitiativeRolled: Charakter" << i << "(" << character.getName() << ") ist jetzt in Zeile" << visualRow;
        
        // Erstelle die Buttons für die Würfelwürfe
        createRollButton(visualRow, ROLL_INITIATIVE_COLUMN, "d20", character.getInitiativeModifier(), "Würfeln");
        createRollButton(visualRow, ROLL_WILL_COLUMN, "will", character.getWillSave(), "Würfeln");
        createRollButton(visualRow, ROLL_REFLEX_COLUMN, "reflex", character.getReflexSave(), "Würfeln");
        createRollButton(visualRow, ROLL_FORTITUDE_COLUMN, "fortitude", character.getFortitudeSave(), "Würfeln");
    }
    
    qDebug() << "onInitiativeRolled: Ende";
}

void MainWindow::onSavesRolled()
{
    // Aktualisiere die Tabelle, wenn Rettungswürfe gewürfelt wurden
    updateTable();
}

void MainWindow::updateTable()
{
    qDebug() << "updateTable: Start";
    
    // Speichere den aktuell ausgewählten Index
    QModelIndex currentIndex = ui->characterTableView->currentIndex();
    qDebug() << "updateTable: Aktueller Index =" << currentIndex;
    
    // Lösche alle Zeilen im Modell
    m_model->removeRows(0, m_model->rowCount());
    qDebug() << "updateTable: Zeilen gelöscht";
    
    // Hole die Charakterliste
    QVector<Character> characters = m_initiativeTracker.getCharacters();
    qDebug() << "updateTable: Anzahl Charaktere =" << characters.size();
    
    // Aktiviere die Bearbeitung für die Tabelle, damit die Buttons angezeigt werden können
    ui->characterTableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    
    // Füge jeden Charakter zur Tabelle hinzu
    for (int i = 0; i < characters.size(); ++i) {
        qDebug() << "updateTable: Verarbeite Charakter" << i << ":" << characters[i].getName();
        const Character &character = characters[i];
        
        // Erstelle die Items für die Zeile
        QList<QStandardItem*> rowItems;
        
        // Name
        QStandardItem *nameItem = new QStandardItem(character.getName());
        QFont boldFont = nameItem->font();
        boldFont.setBold(true);
        nameItem->setFont(boldFont);
        nameItem->setEditable(false); // Name nicht editierbar
        rowItems.append(nameItem);
        
        // Initiative Modifier - editierbar
        QStandardItem *initiativeModItem = new QStandardItem(QString::number(character.getInitiativeModifier()));
        initiativeModItem->setEditable(true);
        // Speichere den ursprünglichen Wert für die Wiederherstellung
        initiativeModItem->setData(QString::number(character.getInitiativeModifier()), Qt::UserRole + 1);
        rowItems.append(initiativeModItem);
        
        // Initiative Ergebnis - nicht editierbar
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
        totalInitiativeItem->setEditable(false);
        rowItems.append(totalInitiativeItem);
        
        // Initiative würfeln (leere Zelle für den Button)
        QStandardItem *rollInitiativeItem = new QStandardItem();
        rollInitiativeItem->setEditable(false);
        rowItems.append(rollInitiativeItem);
        
        // Willenskraft Modifikator - editierbar
        QStandardItem *willSaveItem = new QStandardItem(QString::number(character.getWillSave()));
        willSaveItem->setEditable(true);
        // Speichere den ursprünglichen Wert für die Wiederherstellung
        willSaveItem->setData(QString::number(character.getWillSave()), Qt::UserRole + 1);
        rowItems.append(willSaveItem);
        
        // Willenskraft Ergebnis - nicht editierbar
        QStandardItem *willResultItem = new QStandardItem();
        if (character.getLastWillSaveRoll() > 0) {
            willResultItem->setText(QString("%1 (%2)").arg(character.getLastWillSaveRoll() + character.getWillSave())
                                   .arg(character.getLastWillSaveRoll()));
            willResultItem->setForeground(QBrush(QColor(0, 100, 0))); // Dunkelgrün
        } else {
            willResultItem->setText("-");
        }
        willResultItem->setEditable(false);
        rowItems.append(willResultItem);
        
        // Willenskraft würfeln (leere Zelle für den Button)
        QStandardItem *rollWillItem = new QStandardItem();
        rollWillItem->setEditable(false);
        rowItems.append(rollWillItem);
        
        // Reflex Modifikator - editierbar
        QStandardItem *reflexSaveItem = new QStandardItem(QString::number(character.getReflexSave()));
        reflexSaveItem->setEditable(true);
        // Speichere den ursprünglichen Wert für die Wiederherstellung
        reflexSaveItem->setData(QString::number(character.getReflexSave()), Qt::UserRole + 1);
        rowItems.append(reflexSaveItem);
        
        // Reflex Ergebnis - nicht editierbar
        QStandardItem *reflexResultItem = new QStandardItem();
        if (character.getLastReflexSaveRoll() > 0) {
            reflexResultItem->setText(QString("%1 (%2)").arg(character.getLastReflexSaveRoll() + character.getReflexSave())
                                     .arg(character.getLastReflexSaveRoll()));
            reflexResultItem->setForeground(QBrush(QColor(0, 100, 0))); // Dunkelgrün
        } else {
            reflexResultItem->setText("-");
        }
        reflexResultItem->setEditable(false);
        rowItems.append(reflexResultItem);
        
        // Reflex würfeln (leere Zelle für den Button)
        QStandardItem *rollReflexItem = new QStandardItem();
        rollReflexItem->setEditable(false);
        rowItems.append(rollReflexItem);
        
        // Konstitution Modifikator - editierbar
        QStandardItem *fortitudeSaveItem = new QStandardItem(QString::number(character.getFortitudeSave()));
        fortitudeSaveItem->setEditable(true);
        // Speichere den ursprünglichen Wert für die Wiederherstellung
        fortitudeSaveItem->setData(QString::number(character.getFortitudeSave()), Qt::UserRole + 1);
        rowItems.append(fortitudeSaveItem);
        
        // Konstitution Ergebnis - nicht editierbar
        QStandardItem *fortitudeResultItem = new QStandardItem();
        if (character.getLastFortitudeSaveRoll() > 0) {
            fortitudeResultItem->setText(QString("%1 (%2)").arg(character.getLastFortitudeSaveRoll() + character.getFortitudeSave())
                                        .arg(character.getLastFortitudeSaveRoll()));
            fortitudeResultItem->setForeground(QBrush(QColor(0, 100, 0))); // Dunkelgrün
        } else {
            fortitudeResultItem->setText("-");
        }
        fortitudeResultItem->setEditable(false);
        rowItems.append(fortitudeResultItem);
        
        // Konstitution würfeln (leere Zelle für den Button)
        QStandardItem *rollFortitudeItem = new QStandardItem();
        rollFortitudeItem->setEditable(false);
        rowItems.append(rollFortitudeItem);
        
        // Füge die Zeile zum Modell hinzu
        m_model->appendRow(rowItems);
        qDebug() << "updateTable: Zeile für Charakter" << i << "hinzugefügt";
        
        // Speichere die Zeilen-ID als Eigenschaft für jedes Item
        for (QStandardItem *item : rowItems) {
            item->setData(i, Qt::UserRole);
        }
    }
    
    qDebug() << "updateTable: Erstelle Buttons";
    // Erstelle die Buttons für alle Zeilen
    for (int i = 0; i < characters.size(); ++i) {
        const Character &character = characters[i];
        
        // Erstelle die Buttons für die Würfelwürfe
        qDebug() << "updateTable: Erstelle Buttons für Zeile" << i;
        
        createRollButton(i, ROLL_INITIATIVE_COLUMN, "d20", character.getInitiativeModifier(), "Würfeln");
        createRollButton(i, ROLL_WILL_COLUMN, "will", character.getWillSave(), "Würfeln");
        createRollButton(i, ROLL_REFLEX_COLUMN, "reflex", character.getReflexSave(), "Würfeln");
        createRollButton(i, ROLL_FORTITUDE_COLUMN, "fortitude", character.getFortitudeSave(), "Würfeln");
    }
    
    // Stelle die Auswahl wieder her, falls möglich
    if (currentIndex.isValid() && currentIndex.row() < characters.size()) {
        ui->characterTableView->setCurrentIndex(currentIndex);
        qDebug() << "updateTable: Auswahl wiederhergestellt";
    }
    
    qDebug() << "updateTable: Ende";
}

void MainWindow::createRollButton(int row, int column, const QString &diceType, int modifier, const QString &label)
{
    qDebug() << "createRollButton: Start - Zeile:" << row << "Spalte:" << column << "Typ:" << diceType;
    
    // Erstelle einen Button für den Würfelwurf
    QPushButton *button = new QPushButton(label);
    button->setProperty("row", row);
    button->setProperty("column", column);
    button->setProperty("diceType", diceType);
    button->setProperty("modifier", modifier);
    
    qDebug() << "createRollButton: Button erstellt";
    
    // Verbinde den Button mit dem Slot
    connect(button, &QPushButton::clicked, this, &MainWindow::onRollDiceButtonClicked);
    
    qDebug() << "createRollButton: Button verbunden";
    
    // Setze den Button in die Tabelle
    QModelIndex index = m_proxyModel->index(row, 0);
    qDebug() << "createRollButton: Index erstellt - gültig:" << index.isValid() << " Zeile:" << index.row() << " Spalte:" << index.column();
    
    if (index.isValid()) {
        ui->characterTableView->setIndexWidget(index, button);
        qDebug() << "createRollButton: Button in Tabelle gesetzt";
    } else {
        qDebug() << "createRollButton: Ungültiger Index, Button nicht gesetzt";
        delete button; // Verhindere Memory-Leak
    }
    
    qDebug() << "createRollButton: Ende";
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

void MainWindow::onItemChanged(QStandardItem *item)
{
    qDebug() << "onItemChanged: Start - Spalte:" << item->column();
    
    // Hole die Zeilen-ID aus den Eigenschaften des Items
    int characterIndex = item->data(Qt::UserRole).toInt();
    qDebug() << "onItemChanged: Character-Index:" << characterIndex;
    
    // Hole die Spalte des geänderten Items
    int column = item->column();
    
    // Überprüfe, ob die Spalte editierbar ist
    if (column != 1 && column != 4 && column != 7 && column != 10) {
        qDebug() << "onItemChanged: Spalte nicht editierbar, ignoriere Änderung";
        return;
    }
    
    // Hole den neuen Wert
    bool ok;
    int newValue = item->text().toInt(&ok);
    qDebug() << "onItemChanged: Neuer Wert:" << newValue << "Gültig:" << ok;
    
    // Wenn der Wert kein gültiger Integer ist, ignoriere die Änderung
    if (!ok) {
        qDebug() << "onItemChanged: Ungültiger Wert, ignoriere Änderung";
        // Blockiere Signale, um eine Endlosschleife zu vermeiden
        QSignalBlocker blocker(m_model);
        item->setText(item->data(Qt::UserRole + 1).toString()); // Stelle den ursprünglichen Wert wieder her
        return;
    }
    
    // Aktualisiere den entsprechenden Wert im Character-Objekt
    Character &character = m_initiativeTracker.getCharacterRef(characterIndex);
    
    // Blockiere Signale, um eine Endlosschleife zu vermeiden
    QSignalBlocker blocker(m_model);
    
    if (column == 1) { // Initiative Modifier
        qDebug() << "onItemChanged: Setze Initiative Modifier auf" << newValue;
        character.setInitiativeModifier(newValue);
    } else if (column == 4) { // Willenskraft
        qDebug() << "onItemChanged: Setze Willenskraft auf" << newValue;
        character.setWillSave(newValue);
    } else if (column == 7) { // Reflex
        qDebug() << "onItemChanged: Setze Reflex auf" << newValue;
        character.setReflexSave(newValue);
    } else if (column == 10) { // Konstitution
        qDebug() << "onItemChanged: Setze Konstitution auf" << newValue;
        character.setFortitudeSave(newValue);
    }
    
    qDebug() << "onItemChanged: Ende";
}

/**
 * @brief Richtet den WebSocket-Server ein
 * 
 * Erstellt und konfiguriert den WebSocket-Server, der auf eingehende
 * Verbindungen wartet und Nachrichten empfängt.
 */
void MainWindow::setupWebSocketServer()
{
    qDebug() << "setupWebSocketServer: Start";
    
    // Erstelle den WebSocket-Server
    m_webSocketServer = new QWebSocketServer(QStringLiteral("D&D Initiative Tracker Server"),
                                            QWebSocketServer::NonSecureMode, this);
    
    // Versuche, den Server auf Port 8088 zu starten
    if (m_webSocketServer->listen(QHostAddress::LocalHost, 8088)) {
        qDebug() << "WebSocket-Server gestartet auf Port 8088";
        
        // Verbinde das Signal für neue Verbindungen
        connect(m_webSocketServer, &QWebSocketServer::newConnection,
                this, &MainWindow::onNewWebSocketConnection);
        
        // Zeige eine Meldung im TextEdit an
        m_messageDisplay->append("WebSocket-Server gestartet auf ws://localhost:8088");
        m_messageDisplay->append("Warte auf Verbindungen...");
    } else {
        qDebug() << "Fehler beim Starten des WebSocket-Servers:" << m_webSocketServer->errorString();
        m_messageDisplay->append("Fehler beim Starten des WebSocket-Servers: " + m_webSocketServer->errorString());
    }
    
    qDebug() << "setupWebSocketServer: Ende";
}

/**
 * @brief Slot, der aufgerufen wird, wenn eine neue WebSocket-Verbindung hergestellt wird
 * 
 * Akzeptiert die Verbindung, verbindet die Signale und fügt den Client zur Liste hinzu.
 */
void MainWindow::onNewWebSocketConnection()
{
    qDebug() << "onNewWebSocketConnection: Neue Verbindung";
    
    // Akzeptiere die Verbindung
    QWebSocket *socket = m_webSocketServer->nextPendingConnection();
    
    // Verbinde die Signale
    connect(socket, &QWebSocket::textMessageReceived, this, &MainWindow::processWebSocketMessage);
    connect(socket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected);
    
    // Füge den Client zur Liste hinzu
    m_clients << socket;
    
    // Zeige eine Meldung im TextEdit an
    m_messageDisplay->append("Neue Verbindung hergestellt: " + socket->peerAddress().toString());
    
    qDebug() << "onNewWebSocketConnection: Client hinzugefügt";
}

/**
 * @brief Verarbeitet eine empfangene WebSocket-Nachricht
 * 
 * Parst die JSON-Nachricht und zeigt sie im TextEdit an.
 * 
 * @param message Die empfangene Nachricht
 */
void MainWindow::processWebSocketMessage(const QString &message)
{
    qDebug() << "processWebSocketMessage: Nachricht empfangen:" << message;
    
    // Zeige die Nachricht im TextEdit an
    displayReceivedMessage(message);
    
    // Versuche, die Nachricht als JSON zu parsen
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        
        // Verarbeite Würfelwurf-Nachrichten
        if (jsonObj["type"].toString() == "roll_result") {
            QJsonObject payload = jsonObj["payload"].toObject();
            QJsonObject processedData = jsonObj["processedData"].toObject();
            
            QString playerName = processedData["playerName"].toString();
            QJsonArray operants = processedData["operants"].toArray();
            
            if (!operants.isEmpty()) {
                QJsonObject firstOperant = operants[0].toObject();
                QJsonObject result = firstOperant["result"].toObject();
                QJsonArray operands = result["operands"].toArray();
                
                QString diceRoll = calculateDiceRollDescription(operands);
                int rollResult = calculateDiceRollResult(operands);
                
                updateDiceRollTable(playerName, diceRoll, rollResult);
            }
        }
        
        // Verarbeite andere Nachrichtentypen...
        if (jsonObj.contains("command")) {
            QString command = jsonObj["command"].toString();
            QWebSocket *client = qobject_cast<QWebSocket *>(sender());
            QJsonObject response;
            
            if (command == "rollInitiative") {
                // Initiative für alle Charaktere würfeln
                m_initiativeTracker.rollAllInitiatives();
                
                // Sende eine Antwort zurück
                if (client) {
                    response["status"] = "success";
                    response["message"] = "Initiative für alle Charaktere gewürfelt";
                    client->sendTextMessage(QJsonDocument(response).toJson());
                }
            }
            else if (command == "rollWillSave") {
                // Willenskraft für alle Charaktere würfeln
                m_initiativeTracker.rollAllWillSaves();
                
                // Sende eine Antwort zurück
                if (client) {
                    response["status"] = "success";
                    response["message"] = "Willenskraft für alle Charaktere gewürfelt";
                    client->sendTextMessage(QJsonDocument(response).toJson());
                }
            }
            else if (command == "rollReflexSave") {
                // Reflex für alle Charaktere würfeln
                m_initiativeTracker.rollAllReflexSaves();
                
                // Sende eine Antwort zurück
                if (client) {
                    response["status"] = "success";
                    response["message"] = "Reflex für alle Charaktere gewürfelt";
                    client->sendTextMessage(QJsonDocument(response).toJson());
                }
            }
            else if (command == "rollFortitudeSave") {
                // Konstitution für alle Charaktere würfeln
                m_initiativeTracker.rollAllFortitudeSaves();
                
                // Sende eine Antwort zurück
                if (client) {
                    response["status"] = "success";
                    response["message"] = "Konstitution für alle Charaktere gewürfelt";
                    client->sendTextMessage(QJsonDocument(response).toJson());
                }
            }
            else {
                // Unbekannter Befehl
                if (client) {
                    response["status"] = "error";
                    response["message"] = "Unbekannter Befehl: " + command;
                    client->sendTextMessage(QJsonDocument(response).toJson());
                }
            }
        }
    }
    
    qDebug() << "processWebSocketMessage: Ende";
}

/**
 * @brief Zeigt eine empfangene Nachricht im TextEdit an
 * 
 * Formatiert die Nachricht und fügt sie zum TextEdit hinzu.
 * 
 * @param message Die anzuzeigende Nachricht
 */
void MainWindow::displayReceivedMessage(const QString &message)
{
    qDebug() << "displayReceivedMessage: Start";
    
    // Formatiere die Nachricht für die Anzeige
    QString formattedMessage = QTime::currentTime().toString("[hh:mm:ss] ") + message;
    
    // Füge die Nachricht zum TextEdit hinzu
    m_messageDisplay->append(formattedMessage);
    
    qDebug() << "displayReceivedMessage: Ende";
}

/**
 * @brief Slot, der aufgerufen wird, wenn eine WebSocket-Verbindung getrennt wird
 * 
 * Entfernt den Client aus der Liste und gibt die Ressourcen frei.
 */
void MainWindow::socketDisconnected()
{
    qDebug() << "socketDisconnected: Start";
    
    // Hole den Sender
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        // Entferne den Client aus der Liste
        m_clients.removeAll(client);
        
        // Zeige eine Meldung im TextEdit an
        m_messageDisplay->append("Verbindung getrennt: " + client->peerAddress().toString());
        
        // Gib die Ressourcen frei
        client->deleteLater();
    }
    
    qDebug() << "socketDisconnected: Ende";
}

void MainWindow::updateDiceRollTable(const QString &playerName, const QString &diceRoll, int result)
{
    // Erstelle eine neue Zeile
    QList<QStandardItem*> row;
    
    // Zeitstempel
    row.append(new QStandardItem(QTime::currentTime().toString("hh:mm:ss")));
    
    // Spielername
    row.append(new QStandardItem(playerName));
    
    // Würfelwurf-Beschreibung
    row.append(new QStandardItem(diceRoll));
    
    // Ergebnis
    row.append(new QStandardItem(QString::number(result)));
    
    // Füge die Zeile am Anfang der Tabelle ein
    m_diceRollModel->insertRow(0, row);
}

QString MainWindow::calculateDiceRollDescription(const QJsonArray &operands)
{
    QString description;
    
    for (int i = 0; i < operands.size(); ++i) {
        QJsonObject obj = operands[i].toObject();
        
        // Wenn es nicht das erste Element ist, füge ein "+" hinzu
        if (i > 0) {
            description += " + ";
        }
        
        if (obj.contains("kind")) {
            // Würfelwurf
            QString kind = obj["kind"].toString();
            QJsonArray results = obj["results"].toArray();
            description += QString("%1%2").arg(results.size()).arg(kind);
        } else if (obj.contains("value")) {
            // Fester Wert
            description += QString::number(obj["value"].toInt());
        } else if (obj.contains("operands")) {
            // Verschachtelte Operanden
            QJsonArray nestedOperands = obj["operands"].toArray();
            description += calculateDiceRollDescription(nestedOperands);
        }
    }
    
    return description;
}

int MainWindow::calculateDiceRollResult(const QJsonArray &operands)
{
    int result = 0;
    QString currentOperator = "+";
    
    for (const QJsonValue &operand : operands) {
        QJsonObject obj = operand.toObject();
        
        if (obj.contains("operator")) {
            currentOperator = obj["operator"].toString();
        }
        else if (obj.contains("kind")) {
            // Würfelwurf
            QJsonArray results = obj["results"].toArray();
            int sum = 0;
            for (const QJsonValue &value : results) {
                sum += value.toInt();
            }
            
            if (currentOperator == "+") {
                result += sum;
            } else if (currentOperator == "-") {
                result -= sum;
            }
        }
        else if (obj.contains("value")) {
            // Fester Wert
            int value = obj["value"].toInt();
            if (currentOperator == "+") {
                result += value;
            } else if (currentOperator == "-") {
                result -= value;
            }
        }
    }
    
    return result;
} 