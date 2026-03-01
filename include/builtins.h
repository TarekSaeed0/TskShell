#ifndef BUILTINS_H
#define BUILTINS_H

#include <stddef.h>

/**
 * @struct Builtin
 * @brief A shell builtin command.
 */
typedef struct {
	const char *name;
	int (*function)(int argc, char **argv);
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
