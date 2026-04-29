#ifndef PARK_H
#define PARK_H

#include "building.h"

class Park : public Building
{
private:
    static inline const std::string name = "Park";
    static const int COST = 75;
    static const int MAINTENANCE_COST = 20;
    static const int HAPPINESS_BOOST = 10;

public:
    Park();
    virtual ~Park();

    Building* clone() const override;
    std::string showInfo() const override;
    void onBuy() override;
    void onTurnEnd() override;
};

#endif