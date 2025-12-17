#pragma once

#include "npc.h"

class Slaver : public NPC
{
public:
    Slaver(std::string name, double x, double y) : NPC("Slaver", std::move(name), x, y) {}

    double getMoveDistance() const override { return 30.0; }
    double getKillDistance() const override { return 10.0; }
    bool canKill(const NPC& other) const override;
};