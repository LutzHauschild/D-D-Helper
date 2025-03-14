# C++ Konzepte im D&D Initiative Tracker

Dieses Dokument erklärt die wichtigsten C++-Konzepte, die im D&D Initiative Tracker verwendet werden.

## Includes und Header-Dateien

In C++ werden Includes verwendet, um Code aus anderen Dateien einzubinden. Es gibt zwei Arten von Includes:

1. **System-Includes** mit spitzen Klammern: `#include <header>`
   - Beispiel: `#include <QString>`
   - Diese suchen in den Standard-Bibliotheksverzeichnissen
   - Werden für Qt-Bibliotheken und C++-Standardbibliotheken verwendet

2. **Projekt-Includes** mit Anführungszeichen: `#include "header"`
   - Beispiel: `#include "character.h"`
   - Diese suchen zuerst im aktuellen Verzeichnis
   - Werden für eigene Projektdateien verwendet

### Wichtige Includes im Projekt:

#### Qt-Includes:
- `<QApplication>`: Hauptklasse für Qt-Anwendungen, verwaltet die Ereignisschleife
- `<QVector>`: Dynamisches Array (ähnlich zu std::vector)
- `<QString>`: Unicode-Zeichenkette mit vielen nützlichen Methoden
- `<QObject>`: Basisklasse für alle Qt-Objekte, ermöglicht Signale und Slots
- `<QFile>`, `<QJsonDocument>`, `<QJsonArray>`, `<QJsonObject>`: Für JSON-Dateioperationen

#### C++-Standardbibliothek:
- `<random>`: Enthält Klassen für Zufallszahlengenerierung (random_device, mt19937, uniform_int_distribution)

## Klassen und Objektorientierung

### Klassendeklaration

```cpp
class Character {
public:
    // Öffentliche Methoden und Attribute
private:
    // Private Methoden und Attribute
};
```

- **public**: Zugänglich von überall
- **private**: Nur innerhalb der Klasse zugänglich
- **protected**: Nur innerhalb der Klasse und abgeleiteten Klassen zugänglich

### Konstruktoren

Konstruktoren initialisieren Objekte einer Klasse:

```cpp
Character::Character()
    : m_name(""), m_initiativeModifier(0), m_initiativeRoll(0)
{
    // Zusätzlicher Initialisierungscode (falls nötig)
}
```

Die Initialisierungsliste (nach dem Doppelpunkt) ist effizienter als Zuweisungen im Konstruktorrumpf.

### Getter und Setter

Methoden zum Zugriff auf private Attribute:

```cpp
// Getter
QString Character::getName() const {
    return m_name;
}

// Setter
void Character::setName(const QString &name) {
    m_name = name;
}
```

Das `const` nach dem Methodennamen bedeutet, dass die Methode das Objekt nicht verändert.

### Vererbung

```cpp
class InitiativeTracker : public QObject {
    Q_OBJECT  // Qt-Makro für Signale und Slots
    // ...
};
```

`InitiativeTracker` erbt von `QObject` und erhält alle seine Methoden und Eigenschaften.

## Qt-spezifische Konzepte

### Signale und Slots

Qt's Mechanismus für die Kommunikation zwischen Objekten:

```cpp
signals:
    void charactersChanged();  // Signal-Deklaration

public slots:
    void onCharacterAdded();   // Slot-Deklaration
```

- **Signale**: Werden von Objekten ausgesendet, wenn etwas passiert
- **Slots**: Methoden, die auf Signale reagieren können
- Verbindung: `connect(sender, SIGNAL(signalName()), receiver, SLOT(slotName()))`

### Qt-Ereignisschleife

```cpp
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // ...
    return a.exec();  // Startet die Ereignisschleife
}
```

Die Ereignisschleife:
1. Wartet auf Ereignisse (Mausklicks, Tastatureingaben, Timer, etc.)
2. Verteilt diese an die entsprechenden Objekte
3. Läuft, bis die Anwendung beendet wird

## Speicherverwaltung

### Stack vs. Heap

- **Stack**: Automatische Speicherverwaltung für lokale Variablen
  ```cpp
  Character c;  // Stack-Allokation, wird automatisch freigegeben
  ```

- **Heap**: Manuelle Speicherverwaltung mit `new` und `delete`
  ```cpp
  Character* c = new Character();  // Heap-Allokation
  delete c;  // Manuelles Freigeben des Speichers
  ```

### Qt-Speicherverwaltung

Qt verwendet ein Eltern-Kind-System für die Speicherverwaltung:

```cpp
MainWindow* window = new MainWindow();
QPushButton* button = new QPushButton(window);  // window ist der Elternteil
```

Wenn `window` zerstört wird, wird `button` automatisch mit zerstört.

## Datenstrukturen

### QVector

```cpp
QVector<Character> m_characters;  // Dynamisches Array von Character-Objekten
```

Wichtige Operationen:
- `append(element)`: Fügt ein Element hinzu
- `removeAt(index)`: Entfernt ein Element an einem bestimmten Index
- `clear()`: Entfernt alle Elemente
- `size()`: Gibt die Anzahl der Elemente zurück

### QString

```cpp
QString m_name;  // Unicode-Zeichenkette
```

Vorteile gegenüber `std::string`:
- Unicode-Unterstützung
- Viele nützliche Methoden (split, join, toInt, etc.)
- Automatische Konvertierung zwischen Zahlen und Zeichenketten

## Zufallszahlengenerierung

```cpp
static std::random_device s_rd;  // Hardware-Zufallszahlengenerator
static std::mt19937 s_gen(s_rd());  // Mersenne-Twister-Generator
static std::uniform_int_distribution<> s_d20(1, 20);  // Gleichverteilung 1-20

// Verwendung:
int roll = s_d20(s_gen);  // Generiert eine Zufallszahl zwischen 1 und 20
```

- `random_device`: Erzeugt "echte" Zufallszahlen (wenn verfügbar)
- `mt19937`: Mersenne-Twister-Generator, erzeugt hochqualitative Pseudozufallszahlen
- `uniform_int_distribution`: Erzeugt gleichverteilte Ganzzahlen im angegebenen Bereich

## JSON-Verarbeitung

```cpp
QJsonObject obj;
obj["name"] = character.getName();
obj["initiativeModifier"] = character.getInitiativeModifier();

QJsonDocument doc(obj);
QFile file(filename);
file.open(QIODevice::WriteOnly);
file.write(doc.toJson());
```

- `QJsonObject`: Repräsentiert ein JSON-Objekt (key-value Paare)
- `QJsonArray`: Repräsentiert ein JSON-Array
- `QJsonDocument`: Konvertiert zwischen JSON und Qt-Datenstrukturen
- `QFile`: Dateioperationen

## Const-Korrektheit

```cpp
QString getName() const;  // Diese Methode verändert das Objekt nicht
void setName(const QString &name);  // Der Parameter wird nicht verändert
```

- `const` nach einer Methode: Die Methode verändert das Objekt nicht
- `const` vor einem Parameter: Der Parameter wird nicht verändert
- `const` vor einem Rückgabetyp: Der Rückgabewert kann nicht verändert werden

## Referenzen vs. Zeiger

```cpp
void setName(const QString &name);  // Referenz
Character* getCharacter();  // Zeiger
```

- **Referenzen** (`&`): Aliase für existierende Objekte, können nicht null sein
- **Zeiger** (`*`): Speicheradressen, können null sein und neu zugewiesen werden

## Präprozessor-Direktiven

```cpp
#ifndef CHARACTER_H
#define CHARACTER_H
// Klassendeklaration
#endif // CHARACTER_H
```

Diese "Include Guards" verhindern mehrfaches Einbinden derselben Header-Datei.

## Namenskonventionen

- **m_variable**: Membervariablen (Attribute einer Klasse)
- **s_variable**: Statische Variablen (gehören zur Klasse, nicht zu Instanzen)
- **camelCase**: Für Methoden und Variablen
- **PascalCase**: Für Klassen und Typen

## Dokumentation mit Doxygen

```cpp
/**
 * @brief Kurze Beschreibung
 * 
 * Detaillierte Beschreibung
 * 
 * @param name Parameterbeschreibung
 * @return Beschreibung des Rückgabewerts
 */
```

Diese Kommentare können von Doxygen verwendet werden, um automatisch Dokumentation zu generieren. 