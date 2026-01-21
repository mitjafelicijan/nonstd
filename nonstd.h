#ifndef NONSTD_H
#define NONSTD_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NONSTD_DEF
#ifdef NONSTD_STATIC
#define NONSTD_DEF static
#else
#define NONSTD_DEF extern
#endif
#endif

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef intptr_t isize;
typedef uintptr_t usize;
typedef char c8;

#define countof(a) (sizeof(a) / sizeof((a)[0]))

#define static_foreach(type, var, array)                 \
	for (size_t _i_##var = 0, _n_##var = countof(array); \
		 _i_##var < _n_##var && ((var) = (array)[_i_##var], 1); ++_i_##var)

#define ALLOC(type, n) ((type *)malloc((n) * sizeof(type)))
#define REALLOC(ptr, type, n) ((type *)realloc((ptr), (n) * sizeof(type)))
#define FREE(ptr)   \
	do {            \
		free(ptr);  \
		ptr = NULL; \
	} while (0)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, lo, hi) (MIN((hi), MAX((lo), (x))))

#define UNUSED(x) (void)(x)

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define STATIC_ASSERT(expr, msg) _Static_assert((expr), msg)
#else
#define STATIC_ASSERT(expr, msg) \
	typedef char static_assertion_##msg[(expr) ? 1 : -1]
#endif

// String view - read-only, non-owning reference to a string
typedef struct {
	const char *data;
	size_t length;
} stringv;

NONSTD_DEF stringv sv_from_cstr(const char *s);
NONSTD_DEF stringv sv_from_parts(const char *data, size_t length);
NONSTD_DEF stringv sv_slice(stringv sv, size_t start, size_t end);
NONSTD_DEF int sv_equals(stringv a, stringv b);
NONSTD_DEF int sv_starts_with(stringv sv, stringv prefix);
NONSTD_DEF int sv_ends_with(stringv sv, stringv suffix);

// String builder - owning, mutable, dynamically growing string buffer
typedef struct {
	char *data;
	size_t length;
	size_t capacity;
} stringb;

NONSTD_DEF void sb_init(stringb *sb, size_t initial_cap);
NONSTD_DEF void sb_free(stringb *sb);
NONSTD_DEF void sb_ensure(stringb *sb, size_t additional);
NONSTD_DEF void sb_append_cstr(stringb *sb, const char *s);
NONSTD_DEF void sb_append_sv(stringb *sb, stringv sv);
NONSTD_DEF void sb_append_char(stringb *sb, char c);
NONSTD_DEF stringv sb_as_sv(const stringb *sb);

// Slice - generic non-owning view into an array
// Usage:
//   SLICE_DEF(int);        // Define slice_int type
//   slice(int) view = ...; // Use it
#define SLICE_DEF(T)   \
	typedef struct {   \
		T *data;       \
		size_t length; \
	} slice_##T

#define slice(T) slice_##T

#define make_slice(T, ptr, len) ((slice(T)){.data = (ptr), .length = (len)})

#define array_as_slice(T, arr) \
	((slice(T)){.data = (arr).data, .length = (arr).length})

// Dynamic array - generic type-safe growable array using macros
// Usage: array(int) numbers; array_init(numbers);
#define array(T)         \
	struct {             \
		T *data;         \
		size_t length;   \
		size_t capacity; \
	}

#define array_init(arr)     \
	do {                    \
		(arr).capacity = 0; \
		(arr).data = NULL;  \
		(arr).length = 0;   \
	} while (0)

#define array_init_cap(arr, initial_cap)                             \
	do {                                                             \
		(arr).capacity = (initial_cap) ? (initial_cap) : 16;         \
		(arr).data = ALLOC(__typeof__(*(arr).data), (arr).capacity); \
		(arr).length = 0;                                            \
	} while (0)

#define array_free(arr)     \
	do {                    \
		FREE((arr).data);   \
		(arr).length = 0;   \
		(arr).capacity = 0; \
	} while (0)

#define array_ensure(arr, additional)                                   \
	do {                                                                \
		size_t _needed = (arr).length + (additional);                   \
		if (_needed > (arr).capacity) {                                 \
			size_t _new_cap = (arr).capacity ? (arr).capacity : 16;     \
			while (_new_cap < _needed) {                                \
				_new_cap *= 2;                                          \
			}                                                           \
			(arr).data =                                                \
				REALLOC((arr).data, __typeof__(*(arr).data), _new_cap); \
			(arr).capacity = _new_cap;                                  \
		}                                                               \
	} while (0)

#define array_push(arr, value)                \
	do {                                      \
		array_ensure((arr), 1);               \
		(arr).data[(arr).length++] = (value); \
	} while (0)

#define array_pop(arr) ((arr).length > 0 ? (arr).data[--(arr).length] : 0)

#define array_get(arr, index) ((arr).data[index])

#define array_set(arr, index, value)     \
	do {                                 \
		if ((index) < (arr).length) {    \
			(arr).data[index] = (value); \
		}                                \
	} while (0)

#define array_insert(arr, index, value)                          \
	do {                                                         \
		if ((index) <= (arr).length) {                           \
			array_ensure((arr), 1);                              \
			for (size_t _i = (arr).length; _i > (index); --_i) { \
				(arr).data[_i] = (arr).data[_i - 1];             \
			}                                                    \
			(arr).data[index] = (value);                         \
			(arr).length++;                                      \
		}                                                        \
	} while (0)

#define array_remove(arr, index)                                     \
	do {                                                             \
		if ((index) < (arr).length) {                                \
			for (size_t _i = (index); _i < (arr).length - 1; ++_i) { \
				(arr).data[_i] = (arr).data[_i + 1];                 \
			}                                                        \
			(arr).length--;                                          \
		}                                                            \
	} while (0)

#define array_clear(arr)  \
	do {                  \
		(arr).length = 0; \
	} while (0)

#define array_reserve(arr, new_capacity)                                      \
	do {                                                                      \
		if ((new_capacity) > (arr).capacity) {                                \
			(arr).data =                                                      \
				REALLOC((arr).data, __typeof__(*(arr).data), (new_capacity)); \
			(arr).capacity = (new_capacity);                                  \
		}                                                                     \
	} while (0)

#define array_foreach(arr, var)                                        \
	for (size_t _i_##var = 0;                                          \
		 _i_##var < (arr).length && ((var) = (arr).data[_i_##var], 1); \
		 ++_i_##var)

#define array_foreach_idx(arr, var, index) \
	for (size_t index = 0;               \
		 index < (arr).length && ((var) = (arr).data[index], 1); ++index)

// Arena - block-based memory allocator
// Usage: Arena a = arena_make(); void* p = arena_alloc(&a, 100);
// arena_free(&a);
typedef struct {
	char *ptr;
	char *end;
	array(char *) blocks;
} Arena;

#define ARENA_DEFAULT_BLOCK_SIZE (4096)

NONSTD_DEF Arena arena_make(void);
NONSTD_DEF void arena_grow(Arena *a, size_t min_size);
NONSTD_DEF void *arena_alloc(Arena *a, size_t size);
NONSTD_DEF void arena_free(Arena *a);

// File I/O helpers
NONSTD_DEF char *read_entire_file(const char *filepath, size_t *out_size);
NONSTD_DEF int write_entire_file(const char *filepath, const void *data, size_t size);
NONSTD_DEF stringv read_entire_file_sv(const char *filepath);
NONSTD_DEF stringb read_entire_file_sb(const char *filepath);
NONSTD_DEF int write_file_sv(const char *filepath, stringv sv);
NONSTD_DEF int write_file_sb(const char *filepath, const stringb *sb);

#endif // NONSTD_H

#ifdef NONSTD_IMPLEMENTATION

NONSTD_DEF stringv sv_from_cstr(const char *s) {
	return (stringv){.data = s, .length = s ? strlen(s) : 0};
}

NONSTD_DEF stringv sv_from_parts(const char *data, size_t length) {
	return (stringv){.data = data, .length = length};
}

NONSTD_DEF stringv sv_slice(stringv sv, size_t start, size_t end) {
	if (start > sv.length) {
		start = sv.length;
	}
	if (end > sv.length) {
		end = sv.length;
	}
	if (start > end) {
		start = end;
	}
	return (stringv){.data = sv.data + start, .length = end - start};
}

NONSTD_DEF int sv_equals(stringv a, stringv b) {
	return a.length == b.length && (a.length == 0 || memcmp(a.data, b.data, a.length) == 0);
}

NONSTD_DEF int sv_starts_with(stringv sv, stringv prefix) {
	return sv.length >= prefix.length && memcmp(sv.data, prefix.data, prefix.length) == 0;
}

NONSTD_DEF int sv_ends_with(stringv sv, stringv suffix) {
	return sv.length >= suffix.length && memcmp(sv.data + sv.length - suffix.length, suffix.data, suffix.length) == 0;
}

NONSTD_DEF void sb_init(stringb *sb, size_t initial_cap) {
	sb->capacity = initial_cap ? initial_cap : 16;
	sb->data = ALLOC(char, sb->capacity);
	sb->length = 0;
	if (sb->data) {
		sb->data[0] = '\0';
	}
}

NONSTD_DEF void sb_free(stringb *sb) {
	FREE(sb->data);
	sb->length = 0;
	sb->capacity = 0;
}

NONSTD_DEF void sb_ensure(stringb *sb, size_t additional) {
	size_t needed = sb->length + additional + 1;
	if (needed > sb->capacity) {
		while (sb->capacity < needed) {
			sb->capacity *= 2;
		}
		sb->data = REALLOC(sb->data, char, sb->capacity);
	}
}

NONSTD_DEF void sb_append_cstr(stringb *sb, const char *s) {
	if (!s) {
		return;
	}
	size_t slength = strlen(s);
	sb_ensure(sb, slength);
	memcpy(sb->data + sb->length, s, slength);
	sb->length += slength;
	sb->data[sb->length] = '\0';
}

NONSTD_DEF void sb_append_sv(stringb *sb, stringv sv) {
	if (!sv.data || sv.length == 0) {
		return;
	}
	sb_ensure(sb, sv.length);
	memcpy(sb->data + sb->length, sv.data, sv.length);
	sb->length += sv.length;
	sb->data[sb->length] = '\0';
}

NONSTD_DEF void sb_append_char(stringb *sb, char c) {
	sb_ensure(sb, 1);
	sb->data[sb->length++] = c;
	sb->data[sb->length] = '\0';
}

NONSTD_DEF stringv sb_as_sv(const stringb *sb) {
	return (stringv){.data = sb->data, .length = sb->length};
}

NONSTD_DEF Arena arena_make(void) {
	Arena a = {0};
	array_init(a.blocks);
	return a;
}

NONSTD_DEF void arena_grow(Arena *a, size_t min_size) {
	size_t size = MAX(ARENA_DEFAULT_BLOCK_SIZE, min_size);
	char *block = ALLOC(char, size);
	a->ptr = block;
	a->end = block + size;
	array_push(a->blocks, block);
}

NONSTD_DEF void *arena_alloc(Arena *a, size_t size) {
	// Align to 8 bytes basically
	size_t align = sizeof(void *);
	uintptr_t current = (uintptr_t)a->ptr;
	uintptr_t aligned = (current + align - 1) & ~(align - 1);
	uintptr_t available = (uintptr_t)a->end - aligned;

	if (available < size) {
		arena_grow(a, size);
		current = (uintptr_t)a->ptr;
		aligned = (current + align - 1) & ~(align - 1);
	}

	a->ptr = (char *)(aligned + size);
	return (void *)aligned;
}

NONSTD_DEF void arena_free(Arena *a) {
	char *block;
	array_foreach(a->blocks, block) { FREE(block); }
	array_free(a->blocks);
	a->ptr = NULL;
	a->end = NULL;
}

NONSTD_DEF char *read_entire_file(const char *filepath, size_t *out_size) {
	FILE *f = fopen(filepath, "rb");
	if (!f) {
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (length < 0) {
		fclose(f);
		return NULL;
	}

	size_t size = (size_t)length;
	char *buffer = ALLOC(char, size + 1);
	if (!buffer) {
		fclose(f);
		return NULL;
	}

	size_t read = fread(buffer, 1, size, f);
	fclose(f);

	if (read != size) {
		FREE(buffer);
		return NULL;
	}

	buffer[size] = '\0';
	if (out_size) {
		*out_size = size;
	}

	return buffer;
}

NONSTD_DEF int write_entire_file(const char *filepath, const void *data, size_t size) {
	FILE *f = fopen(filepath, "wb");
	if (!f) {
		return 0;
	}

	size_t written = fwrite(data, 1, size, f);
	fclose(f);

	return written == size;
}

NONSTD_DEF stringv read_entire_file_sv(const char *filepath) {
	size_t size = 0;
	char *data = read_entire_file(filepath, &size);
	if (!data) {
		return (stringv){0};
	}
	return (stringv){.data = data, .length = size};
}

NONSTD_DEF stringb read_entire_file_sb(const char *filepath) {
	size_t size = 0;
	char *data = read_entire_file(filepath, &size);
	stringb sb = {0};
	if (data) {
		sb.data = data;
		sb.length = size;
		sb.capacity = size + 1;
	}
	return sb;
}

NONSTD_DEF int write_file_sv(const char *filepath, stringv sv) {
	return write_entire_file(filepath, sv.data, sv.length);
}

NONSTD_DEF int write_file_sb(const char *filepath, const stringb *sb) {
	return write_entire_file(filepath, sb->data, sb->length);
}

#endif // NONSTD_IMPLEMENTATION