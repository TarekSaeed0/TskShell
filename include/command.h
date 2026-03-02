/**
 * @file command.h
 * @brief Defines the data structure for representing shell commands and related functions for parsing and managing them.
 * @author Tarek Saeed
 * @date 2026-03-02
 */

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
	Arguments arguments;          /**< The list of arguments for the command, where the first argument is the command name. */
	CommandType command_type;     /**< The type of the command (built-in or executable). */
	ExecutionType execution_type; /**< The execution type of the command (foreground or background). */
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
 *
 * @memberof Command
 */
bool command_parse(Command *command, const char *string);

/**
 * @brief Releases resources held by the given command.
 *
 * @param[inout] command Pointer to the command to drop.
 *
 * @memberof Command
 */
void command_drop(Command *command);

/**
 * @brief Gets the name of the given command.
 *
 * @param[in] command Pointer to the command.
 * @return The name of the command.
 *
 * @memberof Command
 */
const char *command_name(const Command *command);

/**
 * @brief Gets the arguments of the given command.
 *
 * @param[in] command Pointer to the command.
 * @return The null-terminated list of arguments.
 *
 * @memberof Command
 */
char **command_arguments(const Command *command);

/**
 * @brief Gets the number of arguments in the given command.
 *
 * @param[in] command Pointer to the command.
 * @return The number of arguments.
 *
 * @memberof Command
 */
size_t command_arguments_count(const Command *command);

#endif // COMMAND_H
