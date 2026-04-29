#include "CWorkerHaver.h"
#include "generator.h"

Generator::Generator() : Building(Generator::name, Generator::COST)
{
    workerComponent = new CWorkerHaver(0, 2);
    addComponent(workerComponent);
}

Generator::~Generator() = default;

Building* Generator::clone() const
{
    return new Generator(*this);
}

std::string Generator::showInfo() const
{
    return std::to_string(workerComponent->getWorkerCount()) + "/" + std::to_string(workerComponent->getMaxWorkers());
}

int Generator::getEnergyProduction() const
{
    if (!isActive() || workerComponent->getWorkerCount() < workerComponent->getMaxWorkers())
        return 0;
    return 9;
}
