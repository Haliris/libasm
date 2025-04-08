#include "libasm.h"
extern size_t ft_strlen(const char* str);
extern char   *ft_strcpy(char* dest, const char* src);
extern int    ft_strcmp(const char* s1, const char* s2);
extern int    ft_write(int fildes, const void* buff, size_t nbytes);
extern int    ft_read(int fildes, const void* buff, size_t nbytes);
extern char   *ft_strdup(const char* s);
extern int    ft_atoi_base(char *str, char *base);


void run_strcpy_test(const char *test_name, const char *src) {
    char dest[1000]; 

    memset(dest, 0, 1000);

    char *result = ft_strcpy(dest, src);

    printf("[%s] src: \"%s\" | dest: \"%s\" | returned: %s\n",
           test_name, src, dest, (result == dest) ? "PASS" : "FAIL");
}

void run_strcmp_test(const char *s1, const char *s2) {
    int result_asm = ft_strcmp(s1, s2);
    int result_c = strcmp(s1, s2);

    if (result_asm < 0) result_asm = -1;
    if (result_asm > 0) result_asm = 1;
    if (result_c < 0) result_c = -1;
    if (result_c > 0) result_c = 1;

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
    int bytes_written = ft_write(1, test_str, length);
    if (bytes_written != expected_bytes) {
        printf("Test failed: expected %d bytes, got %d bytes.\n", expected_bytes, bytes_written);
    } else {
        printf("Test passed: wrote %d bytes successfully.\n", bytes_written);
    }
}

void test_read_success() {
    printf("=== Testing successful read ===\n");
    int fd = open("testfile.txt", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open test file");
        return;
    }
    
    char buf1[100] = {0};
    char buf2[100] = {0};
    
    ssize_t sys_ret = read(fd, buf1, 10);
    lseek(fd, 0, SEEK_SET);
    ssize_t ft_ret = ft_read(fd, buf2, 10);
    
    printf("System read returned: %zd\n", sys_ret);
    printf("ft_read returned: %zd\n", ft_ret);
    printf("Buffers match: %d\n", memcmp(buf1, buf2, 10) == 0);
    
    close(fd);
}

void test_read_empty() {
    printf("=== Testing read from empty file ===\n");
    system("touch empty.txt");
    int fd = open("empty.txt", O_RDONLY);
    
    char buf[10];
    ssize_t ret = ft_read(fd, buf, 10);
    
    printf("Return value (should be 0): %zd\n", ret);
    
    close(fd);
}

void test_read_error() {
    printf("=== Testing error case (bad file descriptor) ===\n");
    char buf[10];
    ssize_t ret = ft_read(-1, buf, 10);
    
    printf("Return value (should be -1): %zd\n", ret);
    printf("errno set: %d (%s)\n", errno, strerror(errno));
}

void test_read_stdin() {
    printf("=== Testing read from stdin ===\n");
    printf("Type something (up to 10 chars) and press enter: ");
    
    char buf[11] = {0};
    ssize_t ret = ft_read(STDIN_FILENO, buf, 10);
    
    printf("\nYou entered (%zd bytes): %s\n", ret, buf);
}

void test_read_zero_bytes() {
    printf("=== Testing read of zero bytes ===\n");
    int fd = open("testfile.txt", O_RDONLY);
    
    char buf[10];
    ssize_t ret = ft_read(fd, buf, 0);
    
    printf("Return value (should be 0): %zd\n", ret);
    
    close(fd);
}

void test_strdup(const char *input) {
    char *result = ft_strdup(input);
    char *expected = strdup(input);

    if (!result) {
        printf("FAIL: strdup returned NULL for input: \"%s\"\n", input);
        free(expected);
        return;
    }

    if (strcmp(result, expected) != 0) {
        printf("FAIL: Expected \"%s\", got \"%s\"\n", expected, result);
    } else {
        printf("PASS: \"%s\" duplicated successfully.\n", input);
    }
    free(result);
    free(expected);
}

void test_atoi_base(char *input, char *base, int expected) {
    int result = ft_atoi_base(input, base);
    if (result == expected) {
        printf("PASS: ft_atoi_base(\"%s\", \"%s\") == %d\n", input, base, expected);
    } else {
        printf("FAIL: ft_atoi_base(\"%s\", \"%s\") == %d, expected %d\n", input, base, result, expected);
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
    run_strcmp_test("abc", "abd");
    run_strcmp_test("zebra", "apple");
    run_strcmp_test("", "");
    run_strcmp_test("abc", "");
    run_strcmp_test("", "abc");
    run_strcmp_test("abcd", "abc");
    run_strcmp_test("abc", "abcd");
    run_strcmp_test("abc\0def", "abc");
    run_strcmp_test("hello", "hellO");
    run_strcmp_test("123", "124");

    printf("\n----/ft_write tests/----\n");
    run_write_test("Hello, world!\n", 14, strlen("Hello, world!\n"));
    run_write_test("", 0, 0);
    run_write_test("This is a longer test string.\n", 30, strlen("This is a longer test string.\n"));
    run_write_test("Error message to stderr.\n", 25, strlen("Error message to stderr.\n"));
    ft_write(2, "Error message to stderr.\n", 25);
    ft_write(1, "Writing to invalid fd.\n", 23);
    int error = ft_write(-1, "error", 5);
    printf("Error value returned: %d\n", error);
    printf("Errno value: %d (%s)\n", errno, strerror(errno));
    ft_write(1, "Writing with bad size.\n", 23);
    run_write_test("This is not the right size\n", -1, -1);
    printf("Errno value: %d (%s)\n", errno, strerror(errno));

    printf("\n----/ft_read tests/----\n");
    FILE *f = fopen("testfile.txt", "w");
    if (f) {
        fprintf(f, "This is a test file for ft_read testing.\n");
        fclose(f);
    }
    test_read_success();
    test_read_empty();
    test_read_error();
    test_read_stdin();
    test_read_zero_bytes();
    remove("testfile.txt");
    remove("empty.txt");

    printf("\n----/Starting ft_strdup Tests/----\n\n");
    test_strdup("");
    test_strdup("hello");
    test_strdup("1234567890");
    test_strdup("!@#$%^&*()");
    test_strdup("The quick brown fox");
    test_strdup("A");
    test_strdup("Long string..................................................");

    printf("\n----/atoi_base tests/----\n");
    // Valid tests
    test_atoi_base("1010", "01", 10);                    // binary
    test_atoi_base("   +1A", "0123456789ABCDEF", 26);    // hex with whitespace and +
    test_atoi_base("-1a", "0123456789abcdef", -26);      // lowercase hex
    test_atoi_base("42", "0123456789", 42);              // decimal
    test_atoi_base("   \t\n\r\f\v7F", "0123456789ABCDEF", 127); // hex with whitespaces

    // Base validation test_atoi_bases
    test_atoi_base("123", "1", 0);                       // invalid base: only one char
    test_atoi_base("123", "1123456", 0);                 // invalid base: duplicate chars
    test_atoi_base("123", "", 0);                        // empty base

    // Invalid characters in string
    test_atoi_base("ZZZ", "0123456789ABCDEF", 0);        // input has characters not in base
    test_atoi_base("123!", "0123456789", 123);           // stops parsing at '!'
    test_atoi_base("+abc", "abc", 5);                    // valid + sign

    // Edge cases
    test_atoi_base("", "0123456789", 0);                 // empty input
    test_atoi_base("-", "0123456789", 0);                // only minus sign
    test_atoi_base("000", "0123456789", 0);              // leading zeroes
    test_atoi_base("   +0", "0123456789", 0);            // zero with whitespaces and sign
    test_atoi_base("   -ff", "0123456789abcdef", -255);
    return 0;
}
