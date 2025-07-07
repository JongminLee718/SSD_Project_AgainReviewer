#ifdef _DEBUG
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    std::cout << "Debug\n";
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#include <iostream>
int main(int argc, char** argv) {
    std::cout << "Release\n";
    return 0;
}
#endif
