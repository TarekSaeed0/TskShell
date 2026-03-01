#ifndef ARGUMENTS_BUILDER_H
#define ARGUMENTS_BUILDER_H

#include <argument.h>
#include <arguments.h>

/**
 * @struct ArgumentsBuilder
 * @brief A builder for building a list of arguments incrementally.
 */
typedef struct {
	Arguments arguments;
	Argument argument;
	bool in_argument;
} ArgumentsBuilder;

/**
 * @brief Creates a new, empty arguments builder.
 *
 * @return The created arguments builder.
 */
ArgumentsBuilder arguments_builder_new(void);

/**
 * @brief Releases resources held by the given arguments builder.
 *
 * @param[inout] builder Pointer to the arguments builder to drop.
 */
void arguments_builder_drop(ArgumentsBuilder *builder);

/**
 * @brief Appends a character to the current argument in the arguments  builder.
 *
 * if the arguments builder wasn't in an argument already, this starts a new argument.
 *
 * @param[inout] builder Pointer to the arguments builder.
 * @param[in] character The character to be appended
 * @return true if successful, false otherwise.
 */
bool arguments_builder_append_character(ArgumentsBuilder *builder, char character);

/**
 * @brief Appends a string to the current argument in the arguments builder.
 *
 * if the arguments builder wasn't in an argument already, this starts a new argument.
 *
 * @param[inout] builder Pointer to the arguments builder.
 * @param[in] character The string to be appended
 * @return true if successful, false otherwise.
 */
bool arguments_builder_append_string(ArgumentsBuilder *builder, const char *string);

/**
 * @brief Ensures that the arguments builder is in an argument.
 *
 * if the arguments builder wasn't in an argument already, this starts a new argument.
 * this is useful when the caller wants an empty argument, which can't be created by appending characters or strings.
 *
 * @param[inout] builder Pointer to the arguments builder.
 */
void arguments_builder_ensure_in_argument(ArgumentsBuilder *builder);

/**
 * @brief Ends the current argument in the arguments builder.
 *
 * if the arguments builder wasn't in an argument already, this does nothing.
 * after this, the arguments builder won't be in an argument anymore.
 *
 * @param[inout] builder Pointer to the arguments builder.
 * @return true if successful, false otherwise.
 */
bool arguments_builder_end_argument(ArgumentsBuilder *builder);

/**
 * @brief Builds the final list of arguments from the arguments builder.
 *
 * if arguments builder is still in an argument, this discards it.
 * after this, the arguments builder is reset to an empty state, and doesn't need to be dropped.
 * the caller is responsible for dropping the returned Arguments struct when it's no longer needed.
 *
 * @param[inout] builder Pointer to the arguments builder.
 * @return The built list of arguments.
 */
Arguments arguments_builder_build(ArgumentsBuilder *builder);

#endif // ARGUMENTS_BUILDER_H
