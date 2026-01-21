**nonstd** is a single-header C library providing a collection of "non-standard"
but highly useful utilities, data structures, and type definitions that are
often missing from the C standard library. It aims to make C programming more
ergonomic and productive. It aims to be C99 compliant.

> [!IMPORTANT]
> There are more involved libraries out there providing better granularity
> and functionality. This library is intended for ease of use first and
> foremost. It is not a replacement for those libraries.

## Features

- **Shorthand Types**: Concise integer types (`i8`, `u32`, `usize`, etc.) for better readability.
- **Utility Macros**: Common helpers like `countof`, `MIN`, `MAX`, `CLAMP`, and `static_foreach`.
- **String View (`stringv`)**: Non-owning, read-only string references to avoid unnecessary copies.
- **String Builder (`stringb`)**: Growable, mutable string buffer for efficient string construction.
- **Dynamic Array (`array`)**: Generic growable arrays implemented via macros (similar to `std::vector` in C++).
- **Slices (`slice`)**: Generic non-owning views into arrays.
- **Memory Arena**: Simple block-based arena allocator for bulk memory management.
- **File I/O**: Helper functions to read and write entire files with a single call.
- **Logging**: Simple, leveled logging with ANSI colors and timestamps.

## Installation

`nonstd` is a single-header library. To use it:

1. Copy `nonstd.h` into your project's include directory.
2. In **one** C source file, define `NONSTD_IMPLEMENTATION` before including the
   header to create the implementation:

```c
#define NONSTD_IMPLEMENTATION
#include "nonstd.h"
```

3. In other files, just include it normally:

```c
#include "nonstd.h"
```

## Usage

Check the `examples` directory for usage examples. But here are a few simple
examples:

### 1. Basic Types & Macros

```c
#include "nonstd.h"

void example() {
    i32 x = 10;
    u64 y = 2000;
    usize size = 1024;
    
    int numbers[] = {1, 2, 3, 4, 5};
    printf("Count: %zu\n", countof(numbers)); // 5
    
    int val = CLAMP(100, 0, 10); // 10
    
    // Static foreach loop
    int n;
    static_foreach(int, n, numbers) {
        printf("%d ", n);
    }
}
```

### 2. Dynamic Arrays

Create type-safe growable arrays for any type.

```c
// Define an array of integers
array(int) numbers;
array_init(numbers);

// Push values
array_push(numbers, 10);
array_push(numbers, 20);
array_push(numbers, 30);

// Iterate (index basic loop or helper macro)
int val;
array_foreach(numbers, val) {
    printf("%d\n", val);
}

// Access by index
if (numbers.length > 0) {
    printf("First: %d\n", numbers.data[0]);
    // or
    printf("First: %d\n", array_get(numbers, 0));
}

// Clean up
array_free(numbers);
```

### 3. String Views & Builders

**String View (`stringv`)**:
Ideal for parsing and passing strings around without allocation.

```c
const char *raw = "Hello World";
stringv sv = sv_from_cstr(raw);
stringv word = sv_slice(sv, 0, 5); // "Hello" (no allocation)

if (sv_starts_with(sv, sv_from_cstr("Hello"))) {
    // ...
}
```

**String Builder (`stringb`)**:
Efficiently construct strings.

```c
stringb sb;
sb_init(&sb, 0); // 0 = default capacity

sb_append_cstr(&sb, "Hello");
sb_append_char(&sb, ' ');
sb_append_cstr(&sb, "World");

printf("%s\n", sb.data); // "Hello World"

sb_free(&sb);
```

### 4. Memory Arena

Efficiently allocate many small objects and free them all at once.

```c
Arena arena = arena_make();

// Allocations are fast and contiguous within blocks
void *obj1 = arena_alloc(&arena, 64);
void *obj2 = arena_alloc(&arena, 128);

// growth is automatic if a block is full

// Free everything at once
arena_free(&arena);
```

### 5. File I/O Helpers

Read or write files with a single functional call.

```c
size_t size;
char *content = read_entire_file("data.txt", &size);

if (content) {
    printf("Read %zu bytes:\n%s\n", size, content);
    free(content); // Standard free (unless using arena)
}

// Or read directly into a string builder
stringb file_sb = read_entire_file_sb("config.ini");
// ... use file_sb ...
sb_free(&file_sb);

```

### 6. Logging

Simple logging with levels (`ERROR`, `WARN`, `INFO`, `DEBUG`), timestamps, and colors.

```c
// Set log level (default is INFO)
set_log_level(LOG_DEBUG);

// Use macros for logging
LOG_INFO_MSG("Starting application...");
LOG_DEBUG_MSG("Variable x = %d", 42);
LOG_WARN_MSG("Low memory warning");
LOG_ERROR_MSG("Connection failed");

// Environment variable override supported:
// LOG_LEVEL=0 (ERROR) ... 3 (DEBUG)
```

## Testing

The project includes a test suite using `minunit`.

To build and run the tests:

```bash
make test
```
