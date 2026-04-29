#ifndef GENERATOR_H
#define GENERATOR_H

#include "building.h"

class CWorkerHaver;

class Generator : public Building
{
private:
    static inline const std::string name = "Generator";
    static const int COST = 75;
    CWorkerHaver* workerComponent;

public:
    Generator();
    virtual ~Generator();

    Building* clone() const override;
    std::string showInfo() const override;

    int getEnergyProduction() const;
};

#endif