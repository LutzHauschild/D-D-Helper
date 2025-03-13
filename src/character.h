#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <random>

/**
 * @brief Die Character-Klasse repräsentiert einen Charakter im D&D-Spiel.
 * 
 * Diese Klasse speichert Informationen über einen Charakter, einschließlich
 * seines Namens und Initiative-Modifikators. Sie bietet auch Funktionen zum
 * Würfeln der Initiative und zur Berechnung des Gesamtinitiative-Werts.
 */
class Character {
public:
    /**
     * @brief Standardkonstruktor, der einen leeren Charakter erstellt.
     */
    Character();
    
    /**
     * @brief Konstruktor, der einen Charakter mit Namen und Initiative-Modifikator erstellt.
     * 
     * @param name Der Name des Charakters
     * @param initiativeModifier Der Initiative-Modifikator des Charakters
     */
    Character(const QString &name, int initiativeModifier);
    
    /**
     * @brief Gibt den Namen des Charakters zurück.
     * 
     * @return Der Name des Charakters als QString
     */
    QString getName() const;
    
    /**
     * @brief Setzt den Namen des Charakters.
     * 
     * @param name Der neue Name des Charakters
     */
    void setName(const QString &name);
    
    /**
     * @brief Gibt den Initiative-Modifikator des Charakters zurück.
     * 
     * @return Der Initiative-Modifikator als Integer
     */
    int getInitiativeModifier() const;
    
    /**
     * @brief Setzt den Initiative-Modifikator des Charakters.
     * 
     * @param modifier Der neue Initiative-Modifikator
     */
    void setInitiativeModifier(int modifier);
    
    /**
     * @brief Gibt den gewürfelten Initiative-Wert zurück.
     * 
     * @return Der gewürfelte Initiative-Wert (1-20) als Integer
     */
    int getInitiativeRoll() const;
    
    /**
     * @brief Würfelt die Initiative für den Charakter (W20).
     * 
     * Diese Methode simuliert einen W20-Würfelwurf und speichert das Ergebnis.
     */
    void rollInitiative();
    
    /**
     * @brief Berechnet den Gesamt-Initiative-Wert.
     * 
     * @return Die Summe aus gewürfeltem Wert und Initiative-Modifikator
     */
    int getTotalInitiative() const;
    
private:
    QString m_name;                  ///< Der Name des Charakters
    int m_initiativeModifier;        ///< Der Initiative-Modifikator des Charakters
    int m_initiativeRoll;            ///< Der gewürfelte Initiative-Wert (1-20)
    
    // Statische Variablen für die Zufallszahlengenerierung
    static std::random_device s_rd;  ///< Zufallszahlengenerator für den Seed
    static std::mt19937 s_gen;       ///< Mersenne-Twister-Generator für bessere Zufallszahlen
    static std::uniform_int_distribution<> s_d20; ///< Gleichverteilung für W20 (1-20)
};

#endif // CHARACTER_H 