#include "CWorkerHaver.h"
#include "game_manager.h"

CWorkerHaver::CWorkerHaver(int startingWorkers, int maxWorkers) : game(GameManager::getInstance()), workers(startingWorkers), maxWorkers(maxWorkers)
{
    if (workers < 0)
        workers = 0;

    if (maxWorkers < 0)
        this->maxWorkers = 0;
    
    if (workers > this->maxWorkers)
        workers = this->maxWorkers;

    if (workers > 0)
        game.modifyResource(ResourceType::Workers, workers);
}

CWorkerHaver::~CWorkerHaver()
{
    if (workers > 0)
        game.modifyResource(ResourceType::Workers, -workers);
}

int CWorkerHaver::getWorkerCount() const
{
    return workers;
}

int CWorkerHaver::getMaxWorkers() const
{
    return maxWorkers;
}

bool CWorkerHaver::addWorker()
{
    if (workers >= maxWorkers)
        return false;

    workers++;
    game.modifyResource(ResourceType::Workers, 1);
    return true;
}

bool CWorkerHaver::removeWorker()
{
    if (workers <= 0)
        return false;
    
    workers--;
    game.modifyResource(ResourceType::Workers, -1);
    return true;
}
