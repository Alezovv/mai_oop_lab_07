#include "fight_visitor.h"
#include "utils.h"
#include <iostream>
#include <mutex>

FightVisitor::FightVisitor(std::vector<std::unique_ptr<NPC>> &npcs, std::mutex& mtx)
    : npcs_(npcs), mtx_(mtx) {}

void FightVisitor::fight(NPC& attacker, NPC& defender)
{
    if (!attacker.isAlive() || !defender.isAlive()) return;

    int attack_roll = attacker.rollDice();
    int defense_roll = defender.rollDice();

    std::lock_guard<std::mutex> lock(mtx_);
    std::cout << attacker.getName() << " attacks " << defender.getName()
              << " (attack: " << attack_roll << ", defense: " << defense_roll << ")" << std::endl;

    if (attack_roll > defense_roll) {
        defender.kill();
        std::cout << defender.getName() << " is killed by " << attacker.getName() << std::endl;
    }
}