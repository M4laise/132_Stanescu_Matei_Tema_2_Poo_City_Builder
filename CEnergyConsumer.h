#ifndef CENERGYCONSUMER_H
#define CENERGYCONSUMER_H

#include "component.h"

class CEnergyConsumer : public Component
{
private:
    int energyConsumption;

public:
    CEnergyConsumer(int energyConsumption);
    virtual ~CEnergyConsumer();

    int getEnergyConsumption() const;
    void setEnergyConsumption(int energyConsumption);
};

#endif