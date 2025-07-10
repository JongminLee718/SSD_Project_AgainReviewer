#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

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
	void makeBufferFiles();
	void loadBufferData();
	void parsingCommandBuffer(const std::vector<std::string> parts);
	void updateBufferFile();
	void renameBuffers(std::string& desired_filename_str, std::vector<std::filesystem::path>& current_paths, int i);
	void generateFileFormatName(int i, std::string& desired_filename_str);
	void addCommandInBuffer(const std::string& command_type, int address, int valueOrSize);
	void updateBufferWithNewCommands(const std::vector<Command>& commands_to_add);
	bool readFromBuffer(int address, unsigned int& outValue);
	void flush();

private:
	const std::string bufferDir = "buffer";
	std::vector<Command> commands;
	std::string nandPath;
};
