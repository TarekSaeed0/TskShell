#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	char **data;
	size_t length;
	size_t capacity;
} Arguments;

Arguments arguments_new(void);
void arguments_drop(Arguments *arguments);
bool arguments_reserve(Arguments *arguments, size_t additional);
bool arguments_append(Arguments *arguments, char *argument);

#endif // ARGUMENTS_H
