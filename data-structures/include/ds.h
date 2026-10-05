#ifndef DS_H
#define DS_H

#include <stdbool.h>
#include <stddef.h>

#define DS_MAX_SIZE 100
#define DS_STRING_MAX 256
#define DS_GRAPH_MAX_VERTICES 20

/* ============================== 线性表 ============================== */

/* 顺序表的元素下标从 0 开始；length 表示当前元素个数。 */
typedef struct {
    int data[DS_MAX_SIZE];
    size_t length;
} SqList;

void sqlist_init(SqList *list);
bool sqlist_insert(SqList *list, size_t index, int value);
bool sqlist_delete(SqList *list, size_t index, int *deleted);
bool sqlist_get(const SqList *list, size_t index, int *value);

/* 单链表使用带头结点的表示法；位置下标仍从第一个数据结点的 0 开始。 */
typedef struct LinkNode {
    int data;
    struct LinkNode *next;
} LinkNode;

typedef struct {
    LinkNode *head;
    size_t length;
} LinkList;

bool linklist_init(LinkList *list);
void linklist_destroy(LinkList *list);
bool linklist_insert(LinkList *list, size_t index, int value);
bool linklist_delete(LinkList *list, size_t index, int *deleted);
bool linklist_get(const LinkList *list, size_t index, int *value);

/* ============================== 栈和队列 ============================ */

/* 顺序栈的 top 表示元素个数；栈顶对应 data[top - 1]。 */
typedef struct {
    int data[DS_MAX_SIZE];
    size_t top;
} SeqStack;

void seqstack_init(SeqStack *stack);
bool seqstack_push(SeqStack *stack, int value);
bool seqstack_pop(SeqStack *stack, int *value);
bool seqstack_peek(const SeqStack *stack, int *value);

/* 链栈以 top 指向栈顶结点，空栈时 top 为 NULL。 */
typedef struct {
    LinkNode *top;
    size_t size;
} LinkStack;

void linkstack_init(LinkStack *stack);
void linkstack_destroy(LinkStack *stack);
bool linkstack_push(LinkStack *stack, int value);
bool linkstack_pop(LinkStack *stack, int *value);

/*
 * 循环队列多留一个空单元，因此最多存放 DS_MAX_SIZE 个元素。
 * front 指向队头元素，rear 指向下一个可插入的位置。
 */
typedef struct {
    int data[DS_MAX_SIZE + 1];
    size_t front;
    size_t rear;
} CirQueue;

void cirqueue_init(CirQueue *queue);
bool cirqueue_enqueue(CirQueue *queue, int value);
bool cirqueue_dequeue(CirQueue *queue, int *value);
bool cirqueue_is_empty(const CirQueue *queue);

/* ============================== 串 ================================== */

/* 串使用定长字符数组保存，length 不包含末尾的 '\0'。 */
typedef struct {
    char data[DS_STRING_MAX + 1];
    size_t length;
} DsString;

bool ds_string_from_cstr(DsString *string, const char *source);
/* 返回模式串首次出现的 0-based 下标；未找到时返回 -1。 */
int ds_string_find_naive(const DsString *text, const DsString *pattern);
int ds_string_find_kmp(const DsString *text, const DsString *pattern);

/* ============================== 树 ================================== */

typedef struct BiNode {
    int data;
    struct BiNode *left;
    struct BiNode *right;
} BiNode;

typedef void (*TreeVisitFn)(int value, void *context);

/*
 * 按先序序列递归建树，null_marker 表示空孩子。
 * 例如 {1, 2, marker, marker, 3, marker, marker} 表示根为 1 的二叉树。
 * index 是调用者维护的游标；失败时已分配的结点会释放。
 */
bool bitree_build_preorder(const int *items, size_t count, size_t *index,
                           int null_marker, BiNode **root);
void bitree_destroy(BiNode *root);
void bitree_preorder(const BiNode *root, TreeVisitFn visit, void *context);
void bitree_inorder(const BiNode *root, TreeVisitFn visit, void *context);
void bitree_postorder(const BiNode *root, TreeVisitFn visit, void *context);
size_t bitree_size(const BiNode *root);
size_t bitree_height(const BiNode *root);

/* ============================== 图 ================================== */

/* 无权图的邻接矩阵；边存在记为 1，不存在记为 0。 */
typedef struct {
    size_t vertex_count;
    bool directed;
    unsigned char adjacent[DS_GRAPH_MAX_VERTICES][DS_GRAPH_MAX_VERTICES];
} Graph;

typedef void (*GraphVisitFn)(size_t vertex, void *context);

bool graph_init(Graph *graph, size_t vertex_count, bool directed);
bool graph_add_edge(Graph *graph, size_t from, size_t to);
bool graph_dfs(const Graph *graph, size_t start, GraphVisitFn visit,
               void *context);
bool graph_bfs(const Graph *graph, size_t start, GraphVisitFn visit,
               void *context);

/* ============================== 查找与排序 ========================== */

/* 返回首次匹配的 0-based 下标；未找到时返回 -1。 */
int search_sequential(const int *data, size_t length, int key);
/* 要求 data 按非降序排列。 */
int search_binary(const int *data, size_t length, int key);

bool sort_insertion(int *data, size_t length);
bool sort_selection(int *data, size_t length);
bool sort_bubble(int *data, size_t length);
bool sort_shell(int *data, size_t length);
bool sort_quick(int *data, size_t length);
bool sort_heap(int *data, size_t length);
/* 归并排序需要临时内存；分配失败时返回 false，原数组保持不变。 */
bool sort_merge(int *data, size_t length);

#endif
