#ifndef WORD_H
#define WORD_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	char *data;
	size_t length;
	size_t capacity;
} Word;

Word word_new(void);
void word_drop(Word *word);
bool word_reserve(Word *word, size_t additional);
bool word_append_character(Word *word, char character);
bool word_append_string(Word *word, const char *string);
char *word_take_data(Word *word);

#endif // WORD_H
