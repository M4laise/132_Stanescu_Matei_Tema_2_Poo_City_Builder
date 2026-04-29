#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <map>

class Building;

enum class ResourceType { Money, Energy, Pollution, Population, Happiness, Workers};

class GameManager
{
private:
    std::vector<Building*> buildings;
    std::map<ResourceType, int> resources;

    int moneyNeededToWin;
    int currentTurn;
    int maxTurns;

    GameManager();

    void addBuilding(Building* building);

public:
    void updateWorkerAssignment();
    GameManager(const GameManager&) = delete;
    void operator=(const GameManager&) = delete;

    static GameManager& getInstance()
    {
        static GameManager instance;
        return instance;
    }

    bool victoryCheck()
    {
        return (resources[ResourceType::Money] >= moneyNeededToWin);
    }

    void initGameManager(int startMoney, int maxTurns, int moneyNeededToWin);

    const std::vector<Building*>& getBuildings() const;
    int getCurrentTurn() const;
    int getMaxTurns() const;
    int getMoneyNeededToWin() const;

    void buyBuilding(Building* building);
    void disableBuilding(int index);
    void activateBuilding(int index);

    void modifyResource(ResourceType resource,int amount);
    int getResource(ResourceType resource) const;

    void removeWorkersFromPopulationLoss(int count);

    void endTurn();
};

#endif