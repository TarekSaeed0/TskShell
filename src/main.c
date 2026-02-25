#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum {
	MAXIMUM_ARGUMENTS = 100,
};

void shell(void) {
	char *buffer       = NULL;
	size_t buffer_size = 0;

	while (true) {
		ssize_t read = getline(&buffer, &buffer_size, stdin);
		if (read == -1) {
			break;
		}

		char *p = buffer;
		p += strspn(p, " \t\n");

		if (*p == '\0') {
			continue;
		}

		char *command = p;
		p += strcspn(p, " \t\n");
		if (*p != '\0') {
			*p++ = '\0';
		}

		char *arguments[MAXIMUM_ARGUMENTS];
		size_t argument_count = 0;

		while (*p != '\0' && argument_count < MAXIMUM_ARGUMENTS - 1) {
			p += strspn(p, " \t\n");
			if (*p == '"') {
				p++;
				arguments[argument_count++] = p;
				p += strcspn(p, "\"");
				if (*p != '\0') {
					*p++ = '\0';
				}
			} else {
				arguments[argument_count++] = p;
				p += strcspn(p, " \t\n");
				if (*p != '\0') {
					*p++ = '\0';
				}
			}
		}

		arguments[argument_count] = NULL;

		printf("Command: %s\n", command);
		for (size_t i = 0; i < argument_count; i++) {
			printf("Argument %zu: %s\n", i, arguments[i]);
		}
	}

	free(buffer);
}

void execute_command(const char *command, char *const arguments[]) {
	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		execvp(command, arguments);
		perror("execvp");
		exit(EXIT_FAILURE);
	} else {
		waitpid(pid, NULL, 0);
	}
}

int main(void) {
	shell();
}
