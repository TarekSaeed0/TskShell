#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdbool.h>

char *get_environment_variable(const char *name);
bool set_environment_variable(const char *name, const char *value);

#endif
