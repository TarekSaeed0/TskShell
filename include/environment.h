#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdbool.h>

/**
 * @brief Gets the value of the environment variable with the given name.
 *
 * @param[in] name The name of the environment variable.
 * @return[in] The value of the environment variable if it exists, NULL otherwise.
 */
const char *get_environment_variable(const char *name);

/**
 * @brief Sets the value of the environment variable with the given name to the given value.
 *
 * @param[in] name The name of the environment variable.
 * @param[in] value The value to set the environment variable to.
 * @return true if successful, false otherwise.
 */
bool set_environment_variable(const char *name, const char *value);

#endif
