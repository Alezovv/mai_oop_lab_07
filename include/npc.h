#pragma once

#include <string>
#include <memory>
#include <atomic>
#include <random>

class NPC
{
public:
    NPC(std::string type, std::string name, double x, double y);
    virtual ~NPC() = default;

    std::string getType() const;
    std::string getName() const;
    double getX() const;
    double getY() const;
    void setPosition(double x, double y);

    bool isAlive() const;
    void kill();

    virtual double getMoveDistance() const = 0;
    virtual double getKillDistance() const = 0;
    virtual bool canKill(const NPC& other) const = 0;

    int rollDice(); // 6-гранный кубик

    // Для сохранения
    friend std::ostream &operator<<(std::ostream &os, const NPC &npc);

protected:
    std::string type_;
    std::string name_;
    double x_;
    double y_;
    std::atomic<bool> alive_;
    mutable std::mt19937 gen_{std::random_device{}()};
};