# Qt-Konzepte im D&D Initiative Tracker

Dieses Dokument erklärt die wichtigsten Qt-spezifischen Konzepte, die im D&D Initiative Tracker verwendet werden.

## Qt Framework Grundlagen

Qt ist ein umfangreiches C++-Framework für die Entwicklung plattformübergreifender Anwendungen. Es bietet:

- **Plattformunabhängigkeit**: Derselbe Code läuft auf Windows, macOS, Linux, Android, iOS
- **Umfangreiche Bibliothek**: GUI-Widgets, Netzwerk, Datenbanken, Multimedia, etc.
- **Eigene Erweiterungen zu C++**: Signale und Slots, Meta-Object-System, Property-System

## Die Qt-Ereignisschleife

Die Ereignisschleife (Event Loop) ist das Herzstück jeder Qt-Anwendung:

```cpp
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();  // Startet die Ereignisschleife
}
```

### Wie die Ereignisschleife funktioniert:

1. **Ereignisquellen**: Benutzerinteraktionen (Maus, Tastatur), Timer, Netzwerkereignisse, etc.
2. **Ereigniswarteschlange**: Ereignisse werden in einer Warteschlange gespeichert
3. **Ereignisverteilung**: Die Ereignisschleife entnimmt Ereignisse aus der Warteschlange und leitet sie an die entsprechenden Objekte weiter
4. **Ereignisbehandlung**: Objekte reagieren auf Ereignisse durch Ausführen von Code

### Beispiel für Ereignisfluss:

1. Benutzer klickt auf einen Button
2. Qt erzeugt ein Mausklick-Ereignis und fügt es der Warteschlange hinzu
3. Die Ereignisschleife entnimmt das Ereignis und sendet es an den Button
4. Der Button verarbeitet das Ereignis und sendet ein Signal (clicked())
5. Verbundene Slots werden aufgerufen

## Signale und Slots

Signale und Slots sind Qt's Mechanismus für die Kommunikation zwischen Objekten:

```cpp
// In der Header-Datei:
class InitiativeTracker : public QObject {
    Q_OBJECT  // Makro für Meta-Object-Compiler
    
signals:
    void charactersChanged();  // Signal-Deklaration
    
public slots:
    void onButtonClicked();    // Slot-Deklaration
};

// In der Implementierungsdatei:
connect(button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
```

### Vorteile von Signalen und Slots:

- **Lose Kopplung**: Signalsender muss nicht wissen, wer auf das Signal reagiert
- **Typsicherheit**: Der Compiler prüft, ob Signal- und Slot-Signaturen kompatibel sind
- **Mehrfachverbindungen**: Ein Signal kann mit mehreren Slots verbunden werden
- **Automatische Trennung**: Verbindungen werden automatisch getrennt, wenn Objekte zerstört werden

### Moderne Syntax (Qt 5+):

```cpp
connect(button, &QPushButton::clicked, this, &MyClass::onButtonClicked);
```

## Qt-Widgets und GUI-Entwicklung

Qt bietet eine umfangreiche Bibliothek von GUI-Elementen (Widgets):

```cpp
QPushButton* button = new QPushButton("Klick mich", parent);
button->setGeometry(10, 10, 100, 30);  // x, y, Breite, Höhe
```

### Wichtige Widgets im Projekt:

- **QMainWindow**: Hauptfenster mit Menüleiste, Symbolleisten, Statusleiste
- **QTableView**: Tabellenansicht für Daten
- **QPushButton**: Schaltfläche
- **QLineEdit**: Einzeiliges Textfeld
- **QSpinBox**: Numerisches Eingabefeld mit Pfeilen zum Erhöhen/Verringern

### Layout-Management:

```cpp
QVBoxLayout* layout = new QVBoxLayout();
layout->addWidget(button1);
layout->addWidget(button2);
widget->setLayout(layout);
```

- **QVBoxLayout**: Vertikale Anordnung
- **QHBoxLayout**: Horizontale Anordnung
- **QGridLayout**: Tabellarische Anordnung
- **QFormLayout**: Beschriftung-Feld-Paare

## Model-View-Architektur

Qt implementiert das Model-View-Muster für die Darstellung von Daten:

```cpp
QTableView* view = new QTableView();
QStandardItemModel* model = new QStandardItemModel();
view->setModel(model);
```

### Komponenten:

- **Model**: Enthält die Daten und ihre Struktur
- **View**: Zeigt die Daten an
- **Delegate**: Steuert, wie Daten angezeigt und bearbeitet werden

### Vorteile:

- **Trennung von Daten und Darstellung**
- **Mehrere Views für dieselben Daten**
- **Automatische Aktualisierung der Views bei Datenänderungen**

## Qt-Ressourcensystem

Qt bietet ein Ressourcensystem zum Einbetten von Dateien in die Anwendung:

```cpp
// In der .qrc-Datei:
<RCC>
    <qresource prefix="/">
        <file>images/icon.png</file>
        <file>i18n/translation_de.qm</file>
    </qresource>
</RCC>

// Im Code:
QIcon icon(":/images/icon.png");
```

### Vorteile:

- **Plattformunabhängigkeit**: Keine Probleme mit Dateipfaden
- **Einfache Verteilung**: Alle Ressourcen sind in der ausführbaren Datei enthalten
- **Schneller Zugriff**: Ressourcen werden beim Start geladen

## Internationalisierung (i18n)

Qt bietet umfassende Unterstützung für mehrsprachige Anwendungen:

```cpp
// Im Code:
tr("Hello World");  // Markiert Text für Übersetzung

// In der main.cpp:
QTranslator translator;
translator.load(":/i18n/myapp_de.qm");
app.installTranslator(&translator);
```

### Prozess:

1. Markieren von Texten mit `tr()`
2. Extrahieren der Texte mit `lupdate`
3. Übersetzen der Texte mit Qt Linguist
4. Kompilieren der Übersetzungen mit `lrelease`
5. Laden der Übersetzungen zur Laufzeit

## Qt-Designer und UI-Dateien

Qt Designer ist ein WYSIWYG-Tool zum Erstellen von Benutzeroberflächen:

```cpp
// In der .ui-Datei (XML):
<ui version="4.0">
 <widget class="QMainWindow" name="MainWindow">
  <!-- ... -->
 </widget>
</ui>

// Im Code:
#include "ui_mainwindow.h"
Ui::MainWindow ui;
ui.setupUi(this);
```

### Vorteile:

- **Visuelle Entwicklung**: Drag-and-Drop-Interface
- **Schnelle Prototypen**: Schnelles Erstellen von Benutzeroberflächen
- **Trennung von Design und Code**: UI-Dateien können unabhängig vom Code bearbeitet werden

## Qt-Metaobjektsystem

Das Metaobjektsystem ist die Grundlage für viele Qt-Funktionen:

```cpp
class MyClass : public QObject {
    Q_OBJECT  // Aktiviert das Metaobjektsystem
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    
public:
    QString name() const { return m_name; }
    void setName(const QString &name) { 
        if (m_name != name) {
            m_name = name;
            emit nameChanged(name);
        }
    }
    
signals:
    void nameChanged(const QString &name);
    
private:
    QString m_name;
};
```

### Funktionen:

- **Laufzeittyp-Informationen**: `qobject_cast<>`, `inherits()`
- **Eigenschaften (Properties)**: Deklarative Definition von Objekteigenschaften
- **Signale und Slots**: Ermöglicht die Kommunikation zwischen Objekten
- **Dynamische Übersetzung**: Unterstützung für `tr()`

## JSON in Qt

Qt bietet einfache Klassen für die Arbeit mit JSON:

```cpp
// JSON erstellen:
QJsonObject obj;
obj["name"] = "John";
obj["age"] = 30;

QJsonArray arr;
arr.append("value1");
arr.append("value2");
obj["array"] = arr;

QJsonDocument doc(obj);
QString json = doc.toJson();

// JSON parsen:
QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
QJsonObject obj = doc.object();
QString name = obj["name"].toString();
int age = obj["age"].toInt();
```

### Klassen:

- **QJsonDocument**: Repräsentiert ein JSON-Dokument
- **QJsonObject**: Repräsentiert ein JSON-Objekt (key-value Paare)
- **QJsonArray**: Repräsentiert ein JSON-Array
- **QJsonValue**: Repräsentiert einen JSON-Wert (String, Zahl, Boolean, etc.)

## Qt-Dateisystem

Qt bietet plattformunabhängige Klassen für Dateioperationen:

```cpp
// Datei lesen:
QFile file("data.txt");
if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
}

// Datei schreiben:
QFile file("data.txt");
if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << "Hello World";
    file.close();
}
```

### Klassen:

- **QFile**: Zugriff auf lokale Dateien
- **QDir**: Verzeichnisoperationen
- **QFileInfo**: Informationen über Dateien
- **QTextStream**: Textbasierte Ein-/Ausgabe
- **QDataStream**: Binäre Ein-/Ausgabe 