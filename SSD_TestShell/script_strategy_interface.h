#pragma once
#include <string>
#include "ssd_client_app.h"

#define interface class

interface ScriptStrategyInterface {
public:
    virtual ~ScriptStrategyInterface() = default;
    virtual bool execute(const std::string& cmd) = 0;
};

class SsdClientTestStrategy : public ScriptStrategyInterface {
public:
    SsdClientTestStrategy(SsdInterface* ssd, UtilsInterface* utils)
        : ssdInterface(ssd), utilsInterface(utils) {
    }

    bool execute(const std::string& cmd) override;

private:
    SsdInterface* ssdInterface;
    UtilsInterface* utilsInterface;
};