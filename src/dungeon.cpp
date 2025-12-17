#include "dungeon.h"
#include "npc_factory.h"
#include "utils.h"
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>

Dungeon::Dungeon(int width, int height)
    : width_(width), height_(height), fight_visitor_(npcs_, cout_mtx_)
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis_x(0, width_);
    std::uniform_real_distribution<> dis_y(0, height_);
    std::uniform_int_distribution<> dis_type(0, 2);

    for (int i = 0; i < 50; ++i) {
        double x = dis_x(gen);
        double y = dis_y(gen);
        std::string name = "NPC" + std::to_string(i);
        std::string type;
        int t = dis_type(gen);
        if (t == 0) type = "Druid";
        else if (t == 1) type = "Orc";
        else type = "Slaver";

        npcs_.push_back(NPCFactory::CreateNPC(type, name, x, y)); // 50 NPC
    }
}

Dungeon::~Dungeon()
{
    stopSimulation();
}

void Dungeon::startSimulation()
{
    running_ = true;
    movement_thread_ = std::thread(&Dungeon::movementLoop, this); // Поток передвижения
    fight_thread_ = std::thread(&Dungeon::fightLoop, this); // Поток боёв
    print_thread_ = std::thread(&Dungeon::printLoop, this); // Поток вывода карты
}

void Dungeon::stopSimulation()
{
    running_ = false;
    fight_cv_.notify_all();
    if (movement_thread_.joinable()) movement_thread_.join();
    if (fight_thread_.joinable()) fight_thread_.join();
    if (print_thread_.joinable()) print_thread_.join();

    
    std::lock_guard<std::mutex> lock(cout_mtx_);
    std::cout << "Survivors:" << std::endl;
    for (const auto& npc : npcs_) {
        if (npc->isAlive()) {
            std::cout << npc->getType() << " " << npc->getName() << std::endl;
        }
    }
}

void Dungeon::movementLoop()
{
    while (running_) {
        {
            std::unique_lock<std::shared_mutex> lock(mtx_);
            for (auto& npc : npcs_) {
                if (npc->isAlive()) {
                    moveNPC(*npc); // Мертвые NPC не передвигаются
                }
            }
        }
        checkForFights();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Dungeon::fightLoop()
{
    while (running_) {
        std::unique_lock<std::mutex> lock(fight_mtx_);
        fight_cv_.wait(lock, [this]() { return !fight_queue_.empty() || !running_; });
        if (!running_) break;

        FightTask task = std::move(fight_queue_.front());
        fight_queue_.pop();
        lock.unlock();

        if (task.attacker && task.defender && task.attacker->canKill(*task.defender)) {
            fight_visitor_.fight(*task.attacker, *task.defender);
        }
    }
}

void Dungeon::printLoop()
{
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printMap();
    }
}

void Dungeon::moveNPC(NPC& npc)
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis(-npc.getMoveDistance(), npc.getMoveDistance());

    double dx = dis(gen);
    double dy = dis(gen);

    // Живые NPC не могут покинуть карту
    double new_x = std::clamp(npc.getX() + dx, 0.0, static_cast<double>(width_));
    double new_y = std::clamp(npc.getY() + dy, 0.0, static_cast<double>(height_));

    npc.setPosition(new_x, new_y);
}

void Dungeon::checkForFights()
{
    std::shared_lock<std::shared_mutex> lock(mtx_); // контроль доступа с помощью std::shared_lock
    for (size_t i = 0; i < npcs_.size(); ++i) {
        for (size_t j = i + 1; j < npcs_.size(); ++j) {
            auto& npc1 = npcs_[i];
            auto& npc2 = npcs_[j];
            if (npc1->isAlive() && npc2->isAlive()) {
                double dist = Distance(npc1->getX(), npc1->getY(), npc2->getX(), npc2->getY());
                if (dist <= npc1->getKillDistance() && npc1->canKill(*npc2)) {
                    std::lock_guard<std::mutex> qlock(fight_mtx_); // контроль доступа с помощью std::lock_guard
                    fight_queue_.push({npcs_[i].get(), npcs_[j].get()});
                    fight_cv_.notify_one();
                }
                if (dist <= npc2->getKillDistance() && npc2->canKill(*npc1)) {
                    std::lock_guard<std::mutex> qlock(fight_mtx_);
                    fight_queue_.push({npcs_[j].get(), npcs_[i].get()});
                    fight_cv_.notify_one();
                }
            }
        }
    }
}

void Dungeon::printMap() const
{
    std::shared_lock<std::shared_mutex> lock(mtx_);
    std::lock_guard<std::mutex> cout_lock(cout_mtx_); // Контроль std::cout
    std::cout << "=== Current Map ===" << std::endl;
    for (const auto& npc : npcs_) {
        if (npc->isAlive()) {
            // Мертвые NPC не отображаются
            std::cout << npc->getType() << " " << npc->getName() << " at ("
                      << std::fixed << std::setprecision(1) << npc->getX() << ", "
                      << std::fixed << std::setprecision(1) << npc->getY() << ")" << std::endl;
        }
    }
    std::cout << std::endl;
}