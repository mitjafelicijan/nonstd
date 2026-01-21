# Makefile for nonstd.h examples

CC = clang
CFLAGS = -Wall -Wextra -std=c99 -fsanitize=address -g -O0
LDFLAGS =

# Example targets
EXAMPLES = foreach stringv stringb array slice arena files

# Default target
all: $(EXAMPLES)

# Build individual examples
foreach: foreach.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

stringv: stringv.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

stringb: stringb.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

array: array.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

slice: slice.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

arena: arena.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

files: files.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Run all examples
run: all
	@echo "\n=== Running stringv ===\n"
	@./stringv
	@echo "\n=== Running stringb ===\n"
	@./stringb
	@echo "\n=== Running foreach ===\n"
	@./foreach
	@echo "\n=== Running array ===\n"
	@./array
	@echo "\n=== Running slice ===\n"
	@./slice
	@echo "\n=== Running arena ===\n"
	@./arena
	@echo "\n=== Running files ===\n"
	@./files

# Clean build artifacts
clean:
	rm -f $(EXAMPLES)

.PHONY: all run clean
