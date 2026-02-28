#include <word.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Word word_new(void) {
	return (Word){
		.data     = NULL,
		.length   = 0,
		.capacity = 0,
	};
}

void word_drop(Word *word) {
	assert(word != NULL);

	free(word->data);
}

bool word_reserve(Word *word, size_t additional) {
	assert(word != NULL);

	if (word->length + additional + 1 > word->capacity) {
		size_t new_capacity = word->capacity == 0 ? 1 : word->capacity * 2;
		while (new_capacity < word->length + additional + 1) {
			new_capacity *= 2;
		}

		char *new_data = realloc(word->data, new_capacity);
		if (new_data == NULL) {
			return false;
		}

		word->data     = new_data;
		word->capacity = new_capacity;
	}

	return true;
}

bool word_append_character(Word *word, char character) {
	assert(word != NULL);

	if (!word_reserve(word, 1)) {
		return false;
	}

	word->data[word->length++] = character;
	word->data[word->length]   = '\0';

	return true;
}

bool word_append_string(Word *word, const char *string) {
	assert(word != NULL);

	size_t length = strlen(string);

	if (!word_reserve(word, length)) {
		return false;
	}

	memcpy(word->data + word->length, string, length);
	word->length += length;
	word->data[word->length] = '\0';

	return true;
}

char *word_take_data(Word *word) {
	assert(word != NULL);

	return word->data;
}
