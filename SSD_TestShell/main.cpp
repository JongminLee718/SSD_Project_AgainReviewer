#ifdef _DEBUG
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    std::cout << "Debug\n";
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#include "ssd_verify_app.h"
#include <iostream>
int main() {
    std::cout << "Release\n";

    SsdVerifyApp app;
    app.getUserCmdLine();
    app.startVerify();

    return 0;
}
#endif
