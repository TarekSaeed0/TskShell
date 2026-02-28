#include <command.h>

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void write_to_log_file(const char *message) {
	FILE *file = fopen("log.txt", "a");
	if (file == NULL) {
		perror("Error");
		return;
	}

	(void)fprintf(file, "%s\n", message);

	if (fclose(file) != 0) {
		perror("Error");
	}
}

void on_child_exit(int signum) {
	(void)signum;

	waitpid(-1, NULL, WNOHANG);

	write_to_log_file("Child terminated");
}

void setup_environment(void) {
	char *home = getenv("HOME");
	if (home != NULL) {
		chdir(home);
	}
}

void execute_shell_builtin(const Command *command) {
	assert(command->command_type == COMMAND_TYPE_BUILTIN);

	for (size_t i = 0; i < builtins_count; i++) {
		if (strcmp(command_name(command), builtins[i].name) == 0) {
			builtins[i].function((int)command_arguments_count(command), command_arguments(command));
			return;
		}
	}
}

void execute_command(const Command *command) {
	assert(command->command_type == COMMAND_TYPE_EXECUTABLE);

	pid_t pid = fork();

	if (pid == 0) {
		execvp(command_name(command), command_arguments(command));
		perror("Error");
		exit(EXIT_FAILURE);
	} else if (pid == -1) {
		perror("Error");
		exit(EXIT_FAILURE);
	} else if (pid > 0 && command->execution_type == EXECUTION_TYPE_FOREGROUND) {
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

		bool is_only_whitespace = true;
		for (ssize_t i = 0; i < read; i++) {
			if (!isspace((unsigned char)buffer[i])) {
				is_only_whitespace = false;
				break;
			}
		}

		if (is_only_whitespace) {
			continue;
		}

		Command command;
		if (!command_parse(&command, buffer)) {
			(void)fprintf(stderr, "Error: invalid syntax, failed to parse command\n");
			continue;
		}

		switch (command.command_type) {
			case COMMAND_TYPE_BUILTIN:
				execute_shell_builtin(&command);
				break;
			case COMMAND_TYPE_EXECUTABLE:
				execute_command(&command);
				break;
		}

		command_drop(&command);
	}

	free(buffer);
}

int main(void) {
	if (signal(SIGCHLD, on_child_exit) == SIG_ERR) {
		perror("Error");
	}

	setup_environment();

	shell();
}
