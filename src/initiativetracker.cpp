#include "initiativetracker.h"
#include <algorithm>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

/**
 * @brief Konstruktor für den InitiativeTracker
 * 
 * Initialisiert einen leeren InitiativeTracker mit dem angegebenen Elternobjekt.
 * 
 * @param parent Das Elternobjekt für die Qt-Objekthierarchie
 */
InitiativeTracker::InitiativeTracker(QObject *parent)
    : QObject(parent)
{
    // Initialisiert einen leeren InitiativeTracker
}

/**
 * @brief Fügt einen Charakter zur Liste hinzu
 * 
 * Der Charakter wird am Ende der Liste hinzugefügt und das Signal
 * charactersChanged() wird gesendet, um die UI zu aktualisieren.
 * 
 * @param character Der hinzuzufügende Charakter
 */
void InitiativeTracker::addCharacter(const Character &character)
{
    // Fügt den Charakter zur Liste hinzu
    m_characters.append(character);
    
    // Sendet ein Signal, dass sich die Charakterliste geändert hat
    emit charactersChanged();
}

/**
 * @brief Entfernt einen Charakter aus der Liste anhand seines Index
 * 
 * Wenn der Index gültig ist, wird der Charakter entfernt und das Signal
 * charactersChanged() wird gesendet, um die UI zu aktualisieren.
 * 
 * @param index Der Index des zu entfernenden Charakters
 */
void InitiativeTracker::removeCharacter(int index)
{
    // Überprüft, ob der Index gültig ist
    if (index >= 0 && index < m_characters.size()) {
        // Entfernt den Charakter an der angegebenen Position
        m_characters.removeAt(index);
        
        // Sendet ein Signal, dass sich die Charakterliste geändert hat
        emit charactersChanged();
    }
}

/**
 * @brief Leert die gesamte Charakterliste
 * 
 * Entfernt alle Charaktere aus der Liste und sendet das Signal
 * charactersChanged(), um die UI zu aktualisieren.
 */
void InitiativeTracker::clearCharacters()
{
    // Entfernt alle Charaktere aus der Liste
    m_characters.clear();
    
    // Sendet ein Signal, dass sich die Charakterliste geändert hat
    emit charactersChanged();
}

/**
 * @brief Gibt die aktuelle Charakterliste zurück
 * 
 * @return Ein QVector mit allen Charakteren in der Reihenfolge, in der sie hinzugefügt wurden
 */
QVector<Character> InitiativeTracker::getCharacters() const
{
    return m_characters;
}

/**
 * @brief Würfelt die Initiative für alle Charaktere in der Liste
 * 
 * Ruft für jeden Charakter in der Liste die rollInitiative()-Methode auf,
 * um einen neuen W20-Wurf zu generieren. Sendet das Signal initiativeRolled(),
 * um die UI zu aktualisieren.
 */
void InitiativeTracker::rollAllInitiatives()
{
    // Würfelt die Initiative für jeden Charakter in der Liste
    for (auto &character : m_characters) {
        character.rollInitiative();
    }
    
    // Sendet ein Signal, dass die Initiative gewürfelt wurde
    emit initiativeRolled();
}

/**
 * @brief Gibt eine nach Initiative sortierte Charakterliste zurück
 * 
 * Erstellt eine Kopie der Charakterliste und sortiert sie nach dem
 * Gesamt-Initiative-Wert (Wurf + Modifikator) in absteigender Reihenfolge,
 * sodass der Charakter mit dem höchsten Wert an erster Stelle steht.
 * 
 * @return Ein QVector mit den sortierten Charakteren
 */
QVector<Character> InitiativeTracker::getSortedInitiativeOrder() const
{
    // Erstellt eine Kopie der Charakterliste
    QVector<Character> sortedCharacters = m_characters;
    
    // Sortiert die Charaktere nach ihrem Gesamt-Initiative-Wert (absteigend)
    std::sort(sortedCharacters.begin(), sortedCharacters.end(), 
              [](const Character &a, const Character &b) {
                  // Vergleicht die Gesamt-Initiative-Werte (höherer Wert zuerst)
                  return a.getTotalInitiative() > b.getTotalInitiative();
              });
    
    return sortedCharacters;
}

/**
 * @brief Speichert die Charakterliste in einer Datei
 * 
 * Die Charakterdaten werden im JSON-Format in der angegebenen Datei gespeichert.
 * Die Datei wird im Anwendungsdatenverzeichnis des Benutzers gespeichert.
 * 
 * @param filename Der Dateiname zum Speichern der Daten
 * @return true, wenn das Speichern erfolgreich war, sonst false
 */
bool InitiativeTracker::saveToFile(const QString &filename)
{
    // Erstellt das Verzeichnis für die Anwendungsdaten, falls es nicht existiert
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // Erstellt den vollständigen Dateipfad
    QString filePath = dataPath + QDir::separator() + filename;
    
    // Öffnet die Datei zum Schreiben
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Fehler beim Öffnen der Datei zum Speichern:" << file.errorString();
        return false;
    }
    
    // Erstellt ein JSON-Array für die Charaktere
    QJsonArray charactersArray;
    
    // Fügt jeden Charakter als JSON-Objekt zum Array hinzu
    for (const auto &character : m_characters) {
        QJsonObject characterObject;
        characterObject["name"] = character.getName();
        characterObject["initiativeModifier"] = character.getInitiativeModifier();
        
        charactersArray.append(characterObject);
    }
    
    // Erstellt ein JSON-Dokument mit dem Array
    QJsonDocument document(charactersArray);
    
    // Schreibt das JSON-Dokument in die Datei
    file.write(document.toJson());
    
    return true;
}

/**
 * @brief Lädt die Charakterliste aus einer Datei
 * 
 * Die Charakterdaten werden aus der angegebenen JSON-Datei geladen.
 * Die Datei wird im Anwendungsdatenverzeichnis des Benutzers gesucht.
 * 
 * @param filename Der Dateiname zum Laden der Daten
 * @return true, wenn das Laden erfolgreich war, sonst false
 */
bool InitiativeTracker::loadFromFile(const QString &filename)
{
    // Erstellt den vollständigen Dateipfad
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filePath = dataPath + QDir::separator() + filename;
    
    // Überprüft, ob die Datei existiert
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << "Datei existiert nicht:" << filePath;
        return false;
    }
    
    // Öffnet die Datei zum Lesen
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Fehler beim Öffnen der Datei zum Laden:" << file.errorString();
        return false;
    }
    
    // Liest den Dateiinhalt
    QByteArray data = file.readAll();
    
    // Parst den JSON-Inhalt
    QJsonDocument document = QJsonDocument::fromJson(data);
    
    // Überprüft, ob das Dokument ein Array enthält
    if (!document.isArray()) {
        qDebug() << "Ungültiges JSON-Format: Kein Array";
        return false;
    }
    
    // Löscht die aktuelle Charakterliste
    m_characters.clear();
    
    // Liest die Charaktere aus dem JSON-Array
    QJsonArray charactersArray = document.array();
    for (const auto &value : charactersArray) {
        if (!value.isObject()) {
            continue;
        }
        
        QJsonObject characterObject = value.toObject();
        
        // Liest die Charakterdaten aus dem JSON-Objekt
        QString name = characterObject["name"].toString();
        int initiativeModifier = characterObject["initiativeModifier"].toInt();
        
        // Erstellt einen neuen Charakter und fügt ihn zur Liste hinzu
        Character character(name, initiativeModifier);
        m_characters.append(character);
    }
    
    // Sendet ein Signal, dass sich die Charakterliste geändert hat
    emit charactersChanged();
    
    return true;
} 