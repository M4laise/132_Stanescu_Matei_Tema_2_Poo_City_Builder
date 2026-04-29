#include "game_manager.h"
#include "park.h"

Park::Park() : Building(Park::name, Park::COST) {}

Park::~Park() = default;

Building* Park::clone() const
{
    return new Park(*this);
}

std::string Park::showInfo() const
{
    return "       Maintenance: " + std::to_string(MAINTENANCE_COST) + "$";
}

void Park::onBuy()
{
    GameManager::getInstance().modifyResource(ResourceType::Happiness, HAPPINESS_BOOST);
}

void Park::onTurnEnd()
{
    GameManager::getInstance().modifyResource(ResourceType::Money, -MAINTENANCE_COST);
}
