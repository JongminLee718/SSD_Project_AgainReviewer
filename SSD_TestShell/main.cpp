#ifdef _DEBUG
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#include "ssd_client_app.h"
#include <iostream>

int main() {
    SsdHandler ssdHandler;
    Utils utils;
    SsdClientApp app;

    while (true) {
        try {
            app.getUserCmdLine();
            app.startVerify(&ssdHandler, &utils);
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

    return 0;
}
#endif
