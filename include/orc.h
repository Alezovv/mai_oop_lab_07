#pragma once

#include "npc.h"

class Orc : public NPC
{
public:
    Orc(std::string name, double x, double y) : NPC("Orc", std::move(name), x, y) {}

    double getMoveDistance() const override { return 20.0; }
    double getKillDistance() const override { return 10.0; }
    bool canKill(const NPC& other) const override { return true; }
};