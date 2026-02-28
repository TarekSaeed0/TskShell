#include <argument.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Argument argument_new(void) {
	return (Argument){
		.data     = NULL,
		.length   = 0,
		.capacity = 0,
	};
}

void argument_drop(Argument *argument) {
	assert(argument != NULL);

	free(argument->data);

	argument->data     = NULL;
	argument->length   = 0;
	argument->capacity = 0;
}

bool argument_reserve(Argument *argument, size_t additional) {
	assert(argument != NULL);

	if (argument->length + additional + 1 > argument->capacity) {
		size_t new_capacity = argument->capacity == 0 ? 1 : argument->capacity * 2;
		while (new_capacity < argument->length + additional + 1) {
			new_capacity *= 2;
		}

		char *new_data = realloc(argument->data, new_capacity);
		if (new_data == NULL) {
			return false;
		}

		argument->data     = new_data;
		argument->capacity = new_capacity;
	}

	argument->data[argument->length] = '\0';

	return true;
}

bool argument_append_character(Argument *argument, char character) {
	assert(argument != NULL);

	if (!argument_reserve(argument, 1)) {
		return false;
	}

	argument->data[argument->length++] = character;
	argument->data[argument->length]   = '\0';

	return true;
}

bool argument_append_string(Argument *argument, const char *string) {
	assert(argument != NULL);

	size_t length = strlen(string);

	if (!argument_reserve(argument, length)) {
		return false;
	}

	memcpy(argument->data + argument->length, string, length);
	argument->length += length;
	argument->data[argument->length] = '\0';

	return true;
}

char *argument_take_data(Argument *argument) {
	assert(argument != NULL);

	char *data         = argument->data;

	argument->data     = NULL;
	argument->length   = 0;
	argument->capacity = 0;

	return data;
}
