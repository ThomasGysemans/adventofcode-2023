# My first Makefile, what a mess!

# to tell Make that "challenge", "execute" and "clear" are not files, but targets to execute.
.PHONY: challenge execute clear

# variables
GPP=g++ -std=c++20 -Wall -Wextra # using C++20 only, I can't guarantee the challenges will work with other versions
EXEC=build/executable
target := # will receive the chosen challenge in the arguments of "make"
src := main.cpp

# functions.
# $(shell ...) executes a shell command.
compile = $(shell ${GPP} $(1) -o ${EXEC})

# ${EXEC} is replaced with the value of the EXEC variable
# execute: ${EXEC} means the target needs the executable as a dependency
# and therefore will not execute if it doesn't exist.
execute: ${EXEC}
	@./${EXEC}

# Here, "challenge_folder" is a temporary variable that uses "target" to complete the path to the sources folder.
# "target" needs to be defined when executing `make`, like this:
# `make challenge target=42` (compiles and executes the C++ code in the folder "./challenge-42").
challenge: challenge_folder := "challenge-${target}"
challenge: full_path_to_src := $(challenge_folder)/$(src)
# "@set -e;" tells Make to stop the target as soon as there is a non-zero exit code.
# In Make, each line is treated in another context, therefore, so as to remember "set -e" it all needs to be executed on the same line,
# and to do this, we separate each command with a semicolon and then "\" is used to tell Make that it's a unique line.
challenge:
	@set -e; \
	if [ -z "$(target)" ]; then \
		echo "Error: execute a specific challenge with 'make challenge target=X' with X the number of the challenge to test"; \
		exit 1; \
	fi
	@set -e; \
	if [ ! -e $(challenge_folder) ]; then \
		echo "I didn't do the challenge number $(target)."; \
		exit 1; \
	fi
	@set -e; \
	if [ ! -e $(full_path_to_src) ]; then \
		echo "The C++ file '$(full_path_to_src)' doesn't exist."; \
		exit 1; \
	fi
	@$(call compile, $(full_path_to_src))
	@$(MAKE) execute

# deletes the executable in `./build`
clear: ${EXEC}
	@rm -fr ${EXEC}