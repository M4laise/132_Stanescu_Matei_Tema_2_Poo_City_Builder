#ifndef CWORKERHAVER_H
#define CWORKERHAVER_H

#include "component.h"

class GameManager;

class CWorkerHaver : public Component
{
private:
    int workers;
    int maxWorkers;
    GameManager& game;

public:
    CWorkerHaver(int startingWorkers, int maxWorkers);
    virtual ~CWorkerHaver();

    int getWorkerCount() const;
    int getMaxWorkers() const;

    bool addWorker();
    bool removeWorker();
};

#endif