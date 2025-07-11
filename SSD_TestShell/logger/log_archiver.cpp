#include <chrono>
#include <filesystem>
#include "log_archiver.h"

using std::string;

void LogArchiver::archiveLogFile() {
    string src = logDir + logFile;
    string dst = generateLogFilename();

    if (!std::filesystem::exists(src)) return;
    if (std::filesystem::file_size(src) < 10 * 1024) return;

    saveAsZipFile();

    std::filesystem::copy_file(src, dst,
        std::filesystem::copy_options::overwrite_existing);
    std::filesystem::remove(src);
}

string LogArchiver::generateLogFilename() {
    std::time_t now = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &now);

    char buffer[64];
    std::snprintf(
        buffer, sizeof(buffer),
        "until_%02d%02d%02d_%02dh_%02dm_%02ds",
        (tm.tm_year % 100),
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
    );

    return logDir + std::string(buffer) + ".log";
}

void LogArchiver::saveAsZipFile() {
    for (const auto& entry : std::filesystem::directory_iterator(logDir)) {
        if (!entry.is_regular_file()) continue;

        const auto& path = entry.path();
        if (path.extension() == ".log" && path.filename() != logFile) {
            std::filesystem::path zipPath = path;
            zipPath.replace_extension(".zip");

            std::filesystem::copy_file(path, zipPath,
                std::filesystem::copy_options::overwrite_existing);
            std::filesystem::remove(path);
        }
    }
}
