#pragma once

#include <string>
#include <memory>

#include "npc.h"

class NPCFactory
{
public:
    static std::unique_ptr<NPC> CreateNPC(const std::string &type, const std::string &name, double x, double y);
};