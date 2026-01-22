#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

int main(void) {
	// Default level is LOG_INFO
	LOG_INFO_MSG("This is an info message: %d", 42);
	LOG_DEBUG_MSG("This debug message will NOT be shown by default");

	// Change level to LOG_DEBUG
	set_log_level(LOG_DEBUG);
	LOG_DEBUG_MSG("Now debug messages are shown: %s", "hello");

	// Warnings and Errors
	LOG_WARN_MSG("This is a warning!");
	LOG_ERROR_MSG("This is an error!");

	// Environment variable override test
	// You can set LOG_LEVEL=1 (WARN) etc.
	LogLevel env_level = get_log_level_from_env();
	if (env_level != LOG_DEBUG) {
		printf("Environment overrides level to: %d\n", env_level);
	}

	return 0;
}
