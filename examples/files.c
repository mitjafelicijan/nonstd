#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

#include <stdio.h>

int main(void) {
	// 1. Basic usage
	const char *msg = "Hello, World!\n";
	if (write_entire_file("test_basic.txt", msg, strlen(msg))) {
		printf("Written test_basic.txt\n");
	}

	size_t sz;
	char *content = read_entire_file("test_basic.txt", &sz);
	if (content) {
		printf("Read: %.*s", (int)sz, content);
		FREE(content);
	}

	// 2. usage with string view
	stringv sv = sv_from_cstr("Hello from String View!\n");
	if (write_file_sv("test_sv.txt", sv)) {
		printf("Written test_sv.txt\n");
	}

	// 3. usage with string builder
	stringb sb;
	sb_init(&sb, 0);
	sb_append_cstr(&sb, "Hello from ");
	sb_append_cstr(&sb, "String Builder!\n");

	if (write_file_sb("test_sb.txt", &sb)) {
		printf("Written test_sb.txt\n");
	}

	// Read into stringb
	stringb sb2 = read_entire_file_sb("test_sb.txt");
	if (sb2.data) {
		printf("Read into sb: %s", sb2.data);
		sb_free(&sb2);
	}

	sb_free(&sb);

	return 0;
}
