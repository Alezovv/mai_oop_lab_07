#include "slaver.h"
#include "druid.h"

bool Slaver::canKill(const NPC& other) const
{
    return dynamic_cast<const Druid*>(&other) != nullptr;
}