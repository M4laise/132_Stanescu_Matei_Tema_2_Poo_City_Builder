#ifndef HOUSE_H
#define HOUSE_H

#include "building.h"

class CPeopleHaver;

class CEnergyConsumer;
class CPeopleHaver;

class House : public Building 
{
private:
    static inline const std::string name = "House";
    static const int COST = 50;
    CPeopleHaver* peopleComponent;
    CEnergyConsumer* energyComponent;
    int consecutiveEnergyFailures;

public:
    House();
    virtual ~House();

    Building* clone() const override;
    std::string showInfo() const override;
    void onTurnEnd() override;
};

#endif