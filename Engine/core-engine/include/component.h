#pragma once

#ifndef HEADER_COMPONENT

#define HEADER_COMPONENT
//INCLUDES
#include <glm/vec2.hpp> //Vec2

//USING
using ComponentID = unsigned char;


class Component
{
private:

    ComponentID id;
public:
    virtual void destroy();
    ComponentID const ID();
protected:
    Component();
};

class Collider : public Component
{
public:
    Collider();
    virtual void collide();
};

#endif // !HEADER_COMPONENT
