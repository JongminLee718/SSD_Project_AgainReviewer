#include <chrono>
#include "log_formatter.h"

using std::string;

string LogFormatter::createLogLine(const string& logMessage, const string& callerInfo)
{
    string parsedMethod = parseMethodName(callerInfo);
    string paddedMethod = padMethodName(parsedMethod, 50);
    return "[" + getCurrentTimestamp() + "] " +
        paddedMethod + " : " + logMessage + "\n";
}

string LogFormatter::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &now);

    char buf[20];
    std::strftime(buf, sizeof(buf), "%y.%m.%d %H:%M", &tm);
    return string(buf);
}

string LogFormatter::parseMethodName(const std::string& sig) {
#if defined(_MSC_VER)
    size_t methodStart = sig.find("__cdecl ");
    if (methodStart == std::string::npos) return sig;

    methodStart += strlen("__cdecl ");
    size_t parenPos = sig.find('(', methodStart);
    if (parenPos == std::string::npos) return sig;

    std::string fullName = sig.substr(methodStart, parenPos - methodStart);
    size_t delim = fullName.rfind("::");
    if (delim == std::string::npos) return fullName + "()";

    std::string className = fullName.substr(0, delim);
    std::string methodName = fullName.substr(delim + 2);
    return className + "." + methodName + "()";
#elif defined(__GNUC__) || defined(__clang__)
    std::regex pattern(R"(\b(\w+)::(\w+)\s*\(\))");
    std::smatch match;
    if (std::regex_search(sig, match, pattern)) {
        return match[1].str() + "." + match[2].str() + "()";
    }
    return sig;

#else
    return sig;
#endif
}

string LogFormatter::padMethodName(const string& methodName, size_t width) {
    if (methodName.length() >= width) {
        return methodName;
    }
    return methodName + string(width - methodName.length(), ' ');
}
