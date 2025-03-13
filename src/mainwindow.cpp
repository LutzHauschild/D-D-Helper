#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_initiativeTracker(this)
{
    ui->setupUi(this);
    
    // Verbindungen herstellen
    connect(&m_initiativeTracker, &InitiativeTracker::charactersChanged, this, &MainWindow::updateCharacterTable);
    connect(&m_initiativeTracker, &InitiativeTracker::initiativeRolled, this, &MainWindow::updateCharacterTable);
    
    // Tabelle konfigurieren
    ui->characterTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->characterTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->characterTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->characterTableWidget->verticalHeader()->setVisible(false);
    
    // Anfangszustand der Tabelle
    updateCharacterTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    QString name = ui->nameEdit->text().trimmed();
    int initiativeModifier = ui->initiativeModSpinBox->value();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, tr("Fehler"), tr("Bitte geben Sie einen Namen ein."));
        return;
    }
    
    Character character(name, initiativeModifier);
    m_initiativeTracker.addCharacter(character);
    
    // Eingabefelder zurücksetzen
    ui->nameEdit->clear();
    ui->initiativeModSpinBox->setValue(0);
    ui->nameEdit->setFocus();
}

void MainWindow::on_rollButton_clicked()
{
    if (m_initiativeTracker.getCharacters().isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Fügen Sie zuerst Charaktere hinzu."));
        return;
    }
    
    m_initiativeTracker.rollAllInitiatives();
}

void MainWindow::on_clearButton_clicked()
{
    if (m_initiativeTracker.getCharacters().isEmpty()) {
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Bestätigung"),
                                                             tr("Möchten Sie wirklich alle Charaktere entfernen?"),
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_initiativeTracker.clearCharacters();
    }
}

void MainWindow::on_removeButton_clicked()
{
    int currentRow = ui->characterTableWidget->currentRow();
    if (currentRow >= 0) {
        m_initiativeTracker.removeCharacter(currentRow);
    } else {
        QMessageBox::information(this, tr("Information"), tr("Bitte wählen Sie einen Charakter aus."));
    }
}

void MainWindow::updateCharacterTable()
{
    ui->characterTableWidget->setRowCount(0);
    
    QVector<Character> sortedCharacters = m_initiativeTracker.getSortedInitiativeOrder();
    
    for (const auto &character : sortedCharacters) {
        int row = ui->characterTableWidget->rowCount();
        ui->characterTableWidget->insertRow(row);
        
        ui->characterTableWidget->setItem(row, 0, new QTableWidgetItem(character.getName()));
        ui->characterTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(character.getInitiativeModifier())));
        ui->characterTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(character.getInitiativeRoll())));
        ui->characterTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(character.getTotalInitiative())));
    }
    
    ui->characterTableWidget->resizeColumnsToContents();
} 