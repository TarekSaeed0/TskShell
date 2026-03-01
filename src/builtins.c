#include <builtins.h>

#include <argument.h>
#include <arguments.h>
#include <arguments_builder.h>
#include <environment.h>

#include <assert.h>
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
		// cd called without arguments, go to the user's home directory
		if (chdir(home) == -1) {
			perror("Error");
			return EXIT_FAILURE;
		}
	} else if (argc == 2 && argv[1][0] == '~') {
		// cd called either with "~" or with a path starting with "~", expand the ~ to the user's home directory
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
