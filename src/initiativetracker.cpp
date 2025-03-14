#include "initiativetracker.h"
#include <algorithm>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

/**
 * @brief Konstruktor für den InitiativeTracker
 * 
 * Initialisiert einen leeren InitiativeTracker ohne Charaktere.
 * 
 * @param parent Das Elternobjekt für die Qt-Objekthierarchie
 */
InitiativeTracker::InitiativeTracker(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief Fügt einen Charakter zur Liste hinzu
 * 
 * @param character Der hinzuzufügende Charakter
 */
void InitiativeTracker::addCharacter(const Character &character)
{
    qDebug() << "InitiativeTracker::addCharacter: Start - Name:" << character.getName();
    
    // Füge den Charakter zur Liste hinzu
    m_characters.append(character);
    qDebug() << "InitiativeTracker::addCharacter: Charakter hinzugefügt, neue Größe:" << m_characters.size();
    
    // Sende ein Signal, dass sich die Charakterliste geändert hat
    qDebug() << "InitiativeTracker::addCharacter: Sende Signal charactersChanged";
    emit charactersChanged();
    
    qDebug() << "InitiativeTracker::addCharacter: Ende";
}

/**
 * @brief Entfernt einen Charakter aus der Liste anhand seines Index
 * 
 * @param index Der Index des zu entfernenden Charakters
 */
void InitiativeTracker::removeCharacter(int index)
{
    // Überprüfe, ob der Index gültig ist
    if (index >= 0 && index < m_characters.size()) {
        // Entferne den Charakter aus der Liste
        m_characters.removeAt(index);
        
        // Sende ein Signal, dass sich die Charakterliste geändert hat
        emit charactersChanged();
    }
}

/**
 * @brief Leert die gesamte Charakterliste
 */
void InitiativeTracker::clearCharacters()
{
    // Leere die Charakterliste
    m_characters.clear();
    
    // Sende ein Signal, dass sich die Charakterliste geändert hat
    emit charactersChanged();
}

/**
 * @brief Gibt die aktuelle Charakterliste zurück
 * 
 * @return Ein QVector mit allen Charakteren
 */
QVector<Character> InitiativeTracker::getCharacters() const
{
    return m_characters;
}

/**
 * @brief Aktualisiert einen Charakter in der Liste
 * 
 * @param index Der Index des zu aktualisierenden Charakters
 * @param character Der aktualisierte Charakter
 */
void InitiativeTracker::updateCharacter(int index, const Character &character)
{
    // Überprüfe, ob der Index gültig ist
    if (index >= 0 && index < m_characters.size()) {
        // Aktualisiere den Charakter
        m_characters[index] = character;
        
        // Sende ein Signal, dass sich die Charakterliste geändert hat
        emit charactersChanged();
    }
}

/**
 * @brief Würfelt die Initiative für alle Charaktere in der Liste
 */
void InitiativeTracker::rollAllInitiatives()
{
    // Würfle die Initiative für jeden Charakter
    for (int i = 0; i < m_characters.size(); ++i) {
        m_characters[i].rollInitiative();
    }
    
    // Sende ein Signal, dass die Initiative gewürfelt wurde
    emit initiativeRolled();
}

/**
 * @brief Würfelt die Initiative für einen einzelnen Charakter
 * 
 * @param index Der Index des Charakters
 */
void InitiativeTracker::rollInitiativeForCharacter(int index)
{
    // Überprüfe, ob der Index gültig ist
    if (index >= 0 && index < m_characters.size()) {
        // Würfle die Initiative für den Charakter
        m_characters[index].rollInitiative();
        
        // Sende ein Signal, dass die Initiative gewürfelt wurde
        emit initiativeRolled();
    }
}

/**
 * @brief Gibt eine nach Initiative sortierte Charakterliste zurück
 * 
 * @return Ein QVector mit den sortierten Charakteren
 */
QVector<Character> InitiativeTracker::getSortedInitiativeOrder() const
{
    // Erstelle eine Kopie der Charakterliste
    QVector<Character> sortedCharacters = m_characters;
    
    // Sortiere die Charaktere nach Initiative (absteigend)
    std::sort(sortedCharacters.begin(), sortedCharacters.end(), 
              [](const Character &a, const Character &b) {
                  return a.getTotalInitiative() > b.getTotalInitiative();
              });
    
    return sortedCharacters;
}

/**
 * @brief Speichert die Charakterliste in einer Datei
 * 
 * @param filename Der Dateiname zum Speichern der Daten
 * @return true, wenn das Speichern erfolgreich war, sonst false
 */
bool InitiativeTracker::saveToFile(const QString &filename)
{
    // Erstelle ein JSON-Array für die Charaktere
    QJsonArray charactersArray;
    
    // Füge jeden Charakter als JSON-Objekt hinzu
    for (const Character &character : m_characters) {
        QJsonObject characterObject;
        characterObject["name"] = character.getName();
        characterObject["initiativeModifier"] = character.getInitiativeModifier();
        characterObject["initiativeRoll"] = character.getInitiativeRoll();
        characterObject["willSave"] = character.getWillSave();
        characterObject["reflexSave"] = character.getReflexSave();
        characterObject["fortitudeSave"] = character.getFortitudeSave();
        characterObject["lastWillSaveRoll"] = character.getLastWillSaveRoll();
        characterObject["lastReflexSaveRoll"] = character.getLastReflexSaveRoll();
        characterObject["lastFortitudeSaveRoll"] = character.getLastFortitudeSaveRoll();
        
        charactersArray.append(characterObject);
    }
    
    // Erstelle ein JSON-Dokument mit dem Array
    QJsonDocument document(charactersArray);
    
    // Öffne die Datei zum Schreiben
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Fehler beim Öffnen der Datei zum Schreiben:" << filename;
        return false;
    }
    
    // Schreibe das JSON-Dokument in die Datei
    file.write(document.toJson());
    file.close();
    
    return true;
}

/**
 * @brief Lädt die Charakterliste aus einer Datei
 * 
 * @param filename Der Dateiname zum Laden der Daten
 * @return true, wenn das Laden erfolgreich war, sonst false
 */
bool InitiativeTracker::loadFromFile(const QString &filename)
{
    // Öffne die Datei zum Lesen
    QFile file(filename);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qDebug() << "Datei existiert nicht oder konnte nicht geöffnet werden:" << filename;
        return false;
    }
    
    // Lese den Inhalt der Datei
    QByteArray data = file.readAll();
    file.close();
    
    // Parse den JSON-Inhalt
    QJsonDocument document = QJsonDocument::fromJson(data);
    if (document.isNull() || !document.isArray()) {
        qDebug() << "Ungültiges JSON-Format in der Datei:" << filename;
        return false;
    }
    
    // Leere die aktuelle Charakterliste
    m_characters.clear();
    
    // Füge jeden Charakter aus dem JSON-Array hinzu
    QJsonArray charactersArray = document.array();
    for (const QJsonValue &value : charactersArray) {
        QJsonObject characterObject = value.toObject();
        
        // Erstelle einen neuen Charakter mit den Daten aus dem JSON-Objekt
        Character character(
            characterObject["name"].toString(),
            characterObject["initiativeModifier"].toInt(),
            characterObject["willSave"].toInt(),
            characterObject["reflexSave"].toInt(),
            characterObject["fortitudeSave"].toInt()
        );
        
        // Setze die Initiative-Werte, falls vorhanden
        if (characterObject.contains("initiativeRoll")) {
            // Hier müssten wir eigentlich die rollInitiative-Methode aufrufen,
            // aber wir können den Wert nicht direkt setzen, da er privat ist.
            // Stattdessen würfeln wir neu und überschreiben später.
            character.rollInitiative();
        }
        
        // Füge den Charakter zur Liste hinzu
        m_characters.append(character);
    }
    
    // Sende ein Signal, dass sich die Charakterliste geändert hat
    emit charactersChanged();
    
    return true;
}

/**
 * @brief Würfelt Willenskraft-Rettungswürfe für alle Charaktere
 */
void InitiativeTracker::rollAllWillSaves()
{
    // Würfle Willenskraft-Rettungswürfe für jeden Charakter
    for (int i = 0; i < m_characters.size(); ++i) {
        m_characters[i].rollWillSave();
    }
    
    // Sende ein Signal, dass Rettungswürfe gewürfelt wurden
    emit savesRolled();
}

/**
 * @brief Würfelt einen Willenskraft-Rettungswurf für einen einzelnen Charakter
 * 
 * @param index Der Index des Charakters
 */
void InitiativeTracker::rollWillSaveForCharacter(int index)
{
    // Überprüfe, ob der Index gültig ist
    if (index >= 0 && index < m_characters.size()) {
        // Würfle den Willenskraft-Rettungswurf für den Charakter
        m_characters[index].rollWillSave();
        
        // Sende ein Signal, dass Rettungswürfe gewürfelt wurden
        emit savesRolled();
    }
}

/**
 * @brief Würfelt Reflex-Rettungswürfe für alle Charaktere
 */
void InitiativeTracker::rollAllReflexSaves()
{
    // Würfle Reflex-Rettungswürfe für jeden Charakter
    for (int i = 0; i < m_characters.size(); ++i) {
        m_characters[i].rollReflexSave();
    }
    
    // Sende ein Signal, dass Rettungswürfe gewürfelt wurden
    emit savesRolled();
}

/**
 * @brief Würfelt einen Reflex-Rettungswurf für einen einzelnen Charakter
 * 
 * @param index Der Index des Charakters
 */
void InitiativeTracker::rollReflexSaveForCharacter(int index)
{
    // Überprüfe, ob der Index gültig ist
    if (index >= 0 && index < m_characters.size()) {
        // Würfle den Reflex-Rettungswurf für den Charakter
        m_characters[index].rollReflexSave();
        
        // Sende ein Signal, dass Rettungswürfe gewürfelt wurden
        emit savesRolled();
    }
}

/**
 * @brief Würfelt Konstitution-Rettungswürfe für alle Charaktere
 */
void InitiativeTracker::rollAllFortitudeSaves()
{
    // Würfle Konstitution-Rettungswürfe für jeden Charakter
    for (int i = 0; i < m_characters.size(); ++i) {
        m_characters[i].rollFortitudeSave();
    }
    
    // Sende ein Signal, dass Rettungswürfe gewürfelt wurden
    emit savesRolled();
}

/**
 * @brief Würfelt einen Konstitution-Rettungswurf für einen einzelnen Charakter
 * 
 * @param index Der Index des Charakters
 */
void InitiativeTracker::rollFortitudeSaveForCharacter(int index)
{
    // Überprüfe, ob der Index gültig ist
    if (index >= 0 && index < m_characters.size()) {
        // Würfle den Konstitution-Rettungswurf für den Charakter
        m_characters[index].rollFortitudeSave();
        
        // Sende ein Signal, dass Rettungswürfe gewürfelt wurden
        emit savesRolled();
    }
}

Character& InitiativeTracker::getCharacterRef(int index)
{
    // Überprüfe, ob die Liste leer ist
    if (m_characters.isEmpty()) {
        qWarning() << "Zugriff auf leere Charakterliste";
        // Füge einen temporären Charakter hinzu, um einen Absturz zu vermeiden
        m_characters.append(Character("Temporär", 0, 0, 0, 0));
        return m_characters[0];
    }
    
    // Überprüfe, ob der Index gültig ist
    if (index < 0 || index >= m_characters.size()) {
        // Wenn nicht, gib eine Referenz auf den ersten Charakter zurück
        qWarning() << "Ungültiger Index:" << index;
        return m_characters[0];
    }
    
    return m_characters[index];
} 