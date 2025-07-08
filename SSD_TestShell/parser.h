#include <string>
#include <vector>

class Parser {
public:
	Parser() = default;
	~Parser() = default;

	std::vector<std::string> parse(const std::string& input);

private:
	std::vector<std::string> split(const std::string& input);
	void checkTokenCount(std::vector<std::string>& result, int expectedCounts);
	void validateAddress(const std::string& address);
	void validateDataValue(const std::string& dataValue);
};