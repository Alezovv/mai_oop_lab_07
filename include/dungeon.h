#pragma once

#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <queue>
#include <condition_variable>

#include "npc.h"
#include "fight_visitor.h"

struct FightTask {
    NPC* attacker;
    NPC* defender;
};

class Dungeon
{
public:
    Dungeon(int width = 100, int height = 100);
    ~Dungeon();

    void startSimulation();
    void stopSimulation();

    void printMap() const;

private:
    int width_, height_;
    std::vector<std::unique_ptr<NPC>> npcs_;
    mutable std::shared_mutex mtx_;
    mutable std::mutex cout_mtx_;
    std::atomic<bool> running_{false};

    // Threads
    std::thread movement_thread_;
    std::thread fight_thread_;
    std::thread print_thread_;

    // Fight queue
    std::queue<FightTask> fight_queue_;
    std::mutex fight_mtx_;
    std::condition_variable fight_cv_;

    FightVisitor fight_visitor_;

    void movementLoop();
    void fightLoop();
    void printLoop();

    void moveNPC(NPC& npc);
    void checkForFights();
};