#include <command.h>

#include <argument.h>
#include <arguments.h>
#include <arguments_builder.h>
#include <environment.h>

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int builtin_cd(int argc, char **argv) {
	char *home = getenv("HOME");
	if (home == NULL) {
		(void)fprintf(stderr, "Error: HOME environment variable is not set\n");
		return EXIT_FAILURE;
	}

	if (argc == 1) {
		if (chdir(home) == -1) {
			perror("Error");
			return EXIT_FAILURE;
		}
	} else if (argc == 2 && argv[1][0] == '~') {
		size_t buffer_size = (size_t)snprintf(NULL, 0, "%s%s", home, argv[1] + 1);
		char *buffer       = malloc(buffer_size + 1);
		if (buffer == NULL) {
			perror("Error");
			exit(EXIT_FAILURE);
		}

		(void)snprintf(buffer, buffer_size + 1, "%s%s", home, argv[1] + 1);

		if (chdir(buffer) == -1) {
			perror("Error");
			free(buffer);
			return EXIT_FAILURE;
		}

		free(buffer);
	} else {
		if (chdir(argv[1]) == -1) {
			perror("Error");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int builtin_echo(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (i > 1) {
			printf(" ");
		}
		printf("%s", argv[i]);
	}
	printf("\n");

	return EXIT_SUCCESS;
}

int builtin_export(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		char *equal_sign = strchr(argv[i], '=');
		if (equal_sign == NULL) {
			(void)fprintf(stderr, "Usage: export <name>=<value>\n");
			return EXIT_FAILURE;
		}

		*equal_sign       = '\0';
		const char *name  = argv[i];
		const char *value = equal_sign + 1;

		if (!set_environment_variable(name, value)) {
			perror("Error");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int builtin_exit(int argc, char **argv) {
	(void)argc, (void)argv;

	exit(EXIT_SUCCESS);
}

const Builtin builtins[] = {
	{ "cd", builtin_cd },
	{ "echo", builtin_echo },
	{ "export", builtin_export },
	{ "exit", builtin_exit },
};

const size_t builtins_count = sizeof(builtins) / sizeof(*builtins);

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

	char *key = strndup(identifier, identifier_length);
	*value    = get_environment_variable(key);
	free(key);

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

	arguments_builder_ensure_in_argument(builder);

	++*start;

	while (**start != '\0' && **start != '"') {
		if (is_expansion_start(*start)) {
			const char *value = NULL;
			if (!parse_expansion(&value, start)) {
				return false;
			}

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
	return character != '\0' && !isspace((unsigned char)character) && character != '\'' && character != '"' && character != '$' && character != '&';
}

static bool parse_word_element(ArgumentsBuilder *builder, const char **start) {
	assert(builder != NULL && start != NULL && *start != NULL);

	if (is_expansion_start(*start)) {
		const char *value = NULL;
		if (!parse_expansion(&value, start)) {
			return false;
		}

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

		return true;
	}

	return false;
}

static bool parse_word(ArgumentsBuilder *builder, const char **start) {
	assert(builder != NULL && start != NULL && *start != NULL);

	while (**start != '\0' && !isspace((unsigned char)**start)) {
		if (!parse_word_element(builder, start)) {
			return false;
		}
	}

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

	if (!parse_word(&builder, start)) {
		arguments_builder_drop(&builder);
		return false;
	}

	command->execution_type = EXECUTION_TYPE_FOREGROUND;

	while (true) {
		skip_whitespace(start);

		if (**start == '\0') {
			break;
		}

		if (**start == '&') {
			++*start;
			command->execution_type = EXECUTION_TYPE_BACKGROUND;
			break;
		}

		if (!parse_word(&builder, start)) {
			arguments_builder_drop(&builder);
			return false;
		}
	}

	command->arguments    = builder.arguments;
	command->command_type = determine_command_type(command_name(command));

	return true;
}

bool command_parse(Command *command, const char *string) {
	return parse_command(command, &string);
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
