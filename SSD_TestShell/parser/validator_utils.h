#include <string>
#include <vector>

class ValidatorUtils {
public:
	ValidatorUtils() = delete;

	static void checkTokenCount(const std::vector<std::string>& tokens, size_t expected);
	static void validateAddress(const std::string& address);
	static void validateDataValue(const std::string& dataValue);
private:
	static const long long MIN_VALID_DATA_VALUE = 0;
	static const long long MAX_VALID_DATA_VALUE = 0xFFFFFFFF;
};