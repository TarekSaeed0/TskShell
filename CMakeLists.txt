cmake_minimum_required(VERSION 4.2.3)
project(
	lab_1
	VERSION 1.0
	LANGUAGES C
	DESCRIPTION "a simple unix shell"
)

add_executable(lab_1 src/main.c)
target_compile_options(
	lab_1
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
