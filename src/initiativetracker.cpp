#include "initiativetracker.h"
#include <algorithm>

InitiativeTracker::InitiativeTracker(QObject *parent)
    : QObject(parent)
{
}

void InitiativeTracker::addCharacter(const Character &character)
{
    m_characters.append(character);
    emit charactersChanged();
}

void InitiativeTracker::removeCharacter(int index)
{
    if (index >= 0 && index < m_characters.size()) {
        m_characters.removeAt(index);
        emit charactersChanged();
    }
}

void InitiativeTracker::clearCharacters()
{
    m_characters.clear();
    emit charactersChanged();
}

QVector<Character> InitiativeTracker::getCharacters() const
{
    return m_characters;
}

void InitiativeTracker::rollAllInitiatives()
{
    for (auto &character : m_characters) {
        character.rollInitiative();
    }
    emit initiativeRolled();
}

QVector<Character> InitiativeTracker::getSortedInitiativeOrder() const
{
    QVector<Character> sortedCharacters = m_characters;
    
    std::sort(sortedCharacters.begin(), sortedCharacters.end(), 
              [](const Character &a, const Character &b) {
                  return a.getTotalInitiative() > b.getTotalInitiative();
              });
    
    return sortedCharacters;
} 