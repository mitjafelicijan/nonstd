#define _POSIX_C_SOURCE 200112L
#include "minunit.h"

#define NONSTD_IMPLEMENTATION
#include "nonstd.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// Custom test runner that prints test names
#define RUN_TEST_WITH_NAME(test)                   \
	do {                                           \
		printf("  %-50s ", #test);                 \
		fflush(stdout);                            \
		int before_fail = minunit_fail;            \
		int saved_stdout = dup(STDOUT_FILENO);     \
		int devnull = open("/dev/null", O_WRONLY); \
		dup2(devnull, STDOUT_FILENO);              \
		close(devnull);                            \
		MU_RUN_TEST(test);                         \
		fflush(stdout);                            \
		dup2(saved_stdout, STDOUT_FILENO);         \
		close(saved_stdout);                       \
		if (minunit_fail == before_fail) {         \
			printf("PASS\n");                      \
		}                                          \
	} while (0)

// String view tests
MU_TEST(test_sv_from_cstr) {
	stringv sv = sv_from_cstr("hello");
	mu_assert_int_eq(5, sv.length);
	mu_check(sv.data != NULL);
	mu_check(strncmp(sv.data, "hello", 5) == 0);
}

MU_TEST(test_sv_from_cstr_null) {
	stringv sv = sv_from_cstr(NULL);
	mu_assert_int_eq(0, sv.length);
}

MU_TEST(test_sv_from_parts) {
	const char *str = "hello world";
	stringv sv = sv_from_parts(str, 5);
	mu_assert_int_eq(5, sv.length);
	mu_check(sv.data == str);
}

MU_TEST(test_sv_from_cstr_literal) {
	stringv sv = sv_from_cstr("test");
	mu_assert_int_eq(4, sv.length);
	mu_check(strncmp(sv.data, "test", 4) == 0);
}

MU_TEST(test_sv_slice_normal) {
	stringv sv = sv_from_cstr("hello world");
	stringv sliced = sv_slice(sv, 0, 5);
	mu_assert_int_eq(5, sliced.length);
	mu_check(strncmp(sliced.data, "hello", 5) == 0);
}

MU_TEST(test_sv_slice_middle) {
	stringv sv = sv_from_cstr("hello world");
	stringv sliced = sv_slice(sv, 6, 11);
	mu_assert_int_eq(5, sliced.length);
	mu_check(strncmp(sliced.data, "world", 5) == 0);
}

MU_TEST(test_sv_slice_out_of_bounds) {
	stringv sv = sv_from_cstr("hello");
	stringv sliced = sv_slice(sv, 0, 100);
	mu_assert_int_eq(5, sliced.length);
}

MU_TEST(test_sv_slice_invalid_range) {
	stringv sv = sv_from_cstr("hello");
	stringv sliced = sv_slice(sv, 10, 5);
	mu_assert_int_eq(0, sliced.length);
}

MU_TEST(test_sv_equals_same) {
	stringv a = sv_from_cstr("hello");
	stringv b = sv_from_cstr("hello");
	mu_check(sv_equals(a, b));
}

MU_TEST(test_sv_equals_different) {
	stringv a = sv_from_cstr("hello");
	stringv b = sv_from_cstr("world");
	mu_check(!sv_equals(a, b));
}

MU_TEST(test_sv_equals_different_length) {
	stringv a = sv_from_cstr("hello");
	stringv b = sv_from_cstr("hi");
	mu_check(!sv_equals(a, b));
}

MU_TEST(test_sv_equals_empty) {
	stringv a = sv_from_parts(NULL, 0);
	stringv b = sv_from_parts(NULL, 0);
	mu_check(sv_equals(a, b));
}

MU_TEST(test_sv_starts_with_true) {
	stringv sv = sv_from_cstr("hello world");
	stringv prefix = sv_from_cstr("hello");
	mu_check(sv_starts_with(sv, prefix));
}

MU_TEST(test_sv_starts_with_false) {
	stringv sv = sv_from_cstr("hello world");
	stringv prefix = sv_from_cstr("world");
	mu_check(!sv_starts_with(sv, prefix));
}

MU_TEST(test_sv_starts_with_empty_prefix) {
	stringv sv = sv_from_cstr("hello");
	stringv prefix = sv_from_parts(NULL, 0);
	mu_check(sv_starts_with(sv, prefix));
}

MU_TEST(test_sv_starts_with_longer_prefix) {
	stringv sv = sv_from_cstr("hi");
	stringv prefix = sv_from_cstr("hello");
	mu_check(!sv_starts_with(sv, prefix));
}

MU_TEST(test_sv_ends_with_true) {
	stringv sv = sv_from_cstr("hello world");
	stringv suffix = sv_from_cstr("world");
	mu_check(sv_ends_with(sv, suffix));
}

MU_TEST(test_sv_ends_with_false) {
	stringv sv = sv_from_cstr("hello world");
	stringv suffix = sv_from_cstr("hello");
	mu_check(!sv_ends_with(sv, suffix));
}

MU_TEST(test_sv_ends_with_empty_suffix) {
	stringv sv = sv_from_cstr("hello");
	stringv suffix = sv_from_parts(NULL, 0);
	mu_check(sv_ends_with(sv, suffix));
}

MU_TEST(test_sv_ends_with_longer_suffix) {
	stringv sv = sv_from_cstr("hi");
	stringv suffix = sv_from_cstr("world");
	mu_check(!sv_ends_with(sv, suffix));
}

// Macro tests
MU_TEST(test_countof) {
	int array[10];
	mu_assert_int_eq(10, countof(array));

	char str[] = "hello";
	mu_assert_int_eq(6, countof(str)); // includes null terminator
}

MU_TEST(test_min_max_clamp) {
	mu_assert_int_eq(5, MIN(5, 10));
	mu_assert_int_eq(5, MIN(10, 5));

	mu_assert_int_eq(10, MAX(5, 10));
	mu_assert_int_eq(10, MAX(10, 5));

	mu_assert_int_eq(5, CLAMP(3, 5, 10));
	mu_assert_int_eq(7, CLAMP(7, 5, 10));
	mu_assert_int_eq(10, CLAMP(15, 5, 10));
}

MU_TEST(test_static_foreach_macro) {
	int values[] = {1, 2, 3, 4, 5};
	int sum = 0;
	int val;

	static_foreach(int, val, values) { sum += val; }

	mu_assert_int_eq(15, sum);
}

MU_TEST(test_static_foreach_with_strings) {
	const char *words[] = {"hello", "world", "test"};
	int count = 0;
	const char *word;

	static_foreach(const char *, word, words) {
		count++;
		mu_check(word != NULL);
	}

	mu_assert_int_eq(3, count);
}

MU_TEST(test_static_foreach_single_element) {
	int single[] = {42};
	int result = 0;
	int val;

	static_foreach(int, val, single) { result = val; }

	mu_assert_int_eq(42, result);
}

MU_TEST(test_static_foreach_modify_counter) {
	u32 numbers[] = {10, 20, 30, 40};
	u32 max = 0;
	u32 num;

	static_foreach(u32, num, numbers) {
		if (num > max)
			max = num;
	}

	mu_assert_int_eq(40, max);
}

MU_TEST(test_static_foreach_with_structs) {
	struct Point {
		int x, y;
	};

	struct Point pts[] = {{1, 2}, {3, 4}, {5, 6}};
	int sum_x = 0;
	int sum_y = 0;
	struct Point p;

	static_foreach(struct Point, p, pts) {
		sum_x += p.x;
		sum_y += p.y;
	}

	mu_assert_int_eq(9, sum_x);	 // 1 + 3 + 5
	mu_assert_int_eq(12, sum_y); // 2 + 4 + 6
}

MU_TEST(test_alloc_and_free) {
	int *ptr = ALLOC(int, 10);
	mu_check(ptr != NULL);

	// Use the memory
	ptr[0] = 42;
	ptr[9] = 99;
	mu_assert_int_eq(42, ptr[0]);
	mu_assert_int_eq(99, ptr[9]);

	FREE(ptr);
	mu_check(ptr == NULL);
}

MU_TEST(test_realloc) {
	int *ptr = ALLOC(int, 5);
	mu_check(ptr != NULL);

	ptr[0] = 1;
	ptr[4] = 5;

	ptr = REALLOC(ptr, int, 10);
	mu_check(ptr != NULL);

	mu_assert_int_eq(1, ptr[0]);
	mu_assert_int_eq(5, ptr[4]);
	ptr[9] = 99;
	mu_assert_int_eq(99, ptr[9]);

	free(ptr);
}

MU_TEST(test_typedefs) {
	// Just verify the types compile and have expected sizes
	i8 a = -1;
	u8 b = 255;
	i16 c = -1000;
	u16 d = 60000;
	i32 e = -100000;
	u32 f = 4000000000U;
	i64 g = -1000000000LL;
	u64 h = 10000000000ULL;

	mu_check(a == -1);
	mu_check(b == 255);
	mu_check(c == -1000);
	mu_check(d == 60000);
	mu_check(e == -100000);
	mu_check(f == 4000000000U);
	mu_check(g == -1000000000LL);
	mu_check(h == 10000000000ULL);
}

// String builder tests
MU_TEST(test_sb_init) {
	stringb sb;
	sb_init(&sb, 0);
	mu_check(sb.data != NULL);
	mu_assert_int_eq(0, sb.length);
	mu_check(sb.capacity >= 16); // default capacity
	mu_assert_int_eq('\0', sb.data[0]);
	sb_free(&sb);
}

MU_TEST(test_sb_init_with_capacity) {
	stringb sb;
	sb_init(&sb, 64);
	mu_check(sb.data != NULL);
	mu_assert_int_eq(0, sb.length);
	mu_assert_int_eq(64, sb.capacity);
	sb_free(&sb);
}

MU_TEST(test_sb_append_cstr) {
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "hello");
	mu_assert_int_eq(5, sb.length);
	mu_check(strcmp(sb.data, "hello") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_append_cstr_multiple) {
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "hello");
	sb_append_cstr(&sb, " ");
	sb_append_cstr(&sb, "world");
	mu_assert_int_eq(11, sb.length);
	mu_check(strcmp(sb.data, "hello world") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_append_cstr_null) {
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "test");
	sb_append_cstr(&sb, NULL);
	mu_assert_int_eq(4, sb.length);
	mu_check(strcmp(sb.data, "test") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_append_sv) {
	stringb sb;
	sb_init(&sb, 0);
	stringv sv = sv_from_cstr("hello");
	sb_append_sv(&sb, sv);
	mu_assert_int_eq(5, sb.length);
	mu_check(strcmp(sb.data, "hello") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_append_sv_slice) {
	stringb sb;
	sb_init(&sb, 0);
	stringv full = sv_from_cstr("hello world");
	stringv slice = sv_slice(full, 6, 11);
	sb_append_sv(&sb, slice);
	mu_assert_int_eq(5, sb.length);
	mu_check(strcmp(sb.data, "world") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_append_sv_empty) {
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "test");
	stringv empty = sv_from_parts(NULL, 0);
	sb_append_sv(&sb, empty);
	mu_assert_int_eq(4, sb.length);
	mu_check(strcmp(sb.data, "test") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_append_char) {
	stringb sb;
	sb_init(&sb, 0);
	sb_append_char(&sb, 'a');
	sb_append_char(&sb, 'b');
	sb_append_char(&sb, 'c');
	mu_assert_int_eq(3, sb.length);
	mu_check(strcmp(sb.data, "abc") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_mixed_append) {
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "Hello");
	sb_append_char(&sb, ' ');
	stringv sv = sv_from_cstr("beautiful");
	sb_append_sv(&sb, sv);
	sb_append_char(&sb, ' ');
	sb_append_cstr(&sb, "world!");
	mu_assert_int_eq(22, sb.length);
	mu_check(strcmp(sb.data, "Hello beautiful world!") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_as_sv) {
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "test string");
	stringv sv = sb_as_sv(&sb);
	mu_assert_int_eq(11, sv.length);
	mu_check(sv.data == sb.data);
	mu_check(strncmp(sv.data, "test string", sv.length) == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_growth) {
	stringb sb;
	sb_init(&sb, 4); // very small initial capacity
	mu_assert_int_eq(4, sb.capacity);

	// Append enough to trigger growth
	sb_append_cstr(&sb, "this is a long string that will exceed initial capacity");
	mu_check(sb.capacity > 4); // capacity should have grown
	mu_check(sb.length == strlen("this is a long string that will exceed initial capacity"));
	mu_check(strcmp(sb.data, "this is a long string that will exceed initial capacity") == 0);
	sb_free(&sb);
}

MU_TEST(test_sb_free) {
	stringb sb;
	sb_init(&sb, 32);
	sb_append_cstr(&sb, "test");
	sb_free(&sb);
	mu_check(sb.data == NULL);
	mu_assert_int_eq(0, sb.length);
	mu_assert_int_eq(0, sb.capacity);
}

MU_TEST(test_sb_sv_interop) {
	// Test that sb can be used to build a string, then viewed with sv
	stringb sb;
	sb_init(&sb, 0);

	const char *words[] = {"one", "two", "three"};
	const char *word;
	static_foreach(const char *, word, words) {
		if (sb.length > 0) {
			sb_append_char(&sb, ',');
		}
		sb_append_cstr(&sb, word);
	}

	stringv result = sb_as_sv(&sb);
	mu_check(sv_equals(result, sv_from_cstr("one,two,three")));

	// Can use sv operations on the builder's content
	mu_check(sv_starts_with(result, sv_from_cstr("one")));
	mu_check(sv_ends_with(result, sv_from_cstr("three")));

	sb_free(&sb);
}

MU_TEST(test_sb_append_sv_from_sb) {
	// Test appending a string view of one builder to another
	stringb sb1, sb2;
	sb_init(&sb1, 0);
	sb_init(&sb2, 0);

	sb_append_cstr(&sb1, "hello");
	sb_append_cstr(&sb2, "world");

	stringv sv1 = sb_as_sv(&sb1);
	sb_append_char(&sb2, ' ');
	sb_append_sv(&sb2, sv1);

	mu_check(strcmp(sb2.data, "world hello") == 0);

	sb_free(&sb1);
	sb_free(&sb2);
}

// Array tests
MU_TEST(test_array_init) {
	array(int) arr;
	array_init(arr);
	mu_check(arr.data == NULL);
	mu_assert_int_eq(0, arr.length);
	mu_assert_int_eq(0, arr.capacity);
	array_free(arr);
}

MU_TEST(test_array_init_cap) {
	array(int) arr;
	array_init_cap(arr, 32);
	mu_check(arr.data != NULL);
	mu_assert_int_eq(0, arr.length);
	mu_assert_int_eq(32, arr.capacity);
	array_free(arr);
}

MU_TEST(test_array_push) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 10);
	array_push(arr, 20);
	array_push(arr, 30);

	mu_assert_int_eq(3, arr.length);
	mu_check(arr.capacity >= 3);
	mu_assert_int_eq(10, arr.data[0]);
	mu_assert_int_eq(20, arr.data[1]);
	mu_assert_int_eq(30, arr.data[2]);

	array_free(arr);
}

MU_TEST(test_array_pop) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 10);
	array_push(arr, 20);

	int val = array_pop(arr);
	mu_assert_int_eq(20, val);
	mu_assert_int_eq(1, arr.length);

	val = array_pop(arr);
	mu_assert_int_eq(10, val);
	mu_assert_int_eq(0, arr.length);

	array_free(arr);
}

MU_TEST(test_array_pop_empty) {
	array(int) arr;
	array_init(arr);
	int val = array_pop(arr);
	mu_assert_int_eq(0, val);
	mu_assert_int_eq(0, arr.length);
	array_free(arr);
}

MU_TEST(test_array_get_set) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 10);
	array_push(arr, 20);

	mu_assert_int_eq(10, array_get(arr, 0));
	mu_assert_int_eq(20, array_get(arr, 1));

	array_set(arr, 0, 100);
	mu_assert_int_eq(100, array_get(arr, 0));

	// Test out of bounds set (should do nothing)
	array_set(arr, 5, 500);
	mu_assert_int_eq(2, arr.length);

	array_free(arr);
}

MU_TEST(test_array_insert) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 10);
	array_push(arr, 30);

	array_insert(arr, 1, 20); // Insert between 10 and 30
	mu_assert_int_eq(3, arr.length);
	mu_assert_int_eq(10, arr.data[0]);
	mu_assert_int_eq(20, arr.data[1]);
	mu_assert_int_eq(30, arr.data[2]);

	array_insert(arr, 0, 5); // Insert at start
	mu_assert_int_eq(5, arr.data[0]);
	mu_assert_int_eq(10, arr.data[1]);

	array_insert(arr, 4, 40); // Insert at end
	mu_assert_int_eq(40, arr.data[4]);
	mu_assert_int_eq(5, arr.length);

	array_free(arr);
}

MU_TEST(test_array_remove) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 10);
	array_push(arr, 20);
	array_push(arr, 30);
	array_push(arr, 40);

	array_remove(arr, 1); // Remove 20
	mu_assert_int_eq(3, arr.length);
	mu_assert_int_eq(10, arr.data[0]);
	mu_assert_int_eq(30, arr.data[1]);
	mu_assert_int_eq(40, arr.data[2]);

	array_remove(arr, 0); // Remove 10 (start)
	mu_assert_int_eq(30, arr.data[0]);

	array_remove(arr, 1); // Remove 40 (end)
	mu_assert_int_eq(30, arr.data[0]);
	mu_assert_int_eq(1, arr.length);

	array_free(arr);
}

MU_TEST(test_array_growth) {
	array(int) arr;
	array_init_cap(arr, 4);

	for (int i = 0; i < 20; i++) {
		array_push(arr, i);
	}

	mu_assert_int_eq(20, arr.length);
	mu_check(arr.capacity >= 20);
	mu_check(arr.capacity > 4);

	for (int i = 0; i < 20; i++) {
		mu_assert_int_eq(i, arr.data[i]);
	}

	array_free(arr);
}

MU_TEST(test_array_reserve) {
	array(int) arr;
	array_init(arr);

	array_reserve(arr, 100);
	mu_assert_int_eq(100, arr.capacity);
	mu_assert_int_eq(0, arr.length);
	mu_check(arr.data != NULL);

	array_free(arr);
}

MU_TEST(test_array_clear) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 1);
	array_push(arr, 2);

	size_t cap = arr.capacity;
	array_clear(arr);

	mu_assert_int_eq(0, arr.length);
	mu_assert_int_eq(cap, arr.capacity); // Capacity stays
	mu_check(arr.data != NULL);

	array_free(arr);
}

MU_TEST(test_array_foreach) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 1);
	array_push(arr, 2);
	array_push(arr, 3);

	int sum = 0;
	int val;
	array_foreach(arr, val) { sum += val; }

	mu_assert_int_eq(6, sum);
	array_free(arr);
}

MU_TEST(test_array_foreach_idx) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 10);
	array_push(arr, 20);
	array_push(arr, 30);

	int sum_val = 0;
	size_t sum_idx = 0;
	int val;

	array_foreach_idx(arr, val, i) {
		sum_val += val;
		sum_idx += i;
		mu_assert_int_eq(val, arr.data[i]);
	}

	mu_assert_int_eq(60, sum_val);
	mu_assert_int_eq(3, sum_idx); // 0+1+2
	array_free(arr);
}

// Slice Tests
SLICE_DEF(int);

MU_TEST(test_slice_make) {
	int nums[] = {1, 2, 3, 4, 5};
	slice(int) s = make_slice(int, nums, 5);

	mu_assert_int_eq(5, s.length);
	mu_check(s.data == nums);
	mu_assert_int_eq(1, s.data[0]);
	mu_assert_int_eq(5, s.data[4]);
}

MU_TEST(test_slice_make_partial) {
	int nums[] = {10, 20, 30, 40, 50};
	slice(int) s = make_slice(int, nums + 1, 3); // 20, 30, 40

	mu_assert_int_eq(3, s.length);
	mu_assert_int_eq(20, s.data[0]);
	mu_assert_int_eq(40, s.data[2]);
}

MU_TEST(test_slice_from_dynamic_array) {
	array(int) arr;
	array_init(arr);
	array_push(arr, 100);
	array_push(arr, 200);

	slice(int) s = array_as_slice(int, arr);

	mu_assert_int_eq(2, s.length);
	mu_check(s.data == arr.data);
	mu_assert_int_eq(100, s.data[0]);

	array_free(arr);
}

MU_TEST(test_slice_modification) {
	int nums[] = {1, 1, 1};
	slice(int) s = make_slice(int, nums, 3);

	s.data[1] = 99;

	mu_assert_int_eq(99, nums[1]);
	mu_assert_int_eq(1, nums[0]);
}

// Arena tests
MU_TEST(test_arena_basic) {
	Arena a = arena_make();
	void *p1 = arena_alloc(&a, 10);
	void *p2 = arena_alloc(&a, 20);

	mu_check(p1 != NULL);
	mu_check(p2 != NULL);
	mu_check(p1 != p2);

	// Basic usage check
	memset(p1, 1, 10);
	memset(p2, 2, 20);

	arena_free(&a);
}

MU_TEST(test_arena_growth) {
	Arena a = arena_make();

	// Force growth by allocating more than default block size (4096)
	// or by allocating many small objects.
	// Let's alloc a big chunk first.
	void *big = arena_alloc(&a, 5000);
	mu_check(big != NULL);
	mu_check(a.blocks.length >= 1);

	// Alloc another big chunk
	void *big2 = arena_alloc(&a, 5000);
	mu_check(big2 != NULL);
	mu_check(big2 != big);
	mu_check(a.blocks.length >= 2);

	arena_free(&a);
}

MU_TEST(test_arena_alignment) {
	Arena a = arena_make();

	void *p1 = arena_alloc(&a, 1); // 1 byte
	void *p2 = arena_alloc(&a, 1); // 1 byte

	uintptr_t addr1 = (uintptr_t)p1;
	uintptr_t addr2 = (uintptr_t)p2;

	mu_check(addr1 % sizeof(void *) == 0);
	mu_check(addr2 % sizeof(void *) == 0);
	mu_check(addr2 - addr1 >= sizeof(void *));

	arena_free(&a);
}

// File I/O tests
MU_TEST(test_file_io_basic) {
	const char *filename = "test_io_basic.txt";
	const char *content = "Hello, file!";
	size_t len = strlen(content);

	// Write
	mu_check(write_entire_file(filename, content, len));

	// Read
	size_t read_len;
	char *read_content = read_entire_file(filename, &read_len);
	mu_check(read_content != NULL);
	mu_assert_int_eq(len, read_len);
	mu_check(strncmp(content, read_content, len) == 0);

	FREE(read_content);
	remove(filename);
}

MU_TEST(test_file_io_sv) {
	const char *filename = "test_io_sv.txt";
	stringv sv = sv_from_cstr("Hello from sv!");

	// Write
	mu_check(write_file_sv(filename, sv));

	// Read
	stringv read_sv = read_entire_file_sv(filename);
	mu_check(read_sv.data != NULL);
	mu_assert_int_eq(sv.length, read_sv.length);
	mu_check(sv_equals(sv, read_sv));

	// The data returned by read_entire_file_sv is malloc'd and needs freeing
	// We cast away const to free it since we know it was allocated
	free((char *)read_sv.data);
	remove(filename);
}

MU_TEST(test_file_io_sb) {
	const char *filename = "test_io_sb.txt";
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "Hello from sb!");

	// Write
	mu_check(write_file_sb(filename, &sb));

	// Read
	stringb read_sb = read_entire_file_sb(filename);
	mu_check(read_sb.data != NULL);
	mu_assert_int_eq(sb.length, read_sb.length);
	mu_check(strcmp(sb.data, read_sb.data) == 0);

	sb_free(&sb);
	sb_free(&read_sb);
	remove(filename);
}

MU_TEST(test_file_io_read_missing) {
	size_t len;
	char *content = read_entire_file("non_existent_file.txt", &len);
	mu_check(content == NULL);
}

MU_TEST(test_file_io_read_missing_sv) {
	stringv sv = read_entire_file_sv("non_existent_file.txt");
	mu_check(sv.data == NULL);
	mu_assert_int_eq(0, sv.length);
}

MU_TEST(test_file_io_read_missing_sb) {
	stringb sb = read_entire_file_sb("non_existent_file.txt");
	mu_check(sb.data == NULL);
	mu_assert_int_eq(0, sb.length);
}

// Test suites
MU_TEST_SUITE(test_suite_stringv) {
	printf("\n[String View Tests]\n");
	RUN_TEST_WITH_NAME(test_sv_from_cstr);
	RUN_TEST_WITH_NAME(test_sv_from_cstr_null);
	RUN_TEST_WITH_NAME(test_sv_from_parts);
	RUN_TEST_WITH_NAME(test_sv_from_cstr_literal);
	RUN_TEST_WITH_NAME(test_sv_slice_normal);
	RUN_TEST_WITH_NAME(test_sv_slice_middle);
	RUN_TEST_WITH_NAME(test_sv_slice_out_of_bounds);
	RUN_TEST_WITH_NAME(test_sv_slice_invalid_range);
	RUN_TEST_WITH_NAME(test_sv_equals_same);
	RUN_TEST_WITH_NAME(test_sv_equals_different);
	RUN_TEST_WITH_NAME(test_sv_equals_different_length);
	RUN_TEST_WITH_NAME(test_sv_equals_empty);
	RUN_TEST_WITH_NAME(test_sv_starts_with_true);
	RUN_TEST_WITH_NAME(test_sv_starts_with_false);
	RUN_TEST_WITH_NAME(test_sv_starts_with_empty_prefix);
	RUN_TEST_WITH_NAME(test_sv_starts_with_longer_prefix);
	RUN_TEST_WITH_NAME(test_sv_ends_with_true);
	RUN_TEST_WITH_NAME(test_sv_ends_with_false);
	RUN_TEST_WITH_NAME(test_sv_ends_with_empty_suffix);
	RUN_TEST_WITH_NAME(test_sv_ends_with_longer_suffix);
}

MU_TEST_SUITE(test_suite_stringb) {
	printf("\n[String Builder Tests]\n");
	RUN_TEST_WITH_NAME(test_sb_init);
	RUN_TEST_WITH_NAME(test_sb_init_with_capacity);
	RUN_TEST_WITH_NAME(test_sb_append_cstr);
	RUN_TEST_WITH_NAME(test_sb_append_cstr_multiple);
	RUN_TEST_WITH_NAME(test_sb_append_cstr_null);
	RUN_TEST_WITH_NAME(test_sb_append_sv);
	RUN_TEST_WITH_NAME(test_sb_append_sv_slice);
	RUN_TEST_WITH_NAME(test_sb_append_sv_empty);
	RUN_TEST_WITH_NAME(test_sb_append_char);
	RUN_TEST_WITH_NAME(test_sb_mixed_append);
	RUN_TEST_WITH_NAME(test_sb_as_sv);
	RUN_TEST_WITH_NAME(test_sb_growth);
	RUN_TEST_WITH_NAME(test_sb_free);
	RUN_TEST_WITH_NAME(test_sb_sv_interop);
	RUN_TEST_WITH_NAME(test_sb_append_sv_from_sb);
}

MU_TEST_SUITE(test_suite_macros) {
	printf("\n[Macro Tests]\n");
	RUN_TEST_WITH_NAME(test_countof);
	RUN_TEST_WITH_NAME(test_min_max_clamp);
	RUN_TEST_WITH_NAME(test_static_foreach_macro);
	RUN_TEST_WITH_NAME(test_static_foreach_with_strings);
	RUN_TEST_WITH_NAME(test_static_foreach_single_element);
	RUN_TEST_WITH_NAME(test_static_foreach_modify_counter);
	RUN_TEST_WITH_NAME(test_static_foreach_with_structs);
	RUN_TEST_WITH_NAME(test_alloc_and_free);
	RUN_TEST_WITH_NAME(test_realloc);
}

MU_TEST_SUITE(test_suite_array) {
	printf("\n[Array Tests]\n");
	RUN_TEST_WITH_NAME(test_array_init);
	RUN_TEST_WITH_NAME(test_array_init_cap);
	RUN_TEST_WITH_NAME(test_array_push);
	RUN_TEST_WITH_NAME(test_array_pop);
	RUN_TEST_WITH_NAME(test_array_pop_empty);
	RUN_TEST_WITH_NAME(test_array_get_set);
	RUN_TEST_WITH_NAME(test_array_insert);
	RUN_TEST_WITH_NAME(test_array_remove);
	RUN_TEST_WITH_NAME(test_array_growth);
	RUN_TEST_WITH_NAME(test_array_reserve);
	RUN_TEST_WITH_NAME(test_array_clear);
	RUN_TEST_WITH_NAME(test_array_foreach);
	RUN_TEST_WITH_NAME(test_array_foreach_idx);
}

MU_TEST_SUITE(test_suite_slice) {
	printf("\n[Slice Tests]\n");
	RUN_TEST_WITH_NAME(test_slice_make);
	RUN_TEST_WITH_NAME(test_slice_make_partial);
	RUN_TEST_WITH_NAME(test_slice_from_dynamic_array);
	RUN_TEST_WITH_NAME(test_slice_modification);
}

MU_TEST_SUITE(test_suite_types) {
	printf("\n[Type Tests]\n");
	RUN_TEST_WITH_NAME(test_typedefs);
}

MU_TEST_SUITE(test_suite_arena) {
	printf("\n[Arena Tests]\n");
	RUN_TEST_WITH_NAME(test_arena_basic);
	RUN_TEST_WITH_NAME(test_arena_growth);
	RUN_TEST_WITH_NAME(test_arena_alignment);
}

MU_TEST_SUITE(test_suite_files) {
	printf("\n[File I/O Tests]\n");
	RUN_TEST_WITH_NAME(test_file_io_basic);
	RUN_TEST_WITH_NAME(test_file_io_sv);
	RUN_TEST_WITH_NAME(test_file_io_sb);
	RUN_TEST_WITH_NAME(test_file_io_read_missing);
	RUN_TEST_WITH_NAME(test_file_io_read_missing_sv);
	RUN_TEST_WITH_NAME(test_file_io_read_missing_sb);
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	MU_RUN_SUITE(test_suite_stringv);
	MU_RUN_SUITE(test_suite_stringb);
	MU_RUN_SUITE(test_suite_macros);
	MU_RUN_SUITE(test_suite_array);
	MU_RUN_SUITE(test_suite_slice);
	MU_RUN_SUITE(test_suite_types);
	MU_RUN_SUITE(test_suite_arena);
	MU_RUN_SUITE(test_suite_files);

	MU_REPORT();

	return MU_EXIT_CODE;
}
