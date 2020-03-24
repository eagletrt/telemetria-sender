#include "log_utils.h"

void logInfo(const char* message) {
    printf("%s[INFO]%s\t\t%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET, message);
}

void logSuccess(const char* message) {
    printf("%s[SUCCESS]%s\t%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, message);
}

void logDebug(const char* message) {
    printf("%s[DEBUG]%s\t\t%s\n", ANSI_COLOR_DARK_GRAY, ANSI_COLOR_RESET, message);
}

void logError(const char* message) {
    printf("%s[ERROR]%s\t\t%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, message);
}

void logWarning(const char* message) {
    printf("%s[WARNING]%s\t%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, message);
}