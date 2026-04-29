#include "CEnergyConsumer.h"

CEnergyConsumer::CEnergyConsumer(int energyConsumption) : energyConsumption(energyConsumption)
{
    if (energyConsumption < 0)
        this->energyConsumption = 0;
}

CEnergyConsumer::~CEnergyConsumer() = default;

int CEnergyConsumer::getEnergyConsumption() const
{
    return energyConsumption;
}

void CEnergyConsumer::setEnergyConsumption(int energyConsumption)
{
    this->energyConsumption = energyConsumption;
    if (energyConsumption < 0)
        energyConsumption = 0;
}
