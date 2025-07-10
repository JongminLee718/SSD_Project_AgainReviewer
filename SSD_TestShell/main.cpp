#ifdef _DEBUG
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#include "ssd_client_app.h"
#include "runner.h"
#include "script_strategy_interface.h"
#include <iostream>

int main(int argc, char* argv[]) {
    SsdHandler ssdHandler;
    Utils utils;
    SsdClientApp app(&ssdHandler, &utils);
    auto ssdClientStrategy = std::make_unique<SsdClientTestStrategy>(&ssdHandler, &utils);
    Runner runner(std::move(ssdClientStrategy));

    if (argc == 1) {
        while (true) {
            try {
                app.getUserCmdLine();
                app.startVerify();
            }
            catch (const std::invalid_argument& e) {
                std::cout << e.what() << '\n';
                continue;
            }
            catch (...) {
                app.printError();
                continue;
            }
        }
    }
    else {
        runner.runScriptFile(argv[1]);
    }

    return 0;
}
#endif
