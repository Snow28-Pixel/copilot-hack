#include "ds.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(condition)                                                      \
    do {                                                                      \
        if (!(condition)) {                                                   \
            fprintf(stderr, "检查失败：%s（%s:%d）\n", #condition, __FILE__, \
                    __LINE__);                                               \
            exit(EXIT_FAILURE);                                              \
        }                                                                     \
    } while (0)

typedef struct {
    int values[DS_GRAPH_MAX_VERTICES + 1];
    size_t length;
} VisitLog;

static void record_tree_value(int value, void *context)
{
    VisitLog *log = (VisitLog *)context;
    log->values[log->length++] = value;
}

static void record_vertex(size_t vertex, void *context)
{
    VisitLog *log = (VisitLog *)context;
    log->values[log->length++] = (int)vertex;
}

static void check_log(const VisitLog *log, const int *expected, size_t length)
{
    size_t i;
    CHECK(log->length == length);
    for (i = 0; i < length; ++i) {
        CHECK(log->values[i] == expected[i]);
    }
}

static void test_linear_structures(void)
{
    SqList array_list;
    LinkList linked_list;
    int value;
    size_t i;

    sqlist_init(&array_list);
    CHECK(sqlist_insert(&array_list, 0, 10));
    CHECK(sqlist_insert(&array_list, 1, 30));
    CHECK(sqlist_insert(&array_list, 1, 20));
    CHECK(!sqlist_insert(&array_list, 4, 40));
    CHECK(sqlist_get(&array_list, 1, &value) && value == 20);
    CHECK(sqlist_delete(&array_list, 1, &value) && value == 20);
    CHECK(!sqlist_get(&array_list, 2, &value));
    sqlist_init(&array_list);
    for (i = 0; i < DS_MAX_SIZE; ++i) {
        CHECK(sqlist_insert(&array_list, array_list.length, (int)i));
    }
    CHECK(!sqlist_insert(&array_list, array_list.length, 999));

    CHECK(linklist_init(&linked_list));
    CHECK(linklist_insert(&linked_list, 0, 4));
    CHECK(linklist_insert(&linked_list, 1, 12));
    CHECK(linklist_insert(&linked_list, 1, 8));
    CHECK(linklist_get(&linked_list, 2, &value) && value == 12);
    CHECK(linklist_delete(&linked_list, 0, &value) && value == 4);
    CHECK(!linklist_delete(&linked_list, 2, &value));
    linklist_destroy(&linked_list);
    CHECK(linked_list.head == NULL && linked_list.length == 0);
}

static void test_stacks_and_queue(void)
{
    SeqStack array_stack;
    LinkStack linked_stack;
    CirQueue queue;
    int value;
    size_t i;

    seqstack_init(&array_stack);
    CHECK(!seqstack_pop(&array_stack, &value));
    CHECK(seqstack_push(&array_stack, 5));
    CHECK(seqstack_push(&array_stack, 9));
    CHECK(seqstack_peek(&array_stack, &value) && value == 9);
    CHECK(seqstack_pop(&array_stack, &value) && value == 9);
    seqstack_init(&array_stack);
    for (i = 0; i < DS_MAX_SIZE; ++i) {
        CHECK(seqstack_push(&array_stack, (int)i));
    }
    CHECK(!seqstack_push(&array_stack, 1000));

    linkstack_init(&linked_stack);
    CHECK(linkstack_push(&linked_stack, 7));
    CHECK(linkstack_push(&linked_stack, 11));
    CHECK(linkstack_pop(&linked_stack, &value) && value == 11);
    linkstack_destroy(&linked_stack);

    cirqueue_init(&queue);
    CHECK(cirqueue_is_empty(&queue));
    for (i = 0; i < DS_MAX_SIZE; ++i) {
        CHECK(cirqueue_enqueue(&queue, (int)i));
    }
    CHECK(!cirqueue_enqueue(&queue, 1000));
    for (i = 0; i < 30; ++i) {
        CHECK(cirqueue_dequeue(&queue, &value) && value == (int)i);
    }
    for (i = 100; i < 130; ++i) {
        CHECK(cirqueue_enqueue(&queue, (int)i));
    }
    for (i = 30; i < DS_MAX_SIZE; ++i) {
        CHECK(cirqueue_dequeue(&queue, &value) && value == (int)i);
    }
    for (i = 100; i < 130; ++i) {
        CHECK(cirqueue_dequeue(&queue, &value) && value == (int)i);
    }
    CHECK(cirqueue_is_empty(&queue));
}

static void test_strings(void)
{
    DsString text;
    DsString pattern;
    DsString empty;
    char text_buffer[46];
    char pattern_buffer[14];
    char too_long[DS_STRING_MAX + 2];
    unsigned int random_state = 19;
    size_t round;

    CHECK(ds_string_from_cstr(&text, "abababcabcac"));
    CHECK(ds_string_from_cstr(&pattern, "abcac"));
    CHECK(ds_string_from_cstr(&empty, ""));
    CHECK(ds_string_find_naive(&text, &pattern) == 7);
    CHECK(ds_string_find_kmp(&text, &pattern) == 7);
    CHECK(ds_string_from_cstr(&pattern, "ababd"));
    CHECK(ds_string_find_naive(&text, &pattern) == -1);
    CHECK(ds_string_find_kmp(&text, &pattern) == -1);
    CHECK(ds_string_find_naive(&text, &empty) == 0);
    CHECK(ds_string_find_kmp(&text, &empty) == 0);
    CHECK(!ds_string_from_cstr(&text, NULL));
    for (round = 0; round < sizeof(too_long); ++round) {
        too_long[round] = 'x';
    }
    too_long[sizeof(too_long) - 1] = '\0';
    CHECK(!ds_string_from_cstr(&text, too_long));

    /* 随机短串对拍，确保 KMP 与朴素算法对各种失配都给出相同结果。 */
    for (round = 0; round < 500; ++round) {
        size_t text_length = round % 45;
        size_t pattern_length = round % 13;
        size_t i;
        for (i = 0; i < text_length; ++i) {
            random_state = random_state * 1103515245u + 12345u;
            text_buffer[i] = (char)('a' + (random_state % 3));
        }
        for (i = 0; i < pattern_length; ++i) {
            random_state = random_state * 1103515245u + 12345u;
            pattern_buffer[i] = (char)('a' + (random_state % 3));
        }
        text_buffer[text_length] = '\0';
        pattern_buffer[pattern_length] = '\0';
        CHECK(ds_string_from_cstr(&text, text_buffer));
        CHECK(ds_string_from_cstr(&pattern, pattern_buffer));
        CHECK(ds_string_find_kmp(&text, &pattern) ==
              ds_string_find_naive(&text, &pattern));
    }
}

static void test_binary_tree(void)
{
    const int marker = INT_MIN;
    const int encoding[] = { 1, 2, marker, marker, 3, 4, marker, marker,
                             marker };
    const int expected_preorder[] = { 1, 2, 3, 4 };
    const int expected_inorder[] = { 2, 1, 4, 3 };
    const int expected_postorder[] = { 2, 4, 3, 1 };
    const int malformed[] = { 1, 2, marker };
    size_t index = 0;
    BiNode *root = NULL;
    VisitLog log = { { 0 }, 0 };

    CHECK(bitree_build_preorder(encoding, sizeof(encoding) / sizeof(encoding[0]),
                                &index, marker, &root));
    CHECK(index == sizeof(encoding) / sizeof(encoding[0]));
    CHECK(bitree_size(root) == 4);
    CHECK(bitree_height(root) == 3);
    bitree_preorder(root, record_tree_value, &log);
    check_log(&log, expected_preorder, 4);
    log.length = 0;
    bitree_inorder(root, record_tree_value, &log);
    check_log(&log, expected_inorder, 4);
    log.length = 0;
    bitree_postorder(root, record_tree_value, &log);
    check_log(&log, expected_postorder, 4);
    bitree_destroy(root);

    index = 0;
    root = NULL;
    CHECK(!bitree_build_preorder(malformed, 3, &index, marker, &root));
    CHECK(root == NULL);
}

static void test_graph(void)
{
    Graph graph;
    VisitLog log = { { 0 }, 0 };
    const int expected_dfs[] = { 0, 1, 3, 2 };
    const int expected_bfs[] = { 0, 1, 2, 3 };

    CHECK(graph_init(&graph, 5, false));
    CHECK(graph_add_edge(&graph, 0, 1));
    CHECK(graph_add_edge(&graph, 0, 2));
    CHECK(graph_add_edge(&graph, 1, 3));
    CHECK(graph_dfs(&graph, 0, record_vertex, &log));
    check_log(&log, expected_dfs, 4);
    log.length = 0;
    CHECK(graph_bfs(&graph, 0, record_vertex, &log));
    check_log(&log, expected_bfs, 4);
    CHECK(!graph_add_edge(&graph, 0, 5));
    CHECK(!graph_bfs(&graph, 5, record_vertex, &log));
}

typedef bool (*SortFn)(int *data, size_t length);

static void check_sort(SortFn sort)
{
    int values[] = { 9, -1, 4, 4, 0, 12, -8, 3 };
    size_t i;
    size_t round;

    CHECK(sort(NULL, 0));
    CHECK(sort(values, sizeof(values) / sizeof(values[0])));
    for (i = 1; i < sizeof(values) / sizeof(values[0]); ++i) {
        CHECK(values[i - 1] <= values[i]);
    }

    /* 多组含重复值的输入，重点检查快速排序的划分边界。 */
    for (round = 0; round < 40; ++round) {
        int generated[128];
        size_t length = round * 3 + 1;
        for (i = 0; i < length; ++i) {
            generated[i] = (int)((i * 37 + round * 19) % 23) - 11;
        }
        CHECK(sort(generated, length));
        for (i = 1; i < length; ++i) {
            CHECK(generated[i - 1] <= generated[i]);
        }
    }
    CHECK(!sort(NULL, 1));
}

static void test_search_and_sort(void)
{
    const int sorted[] = { -5, 0, 4, 4, 8, 20 };

    CHECK(search_sequential(sorted, 6, 4) == 2);
    CHECK(search_binary(sorted, 6, 8) == 4);
    CHECK(search_binary(sorted, 6, 7) == -1);
    check_sort(sort_insertion);
    check_sort(sort_selection);
    check_sort(sort_bubble);
    check_sort(sort_shell);
    check_sort(sort_quick);
    check_sort(sort_heap);
    check_sort(sort_merge);
}

int main(void)
{
    test_linear_structures();
    test_stacks_and_queue();
    test_strings();
    test_binary_tree();
    test_graph();
    test_search_and_sort();
    puts("全部数据结构测试通过。");
    return EXIT_SUCCESS;
}
