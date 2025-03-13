/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *nameEdit;
    QLabel *label_2;
    QSpinBox *initiativeModSpinBox;
    QPushButton *addButton;
    QTableWidget *characterTableWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *rollButton;
    QPushButton *clearButton;
    QPushButton *removeButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        nameEdit = new QLineEdit(groupBox);
        nameEdit->setObjectName("nameEdit");

        horizontalLayout->addWidget(nameEdit);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        initiativeModSpinBox = new QSpinBox(groupBox);
        initiativeModSpinBox->setObjectName("initiativeModSpinBox");
        initiativeModSpinBox->setMinimum(-10);
        initiativeModSpinBox->setMaximum(20);

        horizontalLayout->addWidget(initiativeModSpinBox);

        addButton = new QPushButton(groupBox);
        addButton->setObjectName("addButton");

        horizontalLayout->addWidget(addButton);


        verticalLayout->addWidget(groupBox);

        characterTableWidget = new QTableWidget(centralwidget);
        if (characterTableWidget->columnCount() < 5)
            characterTableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        characterTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        characterTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        characterTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        characterTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        characterTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        characterTableWidget->setObjectName("characterTableWidget");

        verticalLayout->addWidget(characterTableWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        rollButton = new QPushButton(centralwidget);
        rollButton->setObjectName("rollButton");

        horizontalLayout_2->addWidget(rollButton);

        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName("clearButton");

        horizontalLayout_2->addWidget(clearButton);

        removeButton = new QPushButton(centralwidget);
        removeButton->setObjectName("removeButton");

        horizontalLayout_2->addWidget(removeButton);


        verticalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "D&D Initiative Tracker", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Charakter hinzuf\303\274gen", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Name:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Initiative-Modifikator:", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "Hinzuf\303\274gen", nullptr));
        QTableWidgetItem *___qtablewidgetitem = characterTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = characterTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Initiative-Mod", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = characterTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Wurf", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = characterTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Gesamt", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = characterTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "TaleSpire", nullptr));
        rollButton->setText(QCoreApplication::translate("MainWindow", "Initiative w\303\274rfeln", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "Liste leeren", nullptr));
        removeButton->setText(QCoreApplication::translate("MainWindow", "Ausgew\303\244hlten entfernen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
