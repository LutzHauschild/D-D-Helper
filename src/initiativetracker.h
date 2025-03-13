#ifndef INITIATIVETRACKER_H
#define INITIATIVETRACKER_H

#include <QVector>
#include <QObject>
#include "character.h"

class InitiativeTracker : public QObject {
    Q_OBJECT
    
public:
    explicit InitiativeTracker(QObject *parent = nullptr);
    
    void addCharacter(const Character &character);
    void removeCharacter(int index);
    void clearCharacters();
    
    QVector<Character> getCharacters() const;
    
    void rollAllInitiatives();
    QVector<Character> getSortedInitiativeOrder() const;
    
signals:
    void charactersChanged();
    void initiativeRolled();
    
private:
    QVector<Character> m_characters;
};

#endif // INITIATIVETRACKER_H 