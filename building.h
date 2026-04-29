#ifndef BUILDING_H
#define BUILDING_H

class Component;

#include <string>
#include <vector>

class Building
{
private:
    std::string name;
    int cost;
    std::vector<Component*> components;
    bool canBeDeactivated;
    bool active;
    bool energyAvailable;
    
    protected:
    void addComponent(Component* component);
    void setActive(bool active);
    virtual std::string showInfo() const = 0;
    
    public:
    Building(std::string name, int cost, bool canBeDeactivated=false);
    virtual ~Building();
    
    friend std::ostream& operator<<(std::ostream& os, const Building& building);

    virtual Building* clone() const = 0;

    std::string getName() const;
    bool isActive() const;
    bool getCanBeDeactivated() const;
    int getCost() const;
    void deactivate();
    void activate();

    bool isEnergyAvailable() const;
    void setEnergyAvailable(bool available);

    virtual void onBuy();
    virtual void onDeactivate();
    virtual void onActivate();
    virtual void onTurnStart();
    virtual void onTurnEnd();

    template <typename T>
    const T* getComponent() const 
    {
        for (auto component : components) 
        {
            const T* target = dynamic_cast<const T*>(component);
            if (target) return target;
        }
        return nullptr;
    }

    template <typename T>
    T* getComponent()
    {
        for (auto component : components)
        {
            T* target = dynamic_cast<T*>(component);
            if (target) return target;
        }
        return nullptr;
    }
};

#endif