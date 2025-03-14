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
 * Qt-Konzept: QObject und Vererbung
 * Die Klasse erbt von QObject, der Basisklasse für alle Qt-Objekte. Dies ermöglicht:
 * - Verwendung des Signale-und-Slots-Mechanismus
 * - Nutzung des Qt-Metaobjektsystems
 * - Speicherverwaltung durch Eltern-Kind-Beziehungen
 */
class InitiativeTracker : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief Konstruktor für den InitiativeTracker.
     * 
     * Qt-Konzept: Eltern-Kind-Beziehung
     * Der parent-Parameter ermöglicht die Einbindung in die Qt-Objekthierarchie.
     * Wenn der Elternteil zerstört wird, werden alle Kinder automatisch zerstört.
     * Dies ist Teil der automatischen Speicherverwaltung von Qt.
     * 
     * @param parent Der Eltern-QObject (für die Qt-Objekthierarchie)
     */
    explicit InitiativeTracker(QObject *parent = nullptr);
    
    /**
     * @brief Fügt einen Charakter zur Liste hinzu.
     * 
     * Qt-Konzept: Signale
     * Nach dem Hinzufügen wird ein Signal gesendet, um andere Teile der Anwendung
     * über die Änderung zu informieren. Dies ist ein Beispiel für lose Kopplung,
     * da der Sender nicht wissen muss, wer auf das Signal reagiert.
     * 
     * @param character Der hinzuzufügende Charakter
     */
    void addCharacter(const Character &character);
    
    /**
     * @brief Entfernt einen Charakter aus der Liste anhand seines Index.
     * 
     * Qt-Konzept: QVector
     * QVector ist Qt's Implementierung eines dynamischen Arrays, ähnlich zu std::vector.
     * Es bietet Methoden wie removeAt() zum Entfernen von Elementen an bestimmten Positionen.
     * 
     * @param index Der Index des zu entfernenden Charakters
     */
    void removeCharacter(int index);
    
    /**
     * @brief Leert die gesamte Charakterliste.
     * 
     * Qt-Konzept: Signale als Benachrichtigungsmechanismus
     * Auch hier wird nach der Operation ein Signal gesendet, um andere Komponenten
     * zu benachrichtigen. Dies ist ein wichtiger Teil des Observer-Patterns in Qt.
     */
    void clearCharacters();
    
    /**
     * @brief Gibt die aktuelle Charakterliste zurück.
     * 
     * Qt-Konzept: Wertesemantik
     * QVector verwendet Wertesemantik mit implizitem Sharing. Das bedeutet, dass
     * beim Zurückgeben des Vektors eine flache Kopie erstellt wird, die effizient ist,
     * da die Daten erst bei Änderungen tatsächlich kopiert werden (Copy-on-Write).
     * 
     * @return Ein QVector mit allen Charakteren
     */
    QVector<Character> getCharacters() const;
    
    /**
     * @brief Aktualisiert einen Charakter in der Liste.
     * 
     * @param index Der Index des zu aktualisierenden Charakters
     * @param character Der aktualisierte Charakter
     */
    void updateCharacter(int index, const Character &character);
    
    /**
     * @brief Würfelt die Initiative für alle Charaktere in der Liste.
     * 
     * Qt-Konzept: Iteration über Container
     * Diese Methode zeigt, wie man über einen Qt-Container iteriert.
     * In der Implementierung wird eine for-Schleife oder ein Iterator verwendet.
     */
    void rollAllInitiatives();
    
    /**
     * @brief Würfelt die Initiative für einen einzelnen Charakter.
     * 
     * @param index Der Index des Charakters
     */
    void rollInitiativeForCharacter(int index);
    
    /**
     * @brief Gibt eine nach Initiative sortierte Charakterliste zurück.
     * 
     * Qt-Konzept: Algorithmen und Sortieren
     * Qt-Container können mit std::sort oder Qt-eigenen Sortierfunktionen
     * sortiert werden. Hier wird eine sortierte Kopie der Charakterliste erstellt.
     * 
     * @return Ein QVector mit den sortierten Charakteren
     */
    QVector<Character> getSortedInitiativeOrder() const;
    
    /**
     * @brief Speichert die Charakterliste in einer Datei.
     * 
     * Qt-Konzept: JSON-Verarbeitung
     * Qt bietet umfangreiche Unterstützung für JSON mit Klassen wie QJsonObject,
     * QJsonArray und QJsonDocument. Diese werden hier verwendet, um die Charakterdaten
     * in ein JSON-Format zu konvertieren und in eine Datei zu schreiben.
     * 
     * Qt-Konzept: Dateioperationen
     * QFile ist Qt's Klasse für Dateioperationen. Sie bietet plattformunabhängige
     * Methoden zum Öffnen, Lesen, Schreiben und Schließen von Dateien.
     * 
     * @param filename Der Dateiname zum Speichern der Daten
     * @return true, wenn das Speichern erfolgreich war, sonst false
     */
    bool saveToFile(const QString &filename = "characters.json");
    
    /**
     * @brief Lädt die Charakterliste aus einer Datei.
     * 
     * Qt-Konzept: Fehlerbehandlung
     * Qt-Funktionen geben oft bool-Werte zurück, um Erfolg oder Misserfolg anzuzeigen.
     * Dies ist ein einfacher Ansatz zur Fehlerbehandlung, der hier verwendet wird.
     * 
     * @param filename Der Dateiname zum Laden der Daten
     * @return true, wenn das Laden erfolgreich war, sonst false
     */
    bool loadFromFile(const QString &filename = "characters.json");
    
    /**
     * @brief Würfelt Willenskraft-Rettungswürfe für alle Charaktere.
     * 
     * Ruft für jeden Charakter die rollWillSave()-Methode auf und
     * sendet das Signal savesRolled() nach dem Würfeln.
     */
    void rollAllWillSaves();
    
    /**
     * @brief Würfelt einen Willenskraft-Rettungswurf für einen einzelnen Charakter.
     * 
     * @param index Der Index des Charakters
     */
    void rollWillSaveForCharacter(int index);
    
    /**
     * @brief Würfelt Reflex-Rettungswürfe für alle Charaktere.
     * 
     * Ruft für jeden Charakter die rollReflexSave()-Methode auf und
     * sendet das Signal savesRolled() nach dem Würfeln.
     */
    void rollAllReflexSaves();
    
    /**
     * @brief Würfelt einen Reflex-Rettungswurf für einen einzelnen Charakter.
     * 
     * @param index Der Index des Charakters
     */
    void rollReflexSaveForCharacter(int index);
    
    /**
     * @brief Würfelt Konstitution-Rettungswürfe für alle Charaktere.
     * 
     * Ruft für jeden Charakter die rollFortitudeSave()-Methode auf und
     * sendet das Signal savesRolled() nach dem Würfeln.
     */
    void rollAllFortitudeSaves();
    
    /**
     * @brief Würfelt einen Konstitution-Rettungswurf für einen einzelnen Charakter.
     * 
     * @param index Der Index des Charakters
     */
    void rollFortitudeSaveForCharacter(int index);
    
signals:
    /**
     * @brief Signal, das gesendet wird, wenn sich die Charakterliste ändert.
     * 
     * Qt-Konzept: Signale
     * Signale sind spezielle Methoden, die von einer Klasse deklariert werden können.
     * Sie haben keine Implementierung und werden mit dem emit-Schlüsselwort ausgelöst.
     * Signale können mit Slots verbunden werden, die aufgerufen werden, wenn das Signal
     * ausgelöst wird. Dies ist ein zentraler Mechanismus für die Kommunikation zwischen Objekten in Qt.
     */
    void charactersChanged();
    
    /**
     * @brief Signal, das gesendet wird, wenn die Initiative gewürfelt wurde.
     * 
     * Qt-Konzept: Signale ohne Parameter
     * Signale können ohne Parameter definiert werden, wenn sie nur eine Benachrichtigung
     * senden sollen, ohne zusätzliche Daten zu übermitteln.
     */
    void initiativeRolled();
    
    /**
     * @brief Signal, das gesendet wird, wenn Rettungswürfe gewürfelt wurden.
     * 
     * Wird nach dem Aufruf von rollAllWillSaves(), rollAllReflexSaves() oder
     * rollAllFortitudeSaves() gesendet.
     */
    void savesRolled();
    
private:
    /**
     * Qt-Konzept: QVector als Container
     * QVector ist Qt's Implementierung eines dynamischen Arrays. Es bietet ähnliche
     * Funktionalität wie std::vector, ist aber besser in das Qt-Framework integriert.
     * QVector unterstützt implizites Sharing (Copy-on-Write) für effiziente Kopieroperationen.
     */
    QVector<Character> m_characters;  ///< Die Liste der Charaktere
};

#endif // INITIATIVETRACKER_H 