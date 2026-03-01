#ifndef COMMAND_H
#define COMMAND_H

#include <arguments.h>

#include <stdbool.h>
#include <stddef.h>

/**
 * @enum CommandType
 * @brief Whether a command is a shell built-in or an executable.
 */
typedef enum {
	COMMAND_TYPE_BUILTIN,
	COMMAND_TYPE_EXECUTABLE,
} CommandType;

/**
 * @enum ExecutionType
 * @brief Whether a command should run in the foreground or background.
 */
typedef enum {
	EXECUTION_TYPE_FOREGROUND,
	EXECUTION_TYPE_BACKGROUND,
} ExecutionType;

/**
 * @struct Command
 * @brief A shell command.
 */
typedef struct {
	Arguments arguments;
	CommandType command_type;
	ExecutionType execution_type;
} Command;

/**
 * @brief Parses command from string.
 *
 * =================================================
 *  Command Syntax
 * =================================================
 *
 * command        = word , { word }, [ & ] ;
 * word           = { word_element } ;
 *
 * word_element   = unquoted_character
 *                | single_quoted
 *                | double_quoted
 *                | expansion ;
 *
 * unquoted_character        = ? any character except whitespace, ', ", & and $ ? ;
 * single_quoted        = "'" , { single_quoted_character } , "'" ;
 * double_quoted        = '"' , { double_quoted_character | expansion } , '"' ;
 *
 * single_quoted_character   = ? any character except ' ? ;
 * double_quoted_character   = ? any character except " and $ ? ;
 *
 * expansion      = "$" , ( identifier | "{" , identifier , "}" ) ;
 *
 * identifier     = letter | "_", { letter | digit | "_" } ;
 * letter         = "a".."z" | "A".."Z" ;
 * digit          = "0".."9" ;
 *
 * @param[out] command Pointer to store the parsed command.
 * @param[in] string The string to be parsed.
 * @return true if successful, false otherwise.
 */
bool command_parse(Command *command, const char *string);

/**
 * @brief Releases resources held by the given command.
 *
 * @param[inout] command Pointer to the command to drop.
 */
void command_drop(Command *command);

/**
 * @brief Gets the name of the given command.
 *
 * @param[in] command Pointer to the command.
 * @return The name of the command.
 */
const char *command_name(const Command *command);

/**
 * @brief Gets the arguments of the given command.
 *
 * @param[in] command Pointer to the command.
 * @return The null-terminated list of arguments.
 */
char **command_arguments(const Command *command);

/**
 * @brief Gets the number of arguments in the given command.
 *
 * @param[in] command Pointer to the command.
 * @return The number of arguments.
 */
size_t command_arguments_count(const Command *command);

#endif // COMMAND_H
