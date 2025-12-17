#include "dungeon.h"
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    Dungeon dungeon;

    dungeon.startSimulation();

    std::this_thread::sleep_for(std::chrono::seconds(30)); // Запуск на 30 секунд

    dungeon.stopSimulation();

    return 0;
}