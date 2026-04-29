#include "CPeopleHaver.h"
#include "game_manager.h"

CPeopleHaver::CPeopleHaver(int startingPeople, int maxPeople) : game(GameManager::getInstance()), people(startingPeople), maxPeople(maxPeople)
{
    if (maxPeople < 0)
        maxPeople = 0;

    if (people < 0)
        people = 0;

    if (people > maxPeople)
        people = maxPeople;

    if (people > 0)
        game.modifyResource(ResourceType::Population, people);
}

CPeopleHaver::~CPeopleHaver()
{
    if (people > 0)
        game.modifyResource(ResourceType::Population, -people);
}

int CPeopleHaver::getPeople() const {
    return people;
}

int CPeopleHaver::getMaxPeople() const {
    return maxPeople;
}

void CPeopleHaver::increasePeople()
{
    if (people >= maxPeople)
        return;

    people++;
    game.modifyResource(ResourceType::Population, 1);
}

void CPeopleHaver::decreasePeople()
{
    if (people <= 0)
        return;

    people--;
    game.modifyResource(ResourceType::Population, -1);
}