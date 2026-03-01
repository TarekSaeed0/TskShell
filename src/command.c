#include <command.h>

#include <argument.h>
#include <arguments.h>
#include <arguments_builder.h>
#include <builtins.h>
#include <environment.h>

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static inline void skip_whitespace(const char **start) {
	assert(start != NULL && *start != NULL);

	while (isspace((unsigned char)**start)) {
		++*start;
	}
}

static inline bool is_identifier_start_character(char character) {
	return isalpha((unsigned char)character) || character == '_';
}
static inline bool is_identifier_character(char character) {
	return isalnum((unsigned char)character) || character == '_';
}
static bool parse_identifier(const char **identifier, size_t *identifier_length, const char **start) {
	assert(identifier != NULL && identifier_length != NULL && start != NULL && *start != NULL);

	if (!is_identifier_start_character(**start)) {
		return false;
	}

	*identifier        = *start;
	*identifier_length = 1;

	++*start;

	while (is_identifier_character(**start)) {
		++*start;
		++*identifier_length;
	}

	return true;
}

static inline bool is_expansion_start(const char *start) {
	assert(start != NULL);

	return *start == '$' && (start[1] == '{' || is_identifier_start_character(start[1]));
}
static bool parse_expansion(const char **value, const char **start) {
	assert(value != NULL && start != NULL && *start != NULL);

	const char *original_start = *start;

	if (**start != '$') {
		return false;
	}

	++*start;

	const char *identifier   = NULL;
	size_t identifier_length = 0;

	// parse the identifier, which can be either in the form $identifier or ${identifier}
	if (**start == '{') {
		++*start;
		if (!parse_identifier(&identifier, &identifier_length, start)) {
			*start = original_start;
			return false;
		}

		if (**start != '}') {
			*start = original_start;
			return false;
		}

		++*start;
	} else {
		if (!parse_identifier(&identifier, &identifier_length, start)) {
			*start = original_start;
			return false;
		}
	}

	char *name = strndup(identifier, identifier_length);
	*value     = get_environment_variable(name);
	free(name);

	// the value of an undefined variable is treated as an empty string
	if (*value == NULL) {
		*value = "";
	}

	return true;
}

static inline bool is_single_quoted_start(const char *start) {
	assert(start != NULL);

	return *start == '\'';
}
static bool parse_single_quoted(ArgumentsBuilder *builder, const char **start) {
	assert(builder != NULL && start != NULL && *start != NULL);

	if (**start != '\'') {
		return false;
	}

	// this is so that an empty single quoted string is treated as an empty argument
	arguments_builder_ensure_in_argument(builder);

	++*start;

	while (**start != '\0' && **start != '\'') {
		if (!arguments_builder_append_character(builder, **start)) {
			return false;
		}
		++*start;
	}

	if (**start != '\'') {
		return false;
	}

	++*start;

	return true;
}

static inline bool is_double_quoted_start(const char *start) {
	assert(start != NULL);

	return *start == '\"';
}
static bool parse_double_quoted(ArgumentsBuilder *builder, const char **start) {
	assert(builder != NULL && start != NULL && *start != NULL);

	if (**start != '"') {
		return false;
	}

	// this is so that an empty double quoted string is treated as an empty argument
	arguments_builder_ensure_in_argument(builder);

	++*start;

	while (**start != '\0' && **start != '"') {
		if (is_expansion_start(*start)) {
			const char *value = NULL;
			if (!parse_expansion(&value, start)) {
				return false;
			}

			// append the expanded value to the current argument without splitting
			if (!arguments_builder_append_string(builder, value)) {
				return false;
			}
		} else {
			if (!arguments_builder_append_character(builder, **start)) {
				return false;
			}
			++*start;
		}
	}

	if (**start != '"') {
		return false;
	}

	++*start;

	return true;
}

static inline bool is_unquoted_character(char character) {
	return character != '\0' && !isspace((unsigned char)character) && character != '\'' && character != '"' && character != '&';
}

static bool is_word_element_start(const char *start) {
	assert(start != NULL);

	return is_expansion_start(start) || is_single_quoted_start(start) || is_double_quoted_start(start) || is_unquoted_character(*start);
}
static bool parse_word_element(ArgumentsBuilder *builder, const char **start) {
	assert(builder != NULL && start != NULL && *start != NULL);

	if (is_expansion_start(*start)) {
		const char *value = NULL;
		if (!parse_expansion(&value, start)) {
			return false;
		}

		// unquoted expansions are split into multiple arguments based on whitespace
		// if the expansion consists only of whitespace, no arguments are added
		while (*value != '\0') {
			if (isspace((unsigned char)*value)) {
				arguments_builder_end_argument(builder);

				skip_whitespace(&value);
			} else {
				if (!arguments_builder_append_character(builder, *value)) {
					return false;
				}
				++value;
			}
		}

		return true;
	}

	if (is_single_quoted_start(*start)) {
		if (!parse_single_quoted(builder, start)) {
			return false;
		}

		return true;
	}

	if (is_double_quoted_start(*start)) {
		if (!parse_double_quoted(builder, start)) {
			return false;
		}

		return true;
	}

	if (is_unquoted_character(**start)) {
		if (!arguments_builder_append_character(builder, **start)) {
			return false;
		}

		++*start;
	}

	return true;
}

static bool parse_word(ArgumentsBuilder *builder, const char **start) {
	assert(builder != NULL && start != NULL && *start != NULL);

	// parse word elements until we encounter a character that can't start a word element
	while (is_word_element_start(*start)) {
		if (!parse_word_element(builder, start)) {
			return false;
		}
	}

	// end the current argument
	if (!arguments_builder_end_argument(builder)) {
		return false;
	}

	return true;
}

CommandType determine_command_type(const char *name) {
	assert(name != NULL);

	for (size_t i = 0; i < builtins_count; i++) {
		if (strcmp(name, builtins[i].name) == 0) {
			return COMMAND_TYPE_BUILTIN;
		}
	}

	return COMMAND_TYPE_EXECUTABLE;
}

bool parse_command(Command *command, const char **start) {
	assert(command != NULL && start != NULL && *start != NULL);

	ArgumentsBuilder builder = arguments_builder_new();

	skip_whitespace(start);

	command->execution_type = EXECUTION_TYPE_FOREGROUND;

	while (true) {
		skip_whitespace(start);

		if (**start == '\0') {
			break;
		}

		if (**start == '&') {
			// encountered &, which means the command should be executed in the background
			++*start;
			command->execution_type = EXECUTION_TYPE_BACKGROUND;
			break;
		}

		// parse the next word
		if (!parse_word(&builder, start)) {
			arguments_builder_drop(&builder);
			return false;
		}
	}

	Arguments arguments = arguments_builder_build(&builder);

	if (arguments.length == 0) {
		// a command must have at least one argument
		arguments_drop(&arguments);
		return false;
	}

	command->arguments    = arguments;
	command->command_type = determine_command_type(command_name(command));

	return true;
}

bool command_parse(Command *command, const char *string) {
	if (!parse_command(command, &string)) {
		return false;
	}

	// check if there's any non-whitespace characters left after parsing the command

	skip_whitespace(&string);

	return *string == '\0';
}

void command_drop(Command *command) {
	assert(command != NULL);

	arguments_drop(&command->arguments);
}

const char *command_name(const Command *command) {
	assert(command != NULL);

	return command->arguments.data[0];
}

char **command_arguments(const Command *command) {
	assert(command != NULL);

	return command->arguments.data;
}

size_t command_arguments_count(const Command *command) {
	assert(command != NULL);

	return command->arguments.length;
}
