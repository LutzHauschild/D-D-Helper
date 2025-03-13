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
    : m_name(""), m_initiativeModifier(0), m_initiativeRoll(0)
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
    : m_name(name), m_initiativeModifier(initiativeModifier), m_initiativeRoll(0)
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