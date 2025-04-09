#include "libasm.h"

//set $node = (t_list *)$rdi
//print *$node
//
//print *((t_list *)$rdi)
//
//set $n = (t_list *)$rdi
//while ($n != 0)
//  print $n->data
//  set $n = $n->next
//end


typedef struct list {
    void        *data;
    struct list *next;
} t_list;

extern size_t ft_strlen(const char* str);
extern char   *ft_strcpy(char* dest, const char* src);
extern int    ft_strcmp(const char* s1, const char* s2);
extern int    ft_write(int fildes, const void* buff, size_t nbytes);
extern int    ft_read(int fildes, const void* buff, size_t nbytes);
extern char   *ft_strdup(const char* s);
extern int    ft_atoi_base(char *str, char *base);
extern int    ft_list_size(t_list *list);
extern void   ft_list_push_front(t_list **head, void *data);
extern void   ft_list_sort(t_list **begin_list, int (*cmp)());


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

void test_list_size(t_list *head, size_t expected) {
    size_t result = ft_list_size(head);
    if (result == expected)
        printf("PASS: expected %zu, got %zu\n", expected, result);
    else
        printf("FAIL: expected %zu, got %zu\n", expected, result);
}

void print_list_char(t_list *head) {
    printf("List: ");
    while (head) {
        printf("%s -> ", (char *)head->data);
        head = head->next;
    }
    printf("NULL\n");
}

void print_list_int(t_list *head) {
    printf("List: ");
    while (head) {
        printf("%d -> ", *(int *)head->data);
        head = head->next;
    }
    printf("NULL\n");
}

int cmp_int(void *a, void *b) {
//    printf("Comparing %d and %d\n", int_a, int_b); // Debug print
    if (*(int *)a - *(int *)b > 0)
        return 0;
    else
        return 1;
}

int is_sorted(t_list *list, int (*cmp)()) {
    while (list && list->next) {
        if (cmp(list->data, list->next->data) == 0)
            return 0;
        list = list->next;
    }
    return 1;
}

t_list *create_list_from_array(int *arr, int size) {
    t_list *head = NULL;
    for (int i = size - 1; i >= 0; --i) {
        t_list *new_node = malloc(sizeof(t_list));
        new_node->data = &arr[i];
        new_node->next = head;
        head = new_node;
    }
    return head;
}

void free_list(t_list *head) {
    while (head) {
        t_list *tmp = head;
        head = head->next;
        free(tmp);
    }
}

void test_list_push_front(void) {
    t_list *head = NULL;

    ft_list_push_front(&head, "World");
    print_list_char(head);
    ft_list_push_front(&head, "Hello");
    print_list_char(head);

    if (head && strcmp((char *)head->data, "Hello") == 0)
        printf("PASS: Head is Hello\n");
    else
        printf("FAIL: Head is not Hello\n");

    // Push another
    ft_list_push_front(&head, "Start");
    print_list_char(head);

    // Edge case: pushing to an existing list
    ft_list_push_front(&head, "");
    print_list_char(head);

}

void test_list_sort(int *array, int size) {
    printf("Test: sorting list of %d elements\n", size);

    t_list *list = create_list_from_array(array, size);
    printf("Before: ");
    print_list_int(list);

    ft_list_sort(&list, cmp_int);

    printf("After:  ");
    print_list_int(list);

    if(is_sorted(list, cmp_int)) {
        printf("✅ Passed!\n\n");
    }
    else {
        printf("Failed!\n\n");
    }
    free_list(list);
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
    FILE *file = fopen("testfile.txt", "w");
    if (file) {
        fprintf(file, "This is a test file for ft_read testing.\n");
        fclose(file);
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
    test_atoi_base("1010", "01", 10);
    test_atoi_base("   +1A", "0123456789ABCDEF", 26);
    test_atoi_base("-1a", "0123456789abcdef", -26);
    test_atoi_base("42", "0123456789", 42);
    test_atoi_base("   \t\n\r\f\v7F", "0123456789ABCDEF", 127);

    test_atoi_base("123", "1123456", 0);
    test_atoi_base("123", "", 0);

    test_atoi_base("ZZZ", "0123456789ABCDEF", 0);
    test_atoi_base("123!", "0123456789", 123);
    test_atoi_base("+abc", "abc", 5);

    test_atoi_base("", "0123456789", 0);
    test_atoi_base("-", "0123456789", 0);
    test_atoi_base("000", "0123456789", 0);
    test_atoi_base("   +0", "0123456789", 0);
    test_atoi_base("   -ff", "0123456789abcdef", -255);

    printf("\n----/list_size tests/----\n");
    test_list_size(NULL, 0);

    t_list node1 = { .data = NULL, .next = NULL };
    test_list_size(&node1, 1);

    t_list node2 = { .data = NULL, .next = NULL };
    node1.next = &node2;
    test_list_size(&node1, 2);

    t_list node3 = { .data = NULL, .next = NULL };
    node2.next = &node3;
    test_list_size(&node1, 3);

    t_list *nodes = malloc(sizeof(t_list) * 10);
    for (int i = 0; i < 9; ++i) {
        nodes[i].data = NULL;
        nodes[i].next = &nodes[i + 1];
    }
    nodes[9].data = NULL;
    nodes[9].next = NULL;
    test_list_size(&nodes[0], 10);

    printf("\n----/list_push_front tests/----\n");
    test_list_push_front();
    
    printf("\n----/list_sort tests/----\n");
    int a[] = {5, 3, 8, 1, 2};
    test_list_sort(a, 5);

    int b[] = {1, 2, 3, 4, 5}; // already sorted
    test_list_sort(b, 5);

    int c[] = {5, 4, 3, 2, 1}; // reverse order
    test_list_sort(c, 5);

    int d[] = {42}; // single element
    test_list_sort(d, 1);

    int e[] = {}; // empty list
    test_list_sort(e, 0);

    int f[] = {7, 7, 7, 7}; // duplicates
    test_list_sort(f, 4);

    return 0;
}
