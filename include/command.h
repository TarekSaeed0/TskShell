#ifndef COMMAND_H
#define COMMAND_H

#include <arguments.h>

#include <stdbool.h>
#include <stddef.h>

/*

=================================================
Command Syntax
=================================================

command        = word , { word }, [ & ] ;
word           = { word_element } ;

word_element   = unquoted_character
               | single_quoted
               | double_quoted
               | expansion ;

unquoted_character        = ? any character except whitespace, ', ", & and $ ? ;
single_quoted        = "'" , { single_quoted_character } , "'" ;
double_quoted        = '"' , { double_quoted_character | expansion } , '"' ;

single_quoted_character   = ? any character except ' ? ;
double_quoted_character   = ? any character except " and $ ? ;

expansion      = "$" , ( identifier | "{" , identifier , "}" ) ;

identifier     = letter | "_", { letter | digit | "_" } ;
letter         = "a".."z" | "A".."Z" ;
digit          = "0".."9" ;

*/

typedef enum {
	COMMAND_TYPE_BUILTIN,
	COMMAND_TYPE_EXECUTABLE,
} CommandType;

typedef enum {
	EXECUTION_TYPE_FOREGROUND,
	EXECUTION_TYPE_BACKGROUND,
} ExecutionType;

typedef struct {
	Arguments arguments;
	CommandType command_type;
	ExecutionType execution_type;
} Command;

bool command_parse(Command *command, const char *string);
void command_drop(Command *command);
const char *command_name(const Command *command);
char **command_arguments(const Command *command);
size_t command_arguments_count(const Command *command);

typedef struct {
	const char *name;
	int (*function)(int argc, char **argv);
} Builtin;

extern const Builtin builtins[];
extern const size_t builtins_count;

#endif // COMMAND_H
