#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <config.h>
#include <bitset>

namespace Copium
{
    struct Entity
    {
        std::string name;
    private:
        friend class EntityComponentSystem;
        std::bitset<MAX_COMPONENTS> componentsBitset{};
    };
};

#endif // !ENTITY_H