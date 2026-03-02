/**
 * @file argument.h
 * @brief Defines the data structure for representing a shell command argument.
 * @author Tarek Saeed
 * @date 2026-03-02
 */

#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @struct Argument
 * @brief A command argument.
 */
typedef struct {
	char *data;      /**< The data of the argument. */
	size_t length;   /**< The length of the argument excluding the null terminator. */
	size_t capacity; /**< The allocated capacity of the argument excluding the null terminator. */
} Argument;

/**
 * @brief Creates a new, empty argument.
 *
 * @return The created argument.
 *
 * @memberof Argument
 */
Argument argument_new(void);

/**
 * @brief Releases resources held by the given argument.
 *
 * @param[inout] argument Pointer to the argument to drop.
 *
 * @memberof Argument
 */
void argument_drop(Argument *argument);

/**
 * @brief Reserves space for additional characters in the given argument.
 *
 * @param[inout] argument Pointer to the argument.
 * @param[in] additional The number of additional characters to reserve.
 * @return true if successful, false otherwise.
 *
 * @memberof Argument
 */
bool argument_reserve(Argument *argument, size_t additional);

/**
 * @brief Appends a character to the given argument.
 *
 * @param[inout] argument Pointer to the argument.
 * @param[in] character The character to be appended.
 * @return true if successful, false otherwise.
 *
 * @memberof Argument
 */
bool argument_append_character(Argument *argument, char character);

/**
 * @brief Appends a string to the given argument.
 *
 * @param[inout] argument Pointer to the argument.
 * @param[in] string The string to be appended.
 * @return true if successful, false otherwise.
 *
 * @memberof Argument
 */
bool argument_append_string(Argument *argument, const char *string);

/**
 * @brief Takes ownership of the data of the given argument.
 *
 * after this, the argument is reset to an empty state, and doesn't need to be dropped.
 * the caller is responsible for dropping the returned data when it's no longer needed.
 *
 * @param[inout] argument Pointer to the argument.
 * @return Pointer to the data, or NULL if the argument is empty.
 *
 * @memberof Argument
 */
char *argument_take_data(Argument *argument);

#endif // ARGUMENT_H
