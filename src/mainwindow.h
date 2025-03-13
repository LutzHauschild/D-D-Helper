#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "initiativetracker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_rollButton_clicked();
    void on_clearButton_clicked();
    void on_removeButton_clicked();
    
    void updateCharacterTable();

private:
    Ui::MainWindow *ui;
    InitiativeTracker m_initiativeTracker;
};
#endif // MAINWINDOW_H 