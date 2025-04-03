#include "libasm.h"
extern size_t ft_strlen(const char* str);
extern char*  ft_strcpy(char* dest, const char* src);
extern int    ft_strcmp(const char* s1, const char* s2);
extern int    ft_write(int fildes, const void* buff, size_t nbytes);


void run_strcpy_test(const char *test_name, const char *src) {
    char dest[100]; 

    char *result = ft_strcpy(dest, src);

    printf("[%s] src: \"%s\" | dest: \"%s\" | returned: %s\n",
           test_name, src, dest, (result == dest) ? "PASS" : "FAIL");
}

void run_strcmp_test(const char *s1, const char *s2) {
    int result_asm = ft_strcmp(s1, s2);
    int result_c = strcmp(s1, s2);

    // Normalize results (strcmp returns negative, zero, or positive values)
    if (result_asm < 0) result_asm = -1;
    if (result_asm > 0) result_asm = 1;
    if (result_c < 0) result_c = -1;
    if (result_c > 0) result_c = 1;

    // Print results
    printf("Comparing \"%s\" vs \"%s\":\n", s1, s2);
    printf("  ft_strcmp: %d\n", result_asm);
    printf("  strcmp:    %d\n", result_c);
    printf("  Test ");
    if (result_asm == 0 && result_c)
        printf("FAILED");
    else if (result_asm && result_c == 0)
        printf("FAILED");
    else
        printf("PASSED");
    printf("!\n\n");
}

void run_write_test(const char *test_str, int expected_bytes, size_t length) {
    int bytes_written = ft_write(1, test_str, length);  // Write to stdout
    if (bytes_written != expected_bytes) {
        printf("Test failed: expected %d bytes, got %d bytes.\n", expected_bytes, bytes_written);
    } else {
        printf("Test passed: wrote %d bytes successfully.\n", bytes_written);
    }
}


int main(void) {

    printf("----/ft_strlen tests/----\n");
    const char *test_cases[] = {
        "Hello",
        "",
        "A longer string!",
        "123456789",
        "Test\0Hidden"
    };
    for (int i = 0; i < 5; i++) {
        printf("ft_strlen: Length of \"%s\" = %zu\n", test_cases[i], ft_strlen(test_cases[i]));
        printf("strlen: Length of \"%s\" = %zu\n", test_cases[i], strlen(test_cases[i]));
    }

    printf("\n----/ft_strcpy tests/----\n");
    run_strcpy_test("Normal String", "Hello, world!");
    run_strcpy_test("Single Character", "A");
    run_strcpy_test("Long String", "This is a very long string used for testing strcpy.");

    run_strcpy_test("Empty String", "");
    run_strcpy_test("String with Spaces", "   leading and trailing   ");
    run_strcpy_test("String with Newline", "Line1\nLine2");
    run_strcpy_test("String with Null Terminator Midway", "Test\0Hidden");

    run_strcpy_test("String with Special Characters", "!@#$%^&*()_+-=[]{}|;':,.<>?/");

    printf("\n----/ft_strcmp tests/----\n");
    run_strcmp_test("hello", "hello");
    run_strcmp_test("abc", "abd");         // Should return -1
    run_strcmp_test("zebra", "apple");     // Should return 1
    run_strcmp_test("", "");               // Both empty, should return 0
    run_strcmp_test("abc", "");            // Second string empty, should return 1
    run_strcmp_test("", "abc");            // First string empty, should return -1
    run_strcmp_test("abcd", "abc");        // First is longer, should return 1
    run_strcmp_test("abc", "abcd");        // Second is longer, should return -1
    run_strcmp_test("abc\0def", "abc");    // Should stop at '\0', return 0
    run_strcmp_test("hello", "hellO");     // Case-sensitive, should return 1
    run_strcmp_test("123", "124");         // Should return -1

    printf("\n----/ft_write tests/----\n");
    // Test Case 1: Write a simple string to stdout
    run_write_test("Hello, world!\n", 14, strlen("Hello, world!\n"));  // "Hello, world!" + newline
    // Test Case 2: Write an empty string to stdout
    run_write_test("", 0, 0);  // No output, should write 0 bytes
    // Test Case 3: Write a longer string
    run_write_test("This is a longer test string.\n", 30, strlen("This is a longer test string.\n"));  // 30 characters
    // Test Case 4: Test with a custom file descriptor (e.g., stderr)
    run_write_test("Error message to stderr.\n", 25, strlen("Error message to stderr.\n"));  // Expected output length: 25
    ft_write(2, "Error message to stderr.\n", 25); // Write to stderr using ft_write directly
    ft_write(1, "Writing to invalid fd.\n", 23); // Write to stderr using ft_write directly
    int error = ft_write(-1, "error", 5);
    printf("Error value returned: %d\n", error);
    ft_write(1, "Writing with bad size.\n", 23); // Write to stderr using ft_write directly
    run_write_test("This is not the right size\n", -1, -1);
    // Test Case 5: Invalid file descriptor (simulate an error)
}
