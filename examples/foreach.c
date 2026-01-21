#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

#include <stdio.h>

int main(void) {
	// Example 1: Iterate over an array of integers
	int numbers[] = {10, 20, 30, 40, 50};
	int num; // Declare the loop variable

	static_foreach(int, num, numbers) {
		printf("  Number: %d\n", num);
	}
	printf("\n");

	// Example 2: Iterate over an array of floats
	float prices[] = {9.99f, 19.99f, 29.99f, 49.99f};
	float price; // Declare the loop variable

	static_foreach(float, price, prices) {
		printf("  Price: $%.2f\n", price);
	}
	printf("\n");

	// Example 3: Iterate over an array of strings
	const char *fruits[] = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};
	const char *fruit; // Declare the loop variable

	static_foreach(const char *, fruit, fruits) {
		printf("  Fruit: %s\n", fruit);
	}
	printf("\n");

	// Example 4: Perform calculations with static_foreach
	int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int sum = 0;
	int val; // Declare the loop variable

	static_foreach(int, val, values) {
		sum += val;
	}
	printf("  Sum of values: %d\n", sum);
	printf("\n");

	// Example 5: Iterate over struct array
	typedef struct {
		const char *name;
		int age;
	} Person;

	Person people[] = {{"Alice", 25}, {"Bob", 30}, {"Charlie", 35}};
	Person person; // Declare the loop variable

	static_foreach(Person, person, people) {
		printf("  %s is %d years old\n", person.name, person.age);
	}
	printf("\n");

	// Example 6: Modify array elements in place
	int nums[] = {1, 2, 3, 4, 5};
	int n; // Declare the loop variable

	printf("  Before: ");
	static_foreach(int, n, nums) {
		printf("%d ", n);
	}
	printf("\n");

	// Note: static_foreach creates a copy of each element by default
	// To modify elements, use a traditional for loop with array indexing
	for (size_t i = 0; i < countof(nums); i++) {
		nums[i] *= 2;
	}

	printf("  After doubling: ");
	static_foreach(int, n, nums) {
		printf("%d ", n);
	}
	printf("\n");

	return 0;
}
