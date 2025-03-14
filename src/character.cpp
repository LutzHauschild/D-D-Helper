#include "character.h"

// Initialisierung der statischen Klassenvariablen
std::random_device Character::s_rd;  // Erzeugt einen zufälligen Seed
std::mt19937 Character::s_gen(Character::s_rd());  // Initialisiert den Mersenne-Twister mit dem Seed
std::uniform_int_distribution<> Character::s_d20(1, 20);  // Erzeugt eine Gleichverteilung für Zahlen von 1 bis 20 (W20)

/**
 * @brief Standardkonstruktor
 * 
 * Erstellt einen Charakter mit leerem Namen und Initiative-Modifikator 0.
 * Der Initiative-Wurf wird ebenfalls auf 0 gesetzt, bis rollInitiative() aufgerufen wird.
 */
Character::Character()
    : m_name(""), m_initiativeModifier(0), m_initiativeRoll(0),
      m_willSave(0), m_reflexSave(0), m_fortitudeSave(0),
      m_lastWillSaveRoll(0), m_lastReflexSaveRoll(0), m_lastFortitudeSaveRoll(0)
{
    // Initialisiert einen leeren Charakter
}

/**
 * @brief Konstruktor mit Parametern
 * 
 * Erstellt einen Charakter mit dem angegebenen Namen und Initiative-Modifikator.
 * Der Initiative-Wurf wird auf 0 gesetzt, bis rollInitiative() aufgerufen wird.
 * 
 * @param name Der Name des Charakters
 * @param initiativeModifier Der Initiative-Modifikator des Charakters
 */
Character::Character(const QString &name, int initiativeModifier)
    : m_name(name), m_initiativeModifier(initiativeModifier), m_initiativeRoll(0),
      m_willSave(0), m_reflexSave(0), m_fortitudeSave(0),
      m_lastWillSaveRoll(0), m_lastReflexSaveRoll(0), m_lastFortitudeSaveRoll(0)
{
    // Initialisiert einen Charakter mit den angegebenen Werten
}

/**
 * @brief Konstruktor mit allen Parametern
 * 
 * Erstellt einen Charakter mit dem angegebenen Namen, Initiative-Modifikator und Rettungswürfen.
 * Der Initiative-Wurf wird auf 0 gesetzt, bis rollInitiative() aufgerufen wird.
 * 
 * @param name Der Name des Charakters
 * @param initiativeModifier Der Initiative-Modifikator des Charakters
 * @param willSave Der Wert des Will-Rettungswurfs
 * @param reflexSave Der Wert des Reflex-Rettungswurfs
 * @param fortitudeSave Der Wert des Fortitude-Rettungswurfs
 */
Character::Character(const QString &name, int initiativeModifier, int willSave, int reflexSave, int fortitudeSave)
    : m_name(name), m_initiativeModifier(initiativeModifier), m_initiativeRoll(0),
      m_willSave(willSave), m_reflexSave(reflexSave), m_fortitudeSave(fortitudeSave),
      m_lastWillSaveRoll(0), m_lastReflexSaveRoll(0), m_lastFortitudeSaveRoll(0)
{
    // Initialisiert einen Charakter mit den angegebenen Werten
}

/**
 * @brief Gibt den Namen des Charakters zurück
 * 
 * @return Der Name des Charakters als QString
 */
QString Character::getName() const
{
    return m_name;
}

/**
 * @brief Setzt den Namen des Charakters
 * 
 * @param name Der neue Name des Charakters
 */
void Character::setName(const QString &name)
{
    m_name = name;
}

/**
 * @brief Gibt den Initiative-Modifikator des Charakters zurück
 * 
 * @return Der Initiative-Modifikator als Integer
 */
int Character::getInitiativeModifier() const
{
    return m_initiativeModifier;
}

/**
 * @brief Setzt den Initiative-Modifikator des Charakters
 * 
 * @param modifier Der neue Initiative-Modifikator
 */
void Character::setInitiativeModifier(int modifier)
{
    m_initiativeModifier = modifier;
}

/**
 * @brief Gibt den gewürfelten Initiative-Wert zurück
 * 
 * @return Der gewürfelte Initiative-Wert (1-20) als Integer
 */
int Character::getInitiativeRoll() const
{
    return m_initiativeRoll;
}

/**
 * @brief Würfelt die Initiative für den Charakter
 * 
 * Simuliert einen W20-Würfelwurf (1-20) und speichert das Ergebnis.
 * Verwendet den Mersenne-Twister-Algorithmus für eine gute Zufallsverteilung.
 */
void Character::rollInitiative()
{
    // Generiert eine Zufallszahl zwischen 1 und 20 (W20)
    m_initiativeRoll = s_d20(s_gen);
}

/**
 * @brief Berechnet den Gesamt-Initiative-Wert
 * 
 * @return Die Summe aus gewürfeltem Wert und Initiative-Modifikator
 */
int Character::getTotalInitiative() const
{
    // Addiert den Würfelwurf und den Modifikator
    return m_initiativeRoll + m_initiativeModifier;
}

// Implementierung der neuen Methoden für Rettungswürfe

/**
 * @brief Gibt den Wert des Will-Rettungswurfs zurück
 * 
 * @return Der Wert des Will-Rettungswurfs als Integer
 */
int Character::getWillSave() const
{
    return m_willSave;
}

/**
 * @brief Setzt den Wert des Will-Rettungswurfs
 * 
 * @param modifier Der neue Wert des Will-Rettungswurfs
 */
void Character::setWillSave(int modifier)
{
    m_willSave = modifier;
}

/**
 * @brief Gibt den Wert des Reflex-Rettungswurfs zurück
 * 
 * @return Der Wert des Reflex-Rettungswurfs als Integer
 */
int Character::getReflexSave() const
{
    return m_reflexSave;
}

/**
 * @brief Setzt den Wert des Reflex-Rettungswurfs
 * 
 * @param modifier Der neue Wert des Reflex-Rettungswurfs
 */
void Character::setReflexSave(int modifier)
{
    m_reflexSave = modifier;
}

/**
 * @brief Gibt den Wert des Fortitude-Rettungswurfs zurück
 * 
 * @return Der Wert des Fortitude-Rettungswurfs als Integer
 */
int Character::getFortitudeSave() const
{
    return m_fortitudeSave;
}

/**
 * @brief Setzt den Wert des Fortitude-Rettungswurfs
 * 
 * @param modifier Der neue Wert des Fortitude-Rettungswurfs
 */
void Character::setFortitudeSave(int modifier)
{
    m_fortitudeSave = modifier;
}

/**
 * @brief Würfelt den Will-Rettungswurf für den Charakter
 * 
 * Simuliert einen W20-Würfelwurf (1-20) und speichert das Ergebnis.
 * Verwendet den Mersenne-Twister-Algorithmus für eine gute Zufallsverteilung.
 * 
 * @return Der gewürfelte Wert des Will-Rettungswurfs
 */
int Character::rollWillSave()
{
    m_lastWillSaveRoll = s_d20(s_gen);
    return m_lastWillSaveRoll + m_willSave;
}

/**
 * @brief Würfelt den Reflex-Rettungswurf für den Charakter
 * 
 * Simuliert einen W20-Würfelwurf (1-20) und speichert das Ergebnis.
 * Verwendet den Mersenne-Twister-Algorithmus für eine gute Zufallsverteilung.
 * 
 * @return Der gewürfelte Wert des Reflex-Rettungswurfs
 */
int Character::rollReflexSave()
{
    m_lastReflexSaveRoll = s_d20(s_gen);
    return m_lastReflexSaveRoll + m_reflexSave;
}

/**
 * @brief Würfelt den Fortitude-Rettungswurf für den Charakter
 * 
 * Simuliert einen W20-Würfelwurf (1-20) und speichert das Ergebnis.
 * Verwendet den Mersenne-Twister-Algorithmus für eine gute Zufallsverteilung.
 * 
 * @return Der gewürfelte Wert des Fortitude-Rettungswurfs
 */
int Character::rollFortitudeSave()
{
    m_lastFortitudeSaveRoll = s_d20(s_gen);
    return m_lastFortitudeSaveRoll + m_fortitudeSave;
}

/**
 * @brief Gibt den gewürfelten Wert des letzten Will-Rettungswurfs zurück
 * 
 * @return Der gewürfelte Wert des letzten Will-Rettungswurfs
 */
int Character::getLastWillSaveRoll() const
{
    return m_lastWillSaveRoll;
}

/**
 * @brief Gibt den gewürfelten Wert des letzten Reflex-Rettungswurfs zurück
 * 
 * @return Der gewürfelte Wert des letzten Reflex-Rettungswurfs
 */
int Character::getLastReflexSaveRoll() const
{
    return m_lastReflexSaveRoll;
}

/**
 * @brief Gibt den gewürfelten Wert des letzten Fortitude-Rettungswurfs zurück
 * 
 * @return Der gewürfelte Wert des letzten Fortitude-Rettungswurfs
 */
int Character::getLastFortitudeSaveRoll() const
{
    return m_lastFortitudeSaveRoll;
} 