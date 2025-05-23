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
 * 
 * C++ Konzept: Klassen
 * Eine Klasse ist eine Blaupause für Objekte. Sie definiert Attribute (Daten)
 * und Methoden (Funktionen), die auf diesen Daten operieren. Dies ist ein
 * grundlegendes Konzept der objektorientierten Programmierung.
 */
class Character {
public:
    /**
     * @brief Standardkonstruktor, der einen leeren Charakter erstellt.
     * 
     * C++ Konzept: Konstruktoren
     * Konstruktoren sind spezielle Methoden, die beim Erstellen eines Objekts
     * aufgerufen werden. Sie initialisieren die Attribute des Objekts.
     * Ein Standardkonstruktor nimmt keine Parameter entgegen.
     */
    Character();
    
    /**
     * @brief Konstruktor, der einen Charakter mit Namen und Initiative-Modifikator erstellt.
     * 
     * C++ Konzept: Überladung
     * In C++ können mehrere Funktionen denselben Namen haben, solange sie sich
     * in ihren Parametern unterscheiden. Dies wird als Überladung bezeichnet.
     * Hier haben wir zwei Konstruktoren mit unterschiedlichen Parametern.
     * 
     * C++ Konzept: Referenzen
     * Der Parameter 'name' ist eine konstante Referenz (const &). Referenzen sind
     * Aliase für existierende Objekte und vermeiden unnötiges Kopieren.
     * 'const' bedeutet, dass die Methode den Parameter nicht ändern darf.
     * 
     * @param name Der Name des Charakters
     * @param initiativeModifier Der Initiative-Modifikator des Charakters
     */
    Character(const QString &name, int initiativeModifier);
    
    /**
     * @brief Erweiterter Konstruktor mit allen Attributen inklusive Rettungswürfen.
     * 
     * @param name Der Name des Charakters
     * @param initiativeModifier Der Initiative-Modifikator des Charakters
     * @param willSave Der Willenskraft-Rettungswurf-Modifikator
     * @param reflexSave Der Reflex-Rettungswurf-Modifikator
     * @param fortitudeSave Der Konstitution-Rettungswurf-Modifikator
     */
    Character(const QString &name, int initiativeModifier, int willSave, int reflexSave, int fortitudeSave);
    
    /**
     * @brief Gibt den Namen des Charakters zurück.
     * 
     * C++ Konzept: Getter-Methoden
     * Getter-Methoden ermöglichen kontrollierten Zugriff auf private Attribute.
     * Sie sind Teil des Kapselungsprinzips in der OOP.
     * 
     * C++ Konzept: const-Methoden
     * Das 'const' nach der Methodendeklaration bedeutet, dass diese Methode
     * keine Attribute des Objekts ändern darf. Dies ist wichtig für die
     * Const-Korrektheit und ermöglicht die Verwendung mit konstanten Objekten.
     * 
     * @return Der Name des Charakters als QString
     */
    QString getName() const;
    
    /**
     * @brief Setzt den Namen des Charakters.
     * 
     * C++ Konzept: Setter-Methoden
     * Setter-Methoden ermöglichen kontrollierten Zugriff zum Ändern privater Attribute.
     * Sie können Validierungen oder andere Logik vor dem Setzen des Werts ausführen.
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
     * C++ Konzept: Zufallszahlengenerierung in C++11
     * Diese Methode verwendet die C++11-Zufallszahlenbibliothek, die bessere
     * Zufallszahlen erzeugt als die alte rand()-Funktion. Sie verwendet statische
     * Klassenvariablen für den Generator und die Verteilung.
     */
    void rollInitiative();
    
    /**
     * @brief Berechnet den Gesamt-Initiative-Wert.
     * 
     * @return Die Summe aus gewürfeltem Wert und Initiative-Modifikator
     */
    int getTotalInitiative() const;
    
    // Neue Methoden für Rettungswürfe
    
    /**
     * @brief Gibt den Willenskraft-Rettungswurf-Modifikator zurück.
     * 
     * @return Der Willenskraft-Rettungswurf-Modifikator als Integer
     */
    int getWillSave() const;
    
    /**
     * @brief Setzt den Willenskraft-Rettungswurf-Modifikator.
     * 
     * @param modifier Der neue Willenskraft-Rettungswurf-Modifikator
     */
    void setWillSave(int modifier);
    
    /**
     * @brief Gibt den Reflex-Rettungswurf-Modifikator zurück.
     * 
     * @return Der Reflex-Rettungswurf-Modifikator als Integer
     */
    int getReflexSave() const;
    
    /**
     * @brief Setzt den Reflex-Rettungswurf-Modifikator.
     * 
     * @param modifier Der neue Reflex-Rettungswurf-Modifikator
     */
    void setReflexSave(int modifier);
    
    /**
     * @brief Gibt den Konstitution-Rettungswurf-Modifikator zurück.
     * 
     * @return Der Konstitution-Rettungswurf-Modifikator als Integer
     */
    int getFortitudeSave() const;
    
    /**
     * @brief Setzt den Konstitution-Rettungswurf-Modifikator.
     * 
     * @param modifier Der neue Konstitution-Rettungswurf-Modifikator
     */
    void setFortitudeSave(int modifier);
    
    /**
     * @brief Würfelt einen Willenskraft-Rettungswurf (W20 + Modifikator).
     * 
     * @return Das Ergebnis des Willenskraft-Rettungswurfs
     */
    int rollWillSave();
    
    /**
     * @brief Würfelt einen Reflex-Rettungswurf (W20 + Modifikator).
     * 
     * @return Das Ergebnis des Reflex-Rettungswurfs
     */
    int rollReflexSave();
    
    /**
     * @brief Würfelt einen Konstitution-Rettungswurf (W20 + Modifikator).
     * 
     * @return Das Ergebnis des Konstitution-Rettungswurfs
     */
    int rollFortitudeSave();
    
    /**
     * @brief Gibt den letzten gewürfelten Willenskraft-Rettungswurf zurück.
     * 
     * @return Der letzte gewürfelte Willenskraft-Rettungswurf
     */
    int getLastWillSaveRoll() const;
    
    /**
     * @brief Gibt den letzten gewürfelten Reflex-Rettungswurf zurück.
     * 
     * @return Der letzte gewürfelte Reflex-Rettungswurf
     */
    int getLastReflexSaveRoll() const;
    
    /**
     * @brief Gibt den letzten gewürfelten Konstitution-Rettungswurf zurück.
     * 
     * @return Der letzte gewürfelte Konstitution-Rettungswurf
     */
    int getLastFortitudeSaveRoll() const;
    
private:
    /**
     * C++ Konzept: Datenkapselung
     * Private Attribute sind von außen nicht direkt zugänglich. Dies verhindert
     * unbeabsichtigte Änderungen und versteckt die Implementierungsdetails.
     * Der Zugriff erfolgt nur über öffentliche Methoden (Getter und Setter).
     */
    QString m_name;                  ///< Der Name des Charakters
    int m_initiativeModifier;        ///< Der Initiative-Modifikator des Charakters
    int m_initiativeRoll;            ///< Der gewürfelte Initiative-Wert (1-20)
    
    // Neue Attribute für Rettungswürfe
    int m_willSave;                  ///< Der Willenskraft-Rettungswurf-Modifikator
    int m_reflexSave;                ///< Der Reflex-Rettungswurf-Modifikator
    int m_fortitudeSave;             ///< Der Konstitution-Rettungswurf-Modifikator
    int m_lastWillSaveRoll;          ///< Der letzte gewürfelte Willenskraft-Rettungswurf
    int m_lastReflexSaveRoll;        ///< Der letzte gewürfelte Reflex-Rettungswurf
    int m_lastFortitudeSaveRoll;     ///< Der letzte gewürfelte Konstitution-Rettungswurf
    
    /**
     * C++ Konzept: Statische Klassenvariablen
     * Statische Variablen gehören zur Klasse, nicht zu einzelnen Objekten.
     * Sie werden von allen Instanzen der Klasse geteilt. Hier werden sie für
     * die Zufallszahlengenerierung verwendet, damit nicht jedes Character-Objekt
     * seinen eigenen Generator erstellen muss.
     */
    static std::random_device s_rd;  ///< Zufallszahlengenerator für den Seed
    static std::mt19937 s_gen;       ///< Mersenne-Twister-Generator für bessere Zufallszahlen
    static std::uniform_int_distribution<> s_d20; ///< Gleichverteilung für W20 (1-20)
};

#endif // CHARACTER_H 