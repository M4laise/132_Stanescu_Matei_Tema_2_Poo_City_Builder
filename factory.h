#ifndef FACTORY_H
#define FACTORY_H

#include "building.h"

class CEnergyConsumer;
class CWorkerHaver;

class Factory : public Building
{
private:
    static inline const std::string name = "Factory";
    static const int COST = 100;
    CEnergyConsumer* energyComponent;
    CWorkerHaver* workerComponent;
    int productionPerWorker;
    int consecutiveEnergyFailures;
    int activeTurns;
    int accumulatedPollution;

public:
    Factory();
    virtual ~Factory();

    Building* clone() const override;
    std::string showInfo() const override;

    void onDeactivate() override;
    void onActivate() override;
    void onTurnEnd() override;
};

#endif