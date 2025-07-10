#include "gmock/gmock.h"
#include "main.h"
#include "ssd.h"
#include "ssd_runner.h"
#include "fileio.h"
#include "buffer_manager.h"

using std::string;

#if defined(_DEBUG)
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}

#else

int main(int argc, char* argv[]) {
	SsdRunner ssdRunner;
	std::string command = argv[1];

	if (argc < 2) return ssdRunner.writeOutput(ERROR);
	
	if (command == "R") return ssdRunner.readHandler(argv, argc);
	if (command == "W") return ssdRunner.writeHandler(argv, argc);
	if (command == "E") return ssdRunner.eraseHandler(argv, argc);
	if (command == "F") return ssdRunner.flushHandler(argc);
	return ssdRunner.writeOutput(ERROR);
}
#endif