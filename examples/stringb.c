#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

#include <stdio.h>

// Helper function to print string builder content
void print_sb(const char *label, const stringb *sb) {
	printf("%s (len=%zu, cap=%zu): \"%s\"\n", label, sb->length, sb->capacity, sb->data);
}

int main(void) {
	// Example 1: Basic string building
	stringb sb1;
	sb_init(&sb1, 0); // 0 means use default capacity (16)

	sb_append_cstr(&sb1, "Hello");
	sb_append_cstr(&sb1, ", ");
	sb_append_cstr(&sb1, "World");
	sb_append_char(&sb1, '!');

	print_sb("  sb1", &sb1);
	sb_free(&sb1);
	printf("\n");

	// Example 2: Building with initial capacity
	stringb sb2;
	sb_init(&sb2, 100); // Start with larger capacity to avoid reallocations

	print_sb("  Initial", &sb2);

	sb_append_cstr(&sb2, "This is a longer string that benefits from pre-allocation");
	print_sb("  After append", &sb2);

	sb_free(&sb2);
	printf("\n");

	// Example 3: Appending string views
	stringb sb3;
	sb_init(&sb3, 0);

	const char *text = "The quick brown fox";
	stringv word = sv_from_parts(text + 4, 5); // "quick"

	sb_append_cstr(&sb3, "Extracted: ");
	sb_append_sv(&sb3, word);
	sb_append_cstr(&sb3, " (from a view)");

	print_sb("  sb3", &sb3);
	sb_free(&sb3);
	printf("\n");

	// Example 4: Converting builder to view for reading
	stringb sb4;
	sb_init(&sb4, 0);

	sb_append_cstr(&sb4, "Builder content");

	// Get a read-only view of the builder
	stringv view = sb_as_sv(&sb4);
	printf("  Builder as view (len=%zu): \"", view.length);
	fwrite(view.data, 1, view.length, stdout);
	printf("\"\n");

	// You can use all string view operations on it
	stringv prefix = sv_from_cstr("Builder");
	printf("  Starts with 'Builder': %s\n", sv_starts_with(view, prefix) ? "yes" : "no");

	sb_free(&sb4);
	printf("\n");

	// Example 5: Building a formatted message
	stringb sb5;
	sb_init(&sb5, 0);

	const char *names[] = {"Alice", "Bob", "Charlie"};
	int ages[] = {25, 30, 35};

	sb_append_cstr(&sb5, "People:\n");
	for (size_t i = 0; i < countof(names); i++) {
		sb_append_cstr(&sb5, "  - ");
		sb_append_cstr(&sb5, names[i]);
		sb_append_cstr(&sb5, " (age ");

		// Note: For numbers, you'd typically use sprintf with a temp buffer
		char age_buf[32];
		snprintf(age_buf, sizeof(age_buf), "%d", ages[i]);
		sb_append_cstr(&sb5, age_buf);

		sb_append_cstr(&sb5, ")\n");
	}

	printf("%s", sb5.data);
	sb_free(&sb5);
	printf("\n");

	// Example 6: Building CSV data
	stringb csv;
	sb_init(&csv, 64);

	sb_append_cstr(&csv, "Name,Age,City\n");
	sb_append_cstr(&csv, "Alice,25,NYC\n");
	sb_append_cstr(&csv, "Bob,30,LA\n");
	sb_append_cstr(&csv, "Charlie,35,Chicago\n");

	printf("  CSV output:\n%s", csv.data);
	sb_free(&csv);
	printf("\n");

	// Example 7: Practical use case - building a SQL query
	stringb query;
	sb_init(&query, 128);

	sb_append_cstr(&query, "SELECT * FROM users WHERE ");

	const char *conditions[] = {"age > 18", "active = 1", "country = 'US'"};
	for (size_t i = 0; i < countof(conditions); i++) {
		if (i > 0) {
			sb_append_cstr(&query, " AND ");
		}
		sb_append_cstr(&query, conditions[i]);
	}

	sb_append_char(&query, ';');

	print_sb("  Query", &query);
	sb_free(&query);
	printf("\n");

	return 0;
}
