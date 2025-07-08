#include <string>
#include <vector>

class Parser {
public:
	Parser() = default;
	~Parser() = default;

	std::vector<std::string> parse(const std::string& input);

private:
	std::vector<std::string> split(const std::string& input);
};