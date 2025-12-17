#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include "npc.h"

class FightVisitor
{
public:
    FightVisitor(std::vector<std::unique_ptr<NPC>> &npcs, std::mutex& mtx);

    void fight(NPC& attacker, NPC& defender);

private:
    std::vector<std::unique_ptr<NPC>> &npcs_;
    std::mutex& mtx_;
};