CC = clang
CFLAGS = -Wall -Wextra -std=c99 -fsanitize=address -g -O0
TARGET = tests

all: $(TARGET)

$(TARGET): tests.c nonstd.h minunit.h
	$(CC) $(CFLAGS) -o $(TARGET) tests.c

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

format:
	clang-format -i nonstd.h tests.c examples/*.c

.PHONY: all test clean format
