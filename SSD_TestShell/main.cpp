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
    SsdClientApp app;

    while (true) {
        
        app.getUserCmdLine();
        app.startVerify(&ssdHandler);
    }

    return 0;
}
#endif
