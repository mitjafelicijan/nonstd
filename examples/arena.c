#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

#include <stdio.h>

typedef struct {
	int id;
	char *name;
} User;

int main(void) {
	// Initialize the arena
	Arena a = arena_make();

	// Allocate some structs
	User *u1 = arena_alloc(&a, sizeof(User));
	u1->id = 1;
	// Use string builder for name, but we can't easily alloc string builder
	// internal buffer on arena yet unless we make a custom allocator for it.
	// For now, let's just use arena_alloc for a string.
	char *name1 = arena_alloc(&a, 10);
	strcpy(name1, "Alice");
	u1->name = name1;

	User *u2 = arena_alloc(&a, sizeof(User));
	u2->id = 2;
	char *name2 = arena_alloc(&a, 10);
	strcpy(name2, "Bob");
	u2->name = name2;

	printf("User 1: %d, %s\n", u1->id, u1->name);
	printf("User 2: %d, %s\n", u2->id, u2->name);

	// Creating a linked list using arena
	struct Node {
		int val;
		struct Node *next;
	};

	struct Node *head = NULL;
	for (int i = 0; i < 5; i++) {
		struct Node *node = arena_alloc(&a, sizeof(struct Node));
		node->val = i;
		node->next = head;
		head = node;
	}

	printf("List: ");
	for (struct Node *n = head; n; n = n->next) {
		printf("%d ", n->val);
	}
	printf("\n");

	// Free everything at once
	arena_free(&a);
	printf("Arena freed.\n");

	return 0;
}
