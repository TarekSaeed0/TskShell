#include <arguments_builder.h>

#include <assert.h>

ArgumentsBuilder arguments_builder_new(void) {
	return (ArgumentsBuilder){
		.arguments   = arguments_new(),
		.argument    = argument_new(),
		.in_argument = false,
	};
}

void arguments_builder_drop(ArgumentsBuilder *builder) {
	assert(builder != NULL);

	arguments_drop(&builder->arguments);
	argument_drop(&builder->argument);
	builder->in_argument = false;
}

bool arguments_builder_append_character(ArgumentsBuilder *builder, char character) {
	assert(builder != NULL);

	if (!builder->in_argument) {
		builder->in_argument = true;
	}

	return argument_append_character(&builder->argument, character);
}

bool arguments_builder_append_string(ArgumentsBuilder *builder, const char *string) {
	assert(builder != NULL);

	if (!builder->in_argument) {
		builder->in_argument = true;
	}

	return argument_append_string(&builder->argument, string);
}

void arguments_builder_ensure_in_argument(ArgumentsBuilder *builder) {
	assert(builder != NULL);

	builder->in_argument = true;
}

bool arguments_builder_end_argument(ArgumentsBuilder *builder) {
	assert(builder != NULL);

	if (!builder->in_argument) {
		return true;
	}

	if (builder->argument.length == 0) {
		// call reserve to ensure that the empty argument's data isn't NULL, even if it's empty
		if (!argument_reserve(&builder->argument, 0)) {
			return false;
		}
	}

	if (!arguments_append(&builder->arguments, argument_take_data(&builder->argument))) {
		return false;
	}

	builder->in_argument = false;

	return true;
}

Arguments arguments_builder_build(ArgumentsBuilder *builder) {
	assert(builder != NULL);

	Arguments arguments = builder->arguments;

	builder->arguments  = arguments_new();
	argument_drop(&builder->argument);
	builder->in_argument = false;

	return arguments;
}
