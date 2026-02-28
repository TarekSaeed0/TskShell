#ifndef ARGUMENTS_BUILDER_H
#define ARGUMENTS_BUILDER_H

#include <argument.h>
#include <arguments.h>

typedef struct {
	Arguments arguments;
	Argument argument;
	bool in_argument;
} ArgumentsBuilder;

ArgumentsBuilder arguments_builder_new(void);
void arguments_builder_drop(ArgumentsBuilder *builder);
bool arguments_builder_append_character(ArgumentsBuilder *builder, char character);
bool arguments_builder_append_string(ArgumentsBuilder *builder, const char *string);
void arguments_builder_ensure_in_argument(ArgumentsBuilder *builder);
bool arguments_builder_end_argument(ArgumentsBuilder *builder);

#endif // ARGUMENTS_BUILDER_H
