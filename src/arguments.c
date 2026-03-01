#include <arguments.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Arguments arguments_new(void) {
	return (Arguments){
		.data     = NULL,
		.length   = 0,
		.capacity = 0,
	};
}

void arguments_drop(Arguments *arguments) {
	assert(arguments != NULL);

	for (size_t i = 0; i < arguments->length; i++) {
		free(arguments->data[i]);
	}
	free((void *)arguments->data);

	arguments->data     = NULL;
	arguments->length   = 0;
	arguments->capacity = 0;
}

bool arguments_reserve(Arguments *arguments, size_t additional) {
	assert(arguments != NULL);

	// the + 1 is for the null terminator
	if (arguments->length + additional + 1 > arguments->capacity) {
		// double the capacity until it is sufficient to hold the additional arguments
		size_t new_capacity = arguments->capacity == 0 ? 1 : arguments->capacity * 2;
		while (new_capacity < arguments->length + additional + 1) {
			new_capacity *= 2;
		}

		char **new_data = (char **)realloc((void *)arguments->data, new_capacity * sizeof(*arguments->data));
		if (new_data == NULL) {
			return false;
		}

		arguments->data     = new_data;
		arguments->capacity = new_capacity;
	}

	return true;
}

bool arguments_append(Arguments *arguments, char *argument) {
	assert(arguments != NULL);

	if (!arguments_reserve(arguments, 1)) {
		return false;
	}

	arguments->data[arguments->length++] = argument;
	arguments->data[arguments->length]   = NULL;

	return true;
}
