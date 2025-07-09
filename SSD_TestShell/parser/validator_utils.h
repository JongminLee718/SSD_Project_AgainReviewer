#include <string>
#include <vector>

class ValidatorUtils {
public:
	ValidatorUtils() = delete;

	static void checkTokenCount(const std::vector<std::string>& tokens, size_t expected);
	static void checkInteger(const std::string& address);
	static void checkHexValue(const std::string& dataValue);
};