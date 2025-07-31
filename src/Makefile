CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra
BUILD_DIR = ../build/graph
SOURCES = graph.c math_func.c reader.c stack.c token.c

all: graph
graph:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILD_DIR)

clean:
	rm  $(BUILD_DIR)

format:
	clang-format -i *.c *.h

check-format:
	clang-format -n *.c *.h

leak_quest1:
	leaks -atExit -- $(BUILD_DIR)


cpp_check:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES)

.PHONY: clean