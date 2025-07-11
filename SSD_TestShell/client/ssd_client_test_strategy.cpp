#include "script_strategy_interface.h"

bool SsdClientTestStrategy::execute(const std::string& cmd) {
    SsdClientApp app(ssdInterface, utilsInterface);
    app.setInputCmd(cmd);
    return app.startVerify();
}