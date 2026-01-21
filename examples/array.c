#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

#include <stdio.h>

// Example struct to demonstrate arrays of complex types
typedef struct {
	int id;
	const char *name;
} Person;

int main(void) {
	// Example 1: Basic integer array
	printf("Example 1: Basic integer array\n");
	array(int) numbers;
	array_init(numbers);

	// Push some numbers
	for (int i = 1; i <= 5; i++) {
		array_push(numbers, i * 10);
	}

	printf("  Array length: %zu\n", numbers.length);
	printf("  Array capacity: %zu\n", numbers.capacity);
	printf("  Contents: ");
	for (size_t i = 0; i < numbers.length; i++) {
		printf("%d ", numbers.data[i]);
	}
	printf("\n\n");

	// Example 2: Pop elements
	printf("Example 2: Pop elements\n");
	int last = array_pop(numbers);
	printf("  Popped: %d\n", last);
	printf("  After pop: ");
	for (size_t i = 0; i < numbers.length; i++) {
		printf("%d ", numbers.data[i]);
	}
	printf("\n\n");

	// Example 3: Insert and remove
	printf("Example 3: Insert and remove\n");
	array_insert(numbers, 2, 999); // Insert 999 at index 2
	printf("  After insert at index 2: ");
	for (size_t i = 0; i < numbers.length; i++) {
		printf("%d ", numbers.data[i]);
	}
	printf("\n");

	array_remove(numbers, 1); // Remove element at index 1
	printf("  After remove index 1: ");
	for (size_t i = 0; i < numbers.length; i++) {
		printf("%d ", numbers.data[i]);
	}
	printf("\n\n");

	// Example 4: Get and set
	printf("Example 4: Get and set\n");
	int value = array_get(numbers, 0);
	printf("  Value at index 0: %d\n", value);
	array_set(numbers, 0, 777);
	printf("  After setting index 0 to 777: ");
	for (size_t i = 0; i < numbers.length; i++) {
		printf("%d ", numbers.data[i]);
	}
	printf("\n\n");

	array_free(numbers);

	// Example 5: Array with initial capacity
	printf("Example 5: Array with initial capacity\n");
	array(int) preallocated;
	array_init_cap(preallocated, 100);
	printf("  Initial capacity: %zu\n", preallocated.capacity);
	printf("  Initial length: %zu\n", preallocated.length);
	array_free(preallocated);
	printf("\n");

	// Example 6: foreach iteration
	printf("Example 6: foreach iteration\n");
	array(int) values;
	array_init(values);
	for (int i = 0; i < 10; i++) {
		array_push(values, i * i); // Push squares
	}

	printf("  Using array_foreach: ");
	int val;
	array_foreach(values, val) { printf("%d ", val); }
	printf("\n");

	printf("  Using array_foreach_i: ");
	array_foreach_i(values, val, idx) { printf("[%zu]=%d ", idx, val); }
	printf("\n\n");

	array_free(values);

	// Example 7: Array of strings
	printf("Example 7: Array of strings\n");
	array(const char *) words;
	array_init(words);

	array_push(words, "Hello");
	array_push(words, "World");
	array_push(words, "from");
	array_push(words, "C");

	printf("  Words: ");
	const char *word;
	array_foreach(words, word) { printf("%s ", word); }
	printf("\n\n");

	array_free(words);

	// Example 8: Array of structs
	printf("Example 8: Array of structs\n");
	array(Person) people;
	array_init(people);

	array_push(people, ((Person){1, "Alice"}));
	array_push(people, ((Person){2, "Bob"}));
	array_push(people, ((Person){3, "Charlie"}));

	printf("  People:\n");
	Person person;
	array_foreach(people, person) { printf("    ID: %d, Name: %s\n", person.id, person.name); }
	printf("\n");

	array_free(people);

	// Example 9: Reserve capacity
	printf("Example 9: Reserve capacity\n");
	array(double) measurements;
	array_init(measurements);
	printf("  Initial capacity: %zu\n", measurements.capacity);

	array_reserve(measurements, 1000);
	printf("  After reserve(1000): %zu\n", measurements.capacity);

	array_free(measurements);
	printf("\n");

	// Example 10: Clear array
	printf("Example 10: Clear array\n");
	array(int) temp;
	array_init(temp);
	for (int i = 0; i < 5; i++) {
		array_push(temp, i);
	}
	printf("  Length before clear: %zu\n", temp.length);
	array_clear(temp);
	printf("  Length after clear: %zu\n", temp.length);
	printf("  Capacity after clear: %zu\n", temp.capacity);

	array_free(temp);

	return 0;
}
