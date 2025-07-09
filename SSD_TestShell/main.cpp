#ifdef _DEBUG
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    std::cout << "Debug\n";
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#include "ssd_client_app.h"
#include <iostream>
int main() {
    std::cout << "Release\n";

    SsdHandler ssdHandler;
    OutputChecker checker;
    SsdClientApp app;

    while (true) {
        try {
            app.getUserCmdLine();
            app.startVerify(&ssdHandler, &checker);
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
