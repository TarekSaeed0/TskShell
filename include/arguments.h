/**
 * @file arguments.h
 * @brief Defines the data structure for representing a list of shell command argument.
 * @author Tarek Saeed
 * @date 2026-03-02
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @struct Arguments
 * @brief A list of command arguments.
 */
typedef struct {
	char **data;     /**< The list of arguments. */
	size_t length;   /**< The number of arguments in the list excluding the null terminator. */
	size_t capacity; /**< The allocated capacity of the list excluding the null terminator. */
} Arguments;

/**
 * @brief Creates a new, empty list of arguments.
 *
 * @return The created list of arguments.
 *
 * @memberof Arguments
 */
Arguments arguments_new(void);

/**
 * @brief Releases resources held by the given list of arguments.
 *
 * @param[inout] arguments Pointer to the list of arguments to drop.
 *
 * @memberof Arguments
 */
void arguments_drop(Arguments *arguments);

/**
 * @brief Reserves space for additional arguments in the given list of arguments.
 *
 * @param[inout] arguments Pointer to the list of arguments.
 * @param[in] additional The number of additional arguments to reserve.
 * @return true if successful, false otherwise.
 *
 * @memberof Arguments
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
 *
 * @memberof Arguments
 */
bool arguments_append(Arguments *arguments, char *argument);

#endif // ARGUMENTS_H
