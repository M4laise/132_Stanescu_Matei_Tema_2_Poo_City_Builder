#ifndef CPEOPLEHAVER_H
#define CPEOPLEHAVER_H

#include "component.h"

class GameManager;

class CPeopleHaver : public Component
{
private:
    int people;
    int maxPeople;
    GameManager& game;

public:
    CPeopleHaver(int startingPeople, int maxPeople);
    virtual ~CPeopleHaver();

    int getPeople() const;
    int getMaxPeople() const;

    void increasePeople();
    void decreasePeople();
};

#endif