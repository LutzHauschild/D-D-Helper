#include <QtTest>
#include "../src/character.h"

/**
 * @brief Die TestCharacter-Klasse enthält Unit-Tests für die Character-Klasse.
 */
class TestCharacter : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Initialisierung vor jedem Test.
     */
    void init();

    /**
     * @brief Testet den Standardkonstruktor.
     */
    void testDefaultConstructor();

    /**
     * @brief Testet den Konstruktor mit Parametern.
     */
    void testParameterizedConstructor();

    /**
     * @brief Testet die Getter und Setter für den Namen.
     */
    void testNameGetterSetter();

    /**
     * @brief Testet die Getter und Setter für den Initiative-Modifikator.
     */
    void testInitiativeModifierGetterSetter();

    /**
     * @brief Testet das Würfeln der Initiative.
     */
    void testRollInitiative();

    /**
     * @brief Testet die Berechnung des Gesamt-Initiative-Werts.
     */
    void testGetTotalInitiative();


private:
    Character *m_character;
};

void TestCharacter::init()
{
    // Erstellt einen neuen Character für jeden Test
    m_character = new Character();
}

void TestCharacter::testDefaultConstructor()
{
    // Überprüft, ob der Standardkonstruktor die Werte korrekt initialisiert
    QCOMPARE(m_character->getName(), QString(""));
    QCOMPARE(m_character->getInitiativeModifier(), 0);
    QCOMPARE(m_character->getInitiativeRoll(), 0);
    QCOMPARE(m_character->getTotalInitiative(), 0);

    delete m_character;
}

void TestCharacter::testParameterizedConstructor()
{
    // Erstellt einen Character mit dem parametrisierten Konstruktor
    QString name = "Test Character";
    int initiativeModifier = 5;
    Character character(name, initiativeModifier);

    // Überprüft, ob der Konstruktor die Werte korrekt initialisiert
    QCOMPARE(character.getName(), name);
    QCOMPARE(character.getInitiativeModifier(), initiativeModifier);
    QCOMPARE(character.getInitiativeRoll(), 0);
    QCOMPARE(character.getTotalInitiative(), initiativeModifier);

    delete m_character;
}

void TestCharacter::testNameGetterSetter()
{
    // Testet den Setter für den Namen
    QString name = "New Name";
    m_character->setName(name);

    // Überprüft, ob der Getter den korrekten Wert zurückgibt
    QCOMPARE(m_character->getName(), name);

    delete m_character;
}

void TestCharacter::testInitiativeModifierGetterSetter()
{
    // Testet den Setter für den Initiative-Modifikator
    int initiativeModifier = 3;
    m_character->setInitiativeModifier(initiativeModifier);

    // Überprüft, ob der Getter den korrekten Wert zurückgibt
    QCOMPARE(m_character->getInitiativeModifier(), initiativeModifier);

    delete m_character;
}

void TestCharacter::testRollInitiative()
{
    // Würfelt die Initiative
    m_character->rollInitiative();

    // Überprüft, ob der Würfelwurf im gültigen Bereich liegt (1-20)
    int roll = m_character->getInitiativeRoll();
    QVERIFY(roll >= 1 && roll <= 20);

    delete m_character;
}

void TestCharacter::testGetTotalInitiative()
{
    // Setzt den Initiative-Modifikator
    int initiativeModifier = 4;
    m_character->setInitiativeModifier(initiativeModifier);

    // Würfelt die Initiative
    m_character->rollInitiative();
    int roll = m_character->getInitiativeRoll();

    // Überprüft, ob der Gesamt-Initiative-Wert korrekt berechnet wird
    QCOMPARE(m_character->getTotalInitiative(), roll + initiativeModifier);

    delete m_character;
}

QTEST_APPLESS_MAIN(TestCharacter)
#include "tst_character.moc" 