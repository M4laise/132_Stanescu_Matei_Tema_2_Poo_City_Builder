#ifndef COMPONENT_H
#define COMPONENT_H

class Component 
{
protected:
    Component() = default;

public:
    virtual ~Component() = default;

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
};

#endif