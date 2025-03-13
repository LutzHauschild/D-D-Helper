#include "character.h"

std::random_device Character::s_rd;
std::mt19937 Character::s_gen(Character::s_rd());
std::uniform_int_distribution<> Character::s_d20(1, 20);

Character::Character()
    : m_name(""), m_initiativeModifier(0), m_initiativeRoll(0)
{
}

Character::Character(const QString &name, int initiativeModifier)
    : m_name(name), m_initiativeModifier(initiativeModifier), m_initiativeRoll(0)
{
}

QString Character::getName() const
{
    return m_name;
}

void Character::setName(const QString &name)
{
    m_name = name;
}

int Character::getInitiativeModifier() const
{
    return m_initiativeModifier;
}

void Character::setInitiativeModifier(int modifier)
{
    m_initiativeModifier = modifier;
}

int Character::getInitiativeRoll() const
{
    return m_initiativeRoll;
}

void Character::rollInitiative()
{
    m_initiativeRoll = s_d20(s_gen);
}

int Character::getTotalInitiative() const
{
    return m_initiativeRoll + m_initiativeModifier;
} 