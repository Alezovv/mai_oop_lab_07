#pragma once

#include <string>

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void onKill(const std::string &killer, const std::string &victim) = 0;
};

class ConsoleObserver : public IObserver
{
public:
    void onKill(const std::string &killer, const std::string &victim) override;
};

class FileObserver : public IObserver
{
public:
    void onKill(const std::string &killer, const std::string &victim) override;
};