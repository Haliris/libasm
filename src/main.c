#include "libasm.h"
extern size_t ft_strlen(const char* str);
extern char*  ft_strcpy(char* dest, const char* src);


void run_test(const char *test_name, const char *src) {
    char dest[100]; 

    char *result = ft_strcpy(dest, src);

    printf("[%s] src: \"%s\" | dest: \"%s\" | returned: %s\n",
           test_name, src, dest, (result == dest) ? "PASS" : "FAIL");
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
    run_test("Normal String", "Hello, world!");
    run_test("Single Character", "A");
    run_test("Long String", "This is a very long string used for testing strcpy.");

    run_test("Empty String", "");
    run_test("String with Spaces", "   leading and trailing   ");
    run_test("String with Newline", "Line1\nLine2");
    run_test("String with Null Terminator Midway", "Test\0Hidden");

    run_test("String with Special Characters", "!@#$%^&*()_+-=[]{}|;':,.<>?/");
    return 0;
}
