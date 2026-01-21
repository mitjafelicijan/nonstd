#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

#include <stdio.h>

SLICE_DEF(int);

// Example function that accepts a slice of integers
void print_int_slice(slice(int) s) {
	printf("  Slice (len=%zu): [", s.length);
	for (size_t i = 0; i < s.length; i++) {
		printf("%d%s", s.data[i], i < s.length - 1 ? ", " : "");
	}
	printf("]\n");
}

int main(void) {
	// Example 1: Slice from static array
	printf("Example 1: Slice from static C array\n");
	int static_nums[] = {10, 20, 30, 40, 50};

	// Create full slice
	slice(int) s1 = make_slice(int, static_nums, countof(static_nums));
	print_int_slice(s1);

	// Create partial slice (subset)
	slice(int) s2 = make_slice(int, static_nums + 1, 3); // 20, 30, 40
	print_int_slice(s2);
	printf("\n");

	// Example 2: Slice from dynamic array
	printf("Example 2: Slice from dynamic array\n");
	array(int) dyn_arr;
	array_init(dyn_arr);
	for (int i = 1; i <= 5; i++)
		array_push(dyn_arr, i * 100);

	// Convert entire array to slice
	slice(int) s3 = array_as_slice(int, dyn_arr);
	print_int_slice(s3);

	// Create slice from dynamic array data manually
	slice(int) s4 = make_slice(int, dyn_arr.data + 2, 2); // 300, 400
	print_int_slice(s4);

	array_free(dyn_arr);
	printf("  (Dynamic array freed)\n\n");

	// Example 3: Modifying data through slice
	printf("Example 3: Modifying data through slice\n");
	int values[] = {1, 1, 1, 1, 1};
	slice(int) s5 = make_slice(int, values, 5);

	print_int_slice(s5);

	// Modify middle elements
	s5.data[2] = 99;
	s5.data[3] = 99;

	printf("  After modification:\n");
	print_int_slice(s5);
	printf("\n");

	// Example 4: Slice of strings
	printf("Example 4: Slice of strings\n");
	typedef const char *cstr;
	SLICE_DEF(cstr);

	cstr names[] = {"Alice", "Bob", "Charlie"};
	slice(cstr) name_slice = make_slice(cstr, names, 3);

	printf("  Names: ");
	for (size_t i = 0; i < name_slice.length; i++) {
		printf("%s ", name_slice.data[i]);
	}
	printf("\n");

	return 0;
}
