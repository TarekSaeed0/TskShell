#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	char *data;
	size_t length;
	size_t capacity;
} Argument;

Argument argument_new(void);
void argument_drop(Argument *argument);
bool argument_reserve(Argument *argument, size_t additional);
bool argument_append_character(Argument *argument, char character);
bool argument_append_string(Argument *argument, const char *string);
char *argument_take_data(Argument *argument);

#endif // ARGUMENT_H
