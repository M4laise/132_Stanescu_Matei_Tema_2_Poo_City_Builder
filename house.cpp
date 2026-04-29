#include <iostream>

#include "CEnergyConsumer.h"
#include "CPeopleHaver.h"
#include "game_manager.h"
#include "house.h"

House::House() : Building(House::name, House::COST), consecutiveEnergyFailures(0)
{
    peopleComponent = new CPeopleHaver(3, 3);
    addComponent(peopleComponent);

    energyComponent = new CEnergyConsumer(3);
    addComponent(energyComponent);
}

House::~House() = default;

Building* House::clone() const {
    return new House(*this);
}

std::string House::showInfo() const {
    return std::to_string(peopleComponent->getPeople()) + "/" + std::to_string(peopleComponent->getMaxPeople());
}

void House::onTurnEnd() 
{
    int moneyGain = peopleComponent->getPeople() * 15;
    GameManager::getInstance().modifyResource(ResourceType::Money, moneyGain);

    if (!isActive() || !isEnergyAvailable()) 
    {
        consecutiveEnergyFailures++;
        int peopleToLose = consecutiveEnergyFailures;
        for (int i = 0; i < peopleToLose; ++i) 
        {
            peopleComponent->decreasePeople();
        }
    } else 
    {
        consecutiveEnergyFailures = 0;
    }
}