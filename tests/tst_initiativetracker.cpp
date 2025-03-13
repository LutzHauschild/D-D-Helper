#include <QtTest>
#include <QSignalSpy>
#include "../src/initiativetracker.h"

/**
 * @brief Die TestInitiativeTracker-Klasse enthält Unit-Tests für die InitiativeTracker-Klasse.
 */
class TestInitiativeTracker : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Initialisierung vor jedem Test.
     */
    void init();

    /**
     * @brief Aufräumen nach jedem Test.
     */
    void cleanup();

    /**
     * @brief Testet das Hinzufügen eines Charakters.
     */
    void testAddCharacter();

    /**
     * @brief Testet das Entfernen eines Charakters.
     */
    void testRemoveCharacter();

    /**
     * @brief Testet das Leeren der Charakterliste.
     */
    void testClearCharacters();

    /**
     * @brief Testet das Würfeln der Initiative für alle Charaktere.
     */
    void testRollAllInitiatives();

    /**
     * @brief Testet das Sortieren der Charaktere nach Initiative.
     */
    void testGetSortedInitiativeOrder();

    /**
     * @brief Testet das Speichern und Laden der Charakterdaten.
     */
    void testSaveAndLoadFromFile();

private:
    InitiativeTracker *m_initiativeTracker;
    QSignalSpy *m_charactersChangedSpy;
    QSignalSpy *m_initiativeRolledSpy;
};

void TestInitiativeTracker::init()
{
    // Erstellt einen neuen InitiativeTracker für jeden Test
    m_initiativeTracker = new InitiativeTracker();
    
    // Erstellt Signal-Spies für die Signale des InitiativeTrackers
    m_charactersChangedSpy = new QSignalSpy(m_initiativeTracker, &InitiativeTracker::charactersChanged);
    m_initiativeRolledSpy = new QSignalSpy(m_initiativeTracker, &InitiativeTracker::initiativeRolled);
}

void TestInitiativeTracker::cleanup()
{
    // Gibt die Ressourcen frei
    delete m_initiativeTracker;
    delete m_charactersChangedSpy;
    delete m_initiativeRolledSpy;
}

void TestInitiativeTracker::testAddCharacter()
{
    // Erstellt einen Testcharakter
    Character character("Test Character", 3);
    
    // Fügt den Charakter zum InitiativeTracker hinzu
    m_initiativeTracker->addCharacter(character);
    
    // Überprüft, ob der Charakter hinzugefügt wurde
    QVector<Character> characters = m_initiativeTracker->getCharacters();
    QCOMPARE(characters.size(), 1);
    QCOMPARE(characters[0].getName(), QString("Test Character"));
    QCOMPARE(characters[0].getInitiativeModifier(), 3);
    
    // Überprüft, ob das Signal charactersChanged gesendet wurde
    QCOMPARE(m_charactersChangedSpy->count(), 1);
}

void TestInitiativeTracker::testRemoveCharacter()
{
    // Fügt zwei Charaktere hinzu
    m_initiativeTracker->addCharacter(Character("Character 1", 1));
    m_initiativeTracker->addCharacter(Character("Character 2", 2));
    
    // Setzt den Spy zurück
    m_charactersChangedSpy->clear();
    
    // Entfernt den ersten Charakter
    m_initiativeTracker->removeCharacter(0);
    
    // Überprüft, ob der Charakter entfernt wurde
    QVector<Character> characters = m_initiativeTracker->getCharacters();
    QCOMPARE(characters.size(), 1);
    QCOMPARE(characters[0].getName(), QString("Character 2"));
    
    // Überprüft, ob das Signal charactersChanged gesendet wurde
    QCOMPARE(m_charactersChangedSpy->count(), 1);
    
    // Versucht, einen ungültigen Index zu entfernen
    m_charactersChangedSpy->clear();
    m_initiativeTracker->removeCharacter(5);
    
    // Überprüft, ob kein Signal gesendet wurde
    QCOMPARE(m_charactersChangedSpy->count(), 0);
}

void TestInitiativeTracker::testClearCharacters()
{
    // Fügt einige Charaktere hinzu
    m_initiativeTracker->addCharacter(Character("Character 1", 1));
    m_initiativeTracker->addCharacter(Character("Character 2", 2));
    
    // Setzt den Spy zurück
    m_charactersChangedSpy->clear();
    
    // Leert die Charakterliste
    m_initiativeTracker->clearCharacters();
    
    // Überprüft, ob die Liste leer ist
    QCOMPARE(m_initiativeTracker->getCharacters().size(), 0);
    
    // Überprüft, ob das Signal charactersChanged gesendet wurde
    QCOMPARE(m_charactersChangedSpy->count(), 1);
}

void TestInitiativeTracker::testRollAllInitiatives()
{
    // Fügt einige Charaktere hinzu
    m_initiativeTracker->addCharacter(Character("Character 1", 1));
    m_initiativeTracker->addCharacter(Character("Character 2", 2));
    
    // Würfelt die Initiative für alle Charaktere
    m_initiativeTracker->rollAllInitiatives();
    
    // Überprüft, ob die Würfelwürfe im gültigen Bereich liegen
    QVector<Character> characters = m_initiativeTracker->getCharacters();
    for (const auto &character : characters) {
        int roll = character.getInitiativeRoll();
        QVERIFY(roll >= 1 && roll <= 20);
    }
    
    // Überprüft, ob das Signal initiativeRolled gesendet wurde
    QCOMPARE(m_initiativeRolledSpy->count(), 1);
}

void TestInitiativeTracker::testGetSortedInitiativeOrder()
{
    // Erstellt Charaktere mit bekannten Werten
    Character character1("Character 1", 1);
    Character character2("Character 2", 2);
    Character character3("Character 3", 3);
    
    // Setzt die Initiative-Würfe manuell (normalerweise würde rollInitiative() aufgerufen)
    // Dies ist ein Hack für den Test, da wir keinen direkten Zugriff auf m_initiativeRoll haben
    character1.rollInitiative();
    character2.rollInitiative();
    character3.rollInitiative();
    
    // Fügt die Charaktere zum InitiativeTracker hinzu
    m_initiativeTracker->addCharacter(character1);
    m_initiativeTracker->addCharacter(character2);
    m_initiativeTracker->addCharacter(character3);
    
    // Holt die sortierte Liste
    QVector<Character> sortedCharacters = m_initiativeTracker->getSortedInitiativeOrder();
    
    // Überprüft, ob die Liste sortiert ist (höchster Wert zuerst)
    for (int i = 0; i < sortedCharacters.size() - 1; ++i) {
        QVERIFY(sortedCharacters[i].getTotalInitiative() >= sortedCharacters[i + 1].getTotalInitiative());
    }
}

void TestInitiativeTracker::testSaveAndLoadFromFile()
{
    // Erstellt einen temporären Dateinamen für den Test
    QString tempFileName = "test_characters.json";
    
    // Fügt einige Charaktere hinzu
    m_initiativeTracker->addCharacter(Character("Character 1", 1));
    m_initiativeTracker->addCharacter(Character("Character 2", 2));
    
    // Speichert die Charaktere in der Datei
    bool saveSuccess = m_initiativeTracker->saveToFile(tempFileName);
    QVERIFY(saveSuccess);
    
    // Erstellt einen neuen InitiativeTracker
    InitiativeTracker newTracker;
    
    // Lädt die Charaktere aus der Datei
    bool loadSuccess = newTracker.loadFromFile(tempFileName);
    QVERIFY(loadSuccess);
    
    // Überprüft, ob die geladenen Charaktere mit den gespeicherten übereinstimmen
    QVector<Character> originalCharacters = m_initiativeTracker->getCharacters();
    QVector<Character> loadedCharacters = newTracker.getCharacters();
    
    QCOMPARE(loadedCharacters.size(), originalCharacters.size());
    
    for (int i = 0; i < originalCharacters.size(); ++i) {
        QCOMPARE(loadedCharacters[i].getName(), originalCharacters[i].getName());
        QCOMPARE(loadedCharacters[i].getInitiativeModifier(), originalCharacters[i].getInitiativeModifier());
    }
    
    // Löscht die temporäre Datei
    QFile file(tempFileName);
    if (file.exists()) {
        file.remove();
    }
}

QTEST_MAIN(TestInitiativeTracker)
#include "tst_initiativetracker.moc" 