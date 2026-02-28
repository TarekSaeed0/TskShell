#include <environment.h>

#include <stdlib.h>

char *get_environment_variable(const char *name) {
	return getenv(name);
}

bool set_environment_variable(const char *name, const char *value) {
	return setenv(name, value, 1) == 0;
}
