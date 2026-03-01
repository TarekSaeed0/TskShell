#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @struct Arguments
 * @brief A list of command arguments.
 */
typedef struct {
	char **data;
	size_t length;
	size_t capacity;
} Arguments;

/**
 * @brief Creates a new, empty list of arguments.
 *
 * @return The created list of arguments.
 */
Arguments arguments_new(void);

/**
 * @brief Releases resources held by the given list of arguments.
 *
 * @param[inout] arguments Pointer to the list of arguments to drop.
 */
void arguments_drop(Arguments *arguments);

/**
 * @brief Reserves space for additional arguments in the given list of arguments.
 *
 * @param[inout] arguments Pointer to the list of arguments.
 * @param[in] additional The number of additional arguments to reserve.
 * @return true if successful, false otherwise.
 */
bool arguments_reserve(Arguments *arguments, size_t additional);

/**
 * @brief Appends an argument to the given list of arguments.
 *
 * this takes ownership of the argument, so the caller should not free it after.
 *
 * @param[inout] arguments Pointer to the list of arguments.
 * @param[in] argument The argument to be appended.
 * @return true if successful, false otherwise.
 */
bool arguments_append(Arguments *arguments, char *argument);

#endif // ARGUMENTS_H
