#include <environment.h>

#include <stdlib.h>
#include <string.h>
#include <uthash.h>

typedef struct {
	char *name;
	char *value;
	UT_hash_handle hh;
} EnvironmentVariable;

EnvironmentVariable *environment = NULL;

const char *get_environment_variable(const char *name) {
	EnvironmentVariable *variable;
	HASH_FIND_STR(environment, name, variable);
	if (variable != NULL) {
		return variable->value;
	}
	return NULL;
}

bool set_environment_variable(const char *name, const char *value) {
	EnvironmentVariable *variable;
	HASH_FIND_STR(environment, name, variable);
	if (variable == NULL) {
		variable = malloc(sizeof(*variable));
		if (variable == NULL) {
			return false;
		}

		variable->name = strdup(name);
		if (variable->name == NULL) {
			free(variable);
			return false;
		}

		variable->value = strdup(value);
		if (variable->name == NULL) {
			free(variable->name);
			free(variable);
			return false;
		}

		HASH_ADD_KEYPTR(hh, environment, variable->name, strlen(variable->name), variable);
	} else {
		char *value_copy = strdup(value);
		if (value_copy == NULL) {
			return false;
		}

		free(variable->value);
		variable->value = value_copy;
	}

	return true;
}
