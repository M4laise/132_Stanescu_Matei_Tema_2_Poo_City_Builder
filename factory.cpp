#include "CEnergyConsumer.h"
#include "CWorkerHaver.h"
#include "factory.h"
#include "game_manager.h"

#include <algorithm>

Factory::Factory() : Building(Factory::name, Factory::COST, true), productionPerWorker(15), consecutiveEnergyFailures(0), activeTurns(0), accumulatedPollution(0)
{
    energyComponent = new CEnergyConsumer(6);
    addComponent(energyComponent);

    workerComponent = new CWorkerHaver(0, 5);
    addComponent(workerComponent);
}

Factory::~Factory() = default;

Building* Factory::clone() const
{
    return new Factory(*this);
}

std::string Factory::showInfo() const
{
    std::string info = std::to_string(workerComponent->getWorkerCount()) + "/" + std::to_string(workerComponent->getMaxWorkers());
    if (!isActive()) {
        info += " [Deactivated]";
    }
    return info;
}

void Factory::onDeactivate()
{
    // if (accumulatedPollution > 0) 
    // {
    //     GameManager::getInstance().modifyResource(ResourceType::Pollution, -accumulatedPollution);
    //     accumulatedPollution = 0;
    // }
    activeTurns = 0;
}

void Factory::onActivate()
{
    activeTurns = 0;
}

void Factory::onTurnEnd()
{
    if (isActive()) 
    {
        int workerCount = workerComponent->getWorkerCount();
        int baseMoneyGain = productionPerWorker * workerCount;
        int moneyGain = baseMoneyGain + (baseMoneyGain * activeTurns) / 5; // 20%
        GameManager::getInstance().modifyResource(ResourceType::Money, moneyGain);
        
        int pollutionGain = workerCount;
        GameManager::getInstance().modifyResource(ResourceType::Pollution, pollutionGain);
        accumulatedPollution += pollutionGain;

        activeTurns++;
    }
    if (!isActive() || getComponent<CWorkerHaver>()->getWorkerCount() == 0)
    {
        if (accumulatedPollution > 0)
        {
            int tempPollution = std::max(accumulatedPollution / 2 + 1, 1);

            if (accumulatedPollution % 10 == 0)
            {
                GameManager::getInstance().modifyResource(ResourceType::Happiness, 1);
            }

            accumulatedPollution -= tempPollution;
            GameManager::getInstance().modifyResource(ResourceType::Pollution, -tempPollution);
        }
    }

    if (!isEnergyAvailable()) 
    {
        consecutiveEnergyFailures++;
        int workersToLose = consecutiveEnergyFailures;
        for (int i = 0; i < workersToLose; ++i) {
            workerComponent->removeWorker();
        }
    } else
        consecutiveEnergyFailures = 0;
}
