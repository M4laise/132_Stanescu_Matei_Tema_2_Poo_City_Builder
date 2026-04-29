#include <iostream>
#include "component.h"
#include "building.h"

Building::Building(std::string name, int cost, bool canBeDeactivated) : name(name), cost(cost), active(true), canBeDeactivated(canBeDeactivated), energyAvailable(true) {}

Building::~Building() 
{
    for (Component* component : components) 
    {
        delete component;
    }
    components.clear();
}

std::ostream& operator<<(std::ostream& os, const Building& b) 
{
    os << b.name << " " << b.showInfo();
    return os;
}

std::string Building::getName() const{
    return name;
}
bool Building::isActive() const{
    return active;
}
bool Building::getCanBeDeactivated() const{
    return canBeDeactivated;
}
int Building::getCost() const{
    return cost;
}

void Building::addComponent(Component* component) 
{
    if (component != nullptr) 
        components.push_back(component);
}

void Building::deactivate()
{
    if (!canBeDeactivated || !active)
        return;

    active = false;
    onDeactivate();
}

void Building::activate()
{
    if (!canBeDeactivated || active)
        return;

    active = true;
    onActivate();
}

void Building::setActive(bool active)
{
    this->active = active;
}

bool Building::isEnergyAvailable() const
{
    return energyAvailable;
}

void Building::setEnergyAvailable(bool available)
{
    energyAvailable = available;
}

void Building::onTurnStart() {}
void Building::onTurnEnd() {}
void Building::onBuy() {};
void Building::onDeactivate() {};
void Building::onActivate() {};