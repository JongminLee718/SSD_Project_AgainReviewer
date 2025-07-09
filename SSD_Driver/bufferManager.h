#pragma once
#include <string>
#include <vector>

struct Command {
	std::string type;
	bool isEmpty = true;
	int address = -1;
	unsigned int value = 0;
	int eraseSize = 0;
};

class BufferManager {
public:
	BufferManager(std::string nandPath);

	void initializeBuffer();
	void loadBufferData();
	void updateBufferFile();
	void addCommandInBuffer(const std::string& command_type, int address, int valueOrSize);
	bool readFromBuffer(int address, unsigned int& outValue);
	void flush();

private:
	const std::string bufferDir = "buffer";
	std::vector<Command> commands;
	std::string nandPath;
};
