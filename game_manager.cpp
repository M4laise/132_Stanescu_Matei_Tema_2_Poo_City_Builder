#include "exceptions.h"
#include "game_manager.h"
#include "building.h"
#include "CEnergyConsumer.h"
#include "CPeopleHaver.h"
#include "CWorkerHaver.h"
#include "factory.h"
#include "generator.h"
#include "house.h"
#include "park.h"

GameManager::GameManager() {}

void GameManager::initGameManager(int startMoney, int maxTurns, int moneyNeededToWin)
{
    if (startMoney < 0)
        throw InvalidGameConfigurationException("startMoney nu poate fi negativ");
    if (maxTurns <= 0)
        throw InvalidGameConfigurationException("maxTurns trebuie sa fie mai mare decat 0");
    if (moneyNeededToWin <= 0)
        throw InvalidGameConfigurationException("moneyNeededToWin trebuie sa fie mai mare decat 0");

    this->currentTurn = 1;
    this->resources[ResourceType::Money] = startMoney;
    this->resources[ResourceType::Energy] = 0;
    this->resources[ResourceType::Pollution] = 0;
    this->resources[ResourceType::Population] = 0;
    this->resources[ResourceType::Happiness] = 0;
    this->resources[ResourceType::Workers] = 0;
    this->maxTurns = maxTurns;
    this->moneyNeededToWin = moneyNeededToWin;

    for (Building* building : buildings) {
        delete building;
    }
    buildings.clear();
}

const std::vector<Building*>& GameManager::getBuildings() const{
    return buildings;
}

void GameManager::addBuilding(Building* building)
{
    if (building == nullptr)
        return;

    buildings.push_back(building);
}

void GameManager::buyBuilding(Building* building)
{
    if (building == nullptr)
        throw InvalidBuildingException("pointer nul");

    int cost = building->getCost();
    int available = resources[ResourceType::Money];
    if (available < cost) {
        delete building;
        throw InsufficientFundsException(cost, available);
    }

    modifyResource(ResourceType::Money, -cost);

    addBuilding(building);

    building->onBuy();

    if (building->getCanBeDeactivated())
        building->activate();
}

void GameManager::modifyResource(ResourceType resource, int amount)
{
    resources[resource] += amount;

    if (resources[resource] < 0 && resource != ResourceType::Happiness)
        resources[resource] = 0;
}

int GameManager::getResource(ResourceType resource) const
{
    return resources.at(resource);
}

int GameManager::getCurrentTurn() const
{
    return currentTurn;
}

int GameManager::getMaxTurns() const
{
    return maxTurns;
}

int GameManager::getMoneyNeededToWin() const
{
    return moneyNeededToWin;
}

void GameManager::removeWorkersFromPopulationLoss(int count)
{
    if (count <= 0)
        return;

    int remainingToRemove = count;

    for (Building* building : buildings)
    {
        if (remainingToRemove <= 0)
            break;

        Factory* factory = dynamic_cast<Factory*>(building);
        if (factory == nullptr)
            continue;

        CWorkerHaver* workerComponent = factory->getComponent<CWorkerHaver>();
        if (workerComponent == nullptr)
            continue;

        while (remainingToRemove > 0 && workerComponent->getWorkerCount() > 0)
        {
            workerComponent->removeWorker();
            remainingToRemove--;
        }
    }

    for (Building* building : buildings)
    {
        if (remainingToRemove <= 0)
            break;

        Generator* generator = dynamic_cast<Generator*>(building);
        if (generator == nullptr)
            continue;

        CWorkerHaver* workerComponent = generator->getComponent<CWorkerHaver>();
        if (workerComponent == nullptr)
            continue;

        while (remainingToRemove > 0 && workerComponent->getWorkerCount() > 0)
        {
            workerComponent->removeWorker();
            remainingToRemove--;
        }
    }
}

void GameManager::disableBuilding(int index)
{
    if (index < 0 || index >= static_cast<int>(buildings.size()))
        return;

    Building* building = buildings[index];
    if (building == nullptr || !building->getCanBeDeactivated())
        return;

    building->deactivate();
}

void GameManager::activateBuilding(int index)
{
    if (index < 0 || index >= static_cast<int>(buildings.size()))
        return;

    Building* building = buildings[index];
    if (building == nullptr || !building->getCanBeDeactivated())
        return;

    building->activate();
}

void GameManager::updateWorkerAssignment()
{
    int population = getResource(ResourceType::Population);
    int totalWorkers = 0;

    struct WorkerTarget 
    {
        CWorkerHaver* component;
        bool isFactory;
    };

    std::vector<WorkerTarget> workerTargets;

    for (Building* building : buildings)
    {
        CWorkerHaver* workerComponent = building->getComponent<CWorkerHaver>();
        if (workerComponent == nullptr)
            continue;

        bool isFactory = dynamic_cast<Factory*>(building) != nullptr;
        workerTargets.push_back({workerComponent, isFactory});
        totalWorkers += workerComponent->getWorkerCount();
    }

    int availableWorkers = population - totalWorkers;

    if (availableWorkers < 0)
    {
        removeWorkersFromPopulationLoss(-availableWorkers);

        workerTargets.clear();
        totalWorkers = 0;
        for (Building* building : buildings)
        {
            CWorkerHaver* workerComponent = building->getComponent<CWorkerHaver>();
            if (workerComponent == nullptr)
                continue;

            bool isFactory = dynamic_cast<Factory*>(building) != nullptr;
            workerTargets.push_back({workerComponent, isFactory});
            totalWorkers += workerComponent->getWorkerCount();
        }

        availableWorkers = population - totalWorkers;
    }

    if (availableWorkers <= 0)
        return;

    for (auto& target : workerTargets)
    {
        if (!target.isFactory || availableWorkers <= 0)
            continue;

        while (availableWorkers > 0 && target.component->getWorkerCount() < target.component->getMaxWorkers())
        {
            if (!target.component->addWorker())
                break;
            availableWorkers--;
        }
    }

    for (auto& target : workerTargets)
    {
        if (target.isFactory || availableWorkers <= 0)
            continue;

        while (availableWorkers > 0 && target.component->getWorkerCount() < target.component->getMaxWorkers())
        {
            if (!target.component->addWorker())
                break;
            availableWorkers--;
        }
    }
}

void GameManager::endTurn() 
{
    for (Building* building : buildings) 
    {
        building->onTurnStart();
    }

    updateWorkerAssignment();

    int totalProduction = 0;
    int totalConsumption = 0;

    for (Building* building : buildings) 
    {
        if (Generator* gen = dynamic_cast<Generator*>(building)) {
            totalProduction += gen->getEnergyProduction();
        }
        
        CEnergyConsumer* energyComp = building->getComponent<CEnergyConsumer>();
        if (energyComp != nullptr && building->isActive()) {
            totalConsumption += energyComp->getEnergyConsumption();
        }
    }

    bool energyShortage = totalProduction < totalConsumption;
    for (Building* building : buildings) 
    {
        CEnergyConsumer* energyComp = building->getComponent<CEnergyConsumer>();
        if (energyComp != nullptr && energyComp->getEnergyConsumption() > 0)
            building->setEnergyAvailable(!energyShortage);
        else
            building->setEnergyAvailable(true);
    }

    resources[ResourceType::Energy] = totalProduction;

    for (Building* building : buildings)
    {
        building->onTurnEnd();
    }

    int pollution = getResource(ResourceType::Pollution);
    int happinessModifier = -pollution / 10;
    modifyResource(ResourceType::Happiness, happinessModifier);

    int happiness = getResource(ResourceType::Happiness);
    if (happiness < 0) 
    {
        int populationLoss = -happiness; 
        int populationBefore = getResource(ResourceType::Population);
        
        for (Building* building : buildings) 
        {
            if (House* house = dynamic_cast<House*>(building)) 
            {
                CPeopleHaver* peopleComp = house->getComponent<CPeopleHaver>();
                if (peopleComp == nullptr)
                    continue;
                
                for (int i = 0; i < populationLoss && peopleComp->getPeople() > 0; ++i) 
                {
                    peopleComp->decreasePeople();
                }
            }
        }

        int populationAfter = getResource(ResourceType::Population);
        int actualPopulationLoss = populationBefore - populationAfter;
        
        if (actualPopulationLoss > 0)
        {
            removeWorkersFromPopulationLoss(actualPopulationLoss);
        }
    }

    currentTurn++;
}