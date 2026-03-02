/**
 * @file builtins.h
 * @brief Defines the built-in commands available in the shell.
 * @author Tarek Saeed
 * @date 2026-03-02
 */

#ifndef BUILTINS_H
#define BUILTINS_H

#include <stddef.h>

/**
 * @struct Builtin
 * @brief A shell built-in command.
 */
typedef struct {
	const char *name;                       /**< The name of the built-in command. */
	int (*function)(int argc, char **argv); /**< The function implementing the built-in command. */
} Builtin;

/**
 * @brief List of shell built-in commands;
 */
extern const Builtin builtins[];

/**
 * @brief The number of shell built-in commands;
 */
extern const size_t builtins_count;

#endif
