#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

#include <stdio.h>

// Helper function to print a string view
void print_sv(const char *label, stringv sv) {
	printf("%s (len=%zu): \"", label, sv.length);
	fwrite(sv.data, 1, sv.length, stdout);
	printf("\"\n");
}

int main(void) {
	// Example 1: Create string view from C string
	const char *hello = "Hello, World!";
	stringv sv1 = sv_from_cstr(hello);
	print_sv("  sv1", sv1);
	printf("\n");

	// Example 2: Create view from pointer and length
	const char *text = "Programming in C";
	stringv sv2 = sv_from_parts(text, 11); // Only "Programming"
	print_sv("  sv2", sv2);
	printf("\n");

	// Example 3: Slicing - extract substrings without copying
	stringv original = sv_from_cstr("The quick brown fox");

	stringv word1 = sv_slice(original, 0, 3);	// "The"
	stringv word2 = sv_slice(original, 4, 9);	// "quick"
	stringv word3 = sv_slice(original, 10, 15); // "brown"

	print_sv("  Original", original);
	print_sv("  Word 1", word1);
	print_sv("  Word 2", word2);
	print_sv("  Word 3", word3);
	printf("\n");

	// Example 4: Comparing string views
	stringv str_a = sv_from_cstr("hello");
	stringv str_b = sv_from_cstr("hello");
	stringv str_c = sv_from_cstr("world");

	printf("  str_a == str_b: %s\n", sv_equals(str_a, str_b) ? "true" : "false");
	printf("  str_a == str_c: %s\n", sv_equals(str_a, str_c) ? "true" : "false");
	printf("\n");

	// Example 5: Prefix and suffix checking
	stringv filename = sv_from_cstr("document.txt");
	stringv prefix = sv_from_cstr("doc");
	stringv suffix = sv_from_cstr(".txt");
	stringv wrong_suffix = sv_from_cstr(".pdf");

	print_sv("  Filename", filename);
	printf("  Starts with 'doc': %s\n", sv_starts_with(filename, prefix) ? "yes" : "no");
	printf("  Ends with '.txt': %s\n", sv_ends_with(filename, suffix) ? "yes" : "no");
	printf("  Ends with '.pdf': %s\n", sv_ends_with(filename, wrong_suffix) ? "yes" : "no");
	printf("\n");

	// Example 6: Parsing a path (practical use case)
	const char *path = "/home/user/documents/report.pdf";
	stringv path_sv = sv_from_cstr(path);

	// Find the last '/' to extract filename
	size_t last_slash = 0;
	for (size_t i = 0; i < path_sv.length; i++) {
		if (path_sv.data[i] == '/') {
			last_slash = i + 1;
		}
	}

	stringv directory = sv_slice(path_sv, 0, last_slash);
	stringv filename2 = sv_slice(path_sv, last_slash, path_sv.length);

	print_sv("  Full path", path_sv);
	print_sv("  Directory", directory);
	print_sv("  Filename", filename2);
	printf("\n");

	// Example 7: Tokenizing without allocation
	const char *sentence = "one two three four";
	stringv sent_sv = sv_from_cstr(sentence);

	size_t start = 0;
	for (size_t i = 0; i <= sent_sv.length; i++) {
		if (i == sent_sv.length || sent_sv.data[i] == ' ') {
			if (i > start) {
				stringv token = sv_slice(sent_sv, start, i);
				print_sv("  Token", token);
			}
			start = i + 1;
		}
	}
	printf("\n");

	return 0;
}
