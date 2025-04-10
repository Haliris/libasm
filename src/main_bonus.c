#include "libasm.h"

typedef struct list {
    void        *data;
    struct list *next;
} t_list;

extern int    ft_list_size(t_list *list);
extern void   ft_list_push_front(t_list **head, void *data);
extern void   ft_list_sort(t_list **begin_list, int (*cmp)());
extern void   ft_list_remove_if(t_list **begin, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

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

void dummy_free(void *data) {
    (void)data;
    // nothing needed here, since data points to stack ints
    return;
}

int is_equal_int(void *a, void *b) {
    return (*(int *)a == *(int *)b);
}

void test_list_remove_if(void) {
    printf("\n----/list_remove_if tests/----\n");

    int a1[] = {1, 2, 3, 4, 5};
    t_list *list1 = create_list_from_array(a1, 5);
    int ref1 = 3;
    ft_list_remove_if(&list1, &ref1, is_equal_int, dummy_free);
    printf("Remove 3 from [1 2 3 4 5]: ");
    print_list_int(list1);
    free_list(list1);

    int a2[] = {1, 1, 1, 1};
    t_list *list2 = create_list_from_array(a2, 4);
    int ref2 = 1;
    ft_list_remove_if(&list2, &ref2, is_equal_int, dummy_free);
    printf("Remove all 1s from [1 1 1 1]: ");
    print_list_int(list2);
    free_list(list2);

    int a3[] = {1, 2, 3, 4};
    t_list *list3 = create_list_from_array(a3, 4);
    int ref3 = 99;
    ft_list_remove_if(&list3, &ref3, is_equal_int, dummy_free);
    printf("Remove 99 (not present) from [1 2 3 4]: ");
    print_list_int(list3);
    free_list(list3);

    int a4[] = {};
    t_list *list4 = create_list_from_array(a4, 0);
    int ref4 = 1;
    ft_list_remove_if(&list4, &ref4, is_equal_int, dummy_free);
    printf("Remove 1 from empty list: ");
    print_list_int(list4);
    free_list(list4);

    int a5[] = {2};
    t_list *list5 = create_list_from_array(a5, 1);
    int ref5 = 2;
    ft_list_remove_if(&list5, &ref5, is_equal_int, dummy_free);
    printf("Remove 2 from single-element [2]: ");
    print_list_int(list5);
    free_list(list5);
}

int main(void) {
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

    test_list_remove_if();
    return 0;
}
