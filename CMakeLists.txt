cmake_minimum_required(VERSION 4.2.3)
project(
	tskshell
	VERSION 1.0
	LANGUAGES C
	DESCRIPTION "a simple unix shell"
)

add_executable(tskshell src/environment.c src/argument.c src/arguments.c src/arguments_builder.c src/command.c src/builtins.c src/main.c)
target_include_directories(tskshell PRIVATE include)
target_compile_options(
	tskshell
	PRIVATE -Werror
			-Wall
			-Wextra
			-pedantic
			-Wfloat-equal
			-Wundef
			-Wshadow
			-Wpointer-arith
			-Wcast-align
			-Wstrict-prototypes
			-Wstrict-overflow=5
			-Wwrite-strings
			-Wcast-qual
			-Wconversion
)
