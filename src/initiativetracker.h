#ifndef INITIATIVETRACKER_H
#define INITIATIVETRACKER_H

#include <QVector>
#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "character.h"

/**
 * @brief Die InitiativeTracker-Klasse verwaltet die Charaktere und ihre Initiative-Werte.
 * 
 * Diese Klasse ist für die Verwaltung der Charakterliste verantwortlich. Sie ermöglicht
 * das Hinzufügen, Entfernen und Leeren der Charakterliste, sowie das Würfeln der Initiative
 * für alle Charaktere und das Sortieren der Charaktere nach ihren Initiative-Werten.
 * 
 * Die Klasse erbt von QObject, um Signale und Slots zu unterstützen.
 */
class InitiativeTracker : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief Konstruktor für den InitiativeTracker.
     * 
     * @param parent Der Eltern-QObject (für die Qt-Objekthierarchie)
     */
    explicit InitiativeTracker(QObject *parent = nullptr);
    
    /**
     * @brief Fügt einen Charakter zur Liste hinzu.
     * 
     * Sendet das Signal charactersChanged() nach dem Hinzufügen.
     * 
     * @param character Der hinzuzufügende Charakter
     */
    void addCharacter(const Character &character);
    
    /**
     * @brief Entfernt einen Charakter aus der Liste anhand seines Index.
     * 
     * Sendet das Signal charactersChanged() nach dem Entfernen, wenn der Index gültig ist.
     * 
     * @param index Der Index des zu entfernenden Charakters
     */
    void removeCharacter(int index);
    
    /**
     * @brief Leert die gesamte Charakterliste.
     * 
     * Sendet das Signal charactersChanged() nach dem Leeren.
     */
    void clearCharacters();
    
    /**
     * @brief Gibt die aktuelle Charakterliste zurück.
     * 
     * @return Ein QVector mit allen Charakteren
     */
    QVector<Character> getCharacters() const;
    
    /**
     * @brief Würfelt die Initiative für alle Charaktere in der Liste.
     * 
     * Ruft für jeden Charakter die rollInitiative()-Methode auf und
     * sendet das Signal initiativeRolled() nach dem Würfeln.
     */
    void rollAllInitiatives();
    
    /**
     * @brief Gibt eine nach Initiative sortierte Charakterliste zurück.
     * 
     * Die Charaktere werden nach ihrem Gesamt-Initiative-Wert (Wurf + Modifikator)
     * in absteigender Reihenfolge sortiert (höchster Wert zuerst).
     * 
     * @return Ein QVector mit den sortierten Charakteren
     */
    QVector<Character> getSortedInitiativeOrder() const;
    
    /**
     * @brief Speichert die Charakterliste in einer Datei.
     * 
     * Die Charakterdaten werden im JSON-Format in der angegebenen Datei gespeichert.
     * 
     * @param filename Der Dateiname zum Speichern der Daten
     * @return true, wenn das Speichern erfolgreich war, sonst false
     */
    bool saveToFile(const QString &filename = "characters.json");
    
    /**
     * @brief Lädt die Charakterliste aus einer Datei.
     * 
     * Die Charakterdaten werden aus der angegebenen JSON-Datei geladen.
     * 
     * @param filename Der Dateiname zum Laden der Daten
     * @return true, wenn das Laden erfolgreich war, sonst false
     */
    bool loadFromFile(const QString &filename = "characters.json");
    
signals:
    /**
     * @brief Signal, das gesendet wird, wenn sich die Charakterliste ändert.
     * 
     * Wird beim Hinzufügen, Entfernen oder Leeren der Liste gesendet.
     */
    void charactersChanged();
    
    /**
     * @brief Signal, das gesendet wird, wenn die Initiative gewürfelt wurde.
     * 
     * Wird nach dem Aufruf von rollAllInitiatives() gesendet.
     */
    void initiativeRolled();
    
private:
    QVector<Character> m_characters;  ///< Die Liste der Charaktere
};

#endif // INITIATIVETRACKER_H 