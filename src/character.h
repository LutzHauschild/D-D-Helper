#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <random>

class Character {
public:
    Character();
    Character(const QString &name, int initiativeModifier);
    
    QString getName() const;
    void setName(const QString &name);
    
    int getInitiativeModifier() const;
    void setInitiativeModifier(int modifier);
    
    int getInitiativeRoll() const;
    void rollInitiative();
    
    int getTotalInitiative() const;
    
private:
    QString m_name;
    int m_initiativeModifier;
    int m_initiativeRoll;
    
    static std::random_device s_rd;
    static std::mt19937 s_gen;
    static std::uniform_int_distribution<> s_d20;
};

#endif // CHARACTER_H 