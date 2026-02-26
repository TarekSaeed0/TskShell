#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*

=============================
Grammer
=============================

command =

<quoted-string> = "\"",
<ascii-character>

*/

typedef enum {
	COMMAND_TYPE_BUILTIN,
	COMMAND_TYPE_EXECUTABLE,
	COMMAND_TYPE_ERROR,
} CommandType;

typedef enum {
	EXECUTION_TYPE_FOREGROUND,
	EXECUTION_TYPE_BACKGROUND,
} ExecutionType;

typedef struct {
	CommandType command_type;
	ExecutionType execution_type;
	char *name;
	char **arguments;
	size_t argument_count;
} Command;

typedef struct {
	const char *name;
	int (*function)(int argc, char **argv);
} BuiltinCommand;

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

		if (setenv(name, value, 1) == -1) {
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

const BuiltinCommand builtins[] = {
	{ "cd", builtin_cd },
	{ "echo", builtin_echo },
	{ "export", builtin_export },
	{ "exit", builtin_exit },
};

CommandType determine_command_type(const char *name) {
	if (name == NULL) {
		return COMMAND_TYPE_ERROR;
	}

	for (size_t i = 0; i < sizeof(builtins) / sizeof(*builtins); i++) {
		if (strcmp(name, builtins[i].name) == 0) {
			return COMMAND_TYPE_BUILTIN;
		}
	}

	return COMMAND_TYPE_EXECUTABLE;
}

Command parse_command(char *input) {
	Command command = {
		.execution_type = EXECUTION_TYPE_FOREGROUND,
		.command_type   = COMMAND_TYPE_ERROR,
		.name           = NULL,
		.arguments      = NULL,
		.argument_count = 0,
	};

	char *p = input;
	while (true) {
		char **arguments = (char **)realloc((void *)command.arguments, (command.argument_count + 1) * sizeof(*command.arguments));
		if (arguments == NULL) {
			perror("Error");
			exit(EXIT_FAILURE);
		}

		command.arguments = arguments;

		p += strspn(p, " \t\n");
		if (*p == '\0') {
			break;
		}

		if (*p == '&') {
			command.execution_type = EXECUTION_TYPE_BACKGROUND;
			p++;
			break;
		}

		if (*p == '"') {
			p++;
			command.arguments[command.argument_count++] = p;
			p += strcspn(p, "\"");
			if (*p != '\0') {
				*p++ = '\0';
			}
		} else {
			command.arguments[command.argument_count++] = p;
			p += strcspn(p, " \t\n");
			if (*p != '\0') {
				*p++ = '\0';
			}
		}
	}

	command.arguments[command.argument_count] = NULL;

	command.name                              = command.arguments[0];
	command.command_type                      = determine_command_type(command.name);

	return command;
}

void execute_shell_builtin(Command command) {
	assert(command.name != NULL && command.command_type == COMMAND_TYPE_BUILTIN);

	for (size_t i = 0; i < sizeof(builtins) / sizeof(*builtins); i++) {
		if (strcmp(command.name, builtins[i].name) == 0) {
			builtins[i].function((int)command.argument_count, command.arguments);
			return;
		}
	}
}

void execute_command(Command command) {
	assert(command.name != NULL && command.command_type == COMMAND_TYPE_EXECUTABLE);

	pid_t pid = fork();

	if (pid == 0) {
		execvp(command.name, command.arguments);
		perror("Error");
		exit(EXIT_FAILURE);
	} else if (pid == -1) {
		perror("Error");
		exit(EXIT_FAILURE);
	} else if (command.execution_type == EXECUTION_TYPE_FOREGROUND) {
		waitpid(pid, NULL, 0);
	}
}

void shell(void) {
	char *buffer       = NULL;
	size_t buffer_size = 0;

	while (true) {
		printf("> ");

		ssize_t read = getline(&buffer, &buffer_size, stdin);
		if (read == -1) {
			break;
		}

		Command command = parse_command(buffer);
		switch (command.command_type) {
			case COMMAND_TYPE_BUILTIN:
				execute_shell_builtin(command);
				break;
			case COMMAND_TYPE_EXECUTABLE:
				execute_command(command);
				break;
			case COMMAND_TYPE_ERROR:
				break;
		}

		free((void *)command.arguments);
	}

	free(buffer);
}

void on_child_exit(int signum) {
	(void)signum;
	while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

void setup_environment(void) {
}

int main(void) {
	(void)signal(SIGCHLD, on_child_exit);
	setup_environment();
	shell();
}
