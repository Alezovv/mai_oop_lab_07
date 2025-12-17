#include <iostream>
#include <fstream>
#include <string>

#include "observer.h"

void ConsoleObserver::onKill(const std::string &killer, const std::string &victim)
{
    std::cout << "[Battle Event] " << killer << " killed " << victim << std::endl;
}

void FileObserver::onKill(const std::string &killer, const std::string &victim)
{
    std::ofstream log("log.txt", std::ios::app);
    if (log.is_open())
    {
        log << killer << " killed " << victim << std::endl;
    }
}