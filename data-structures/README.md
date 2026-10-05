# 考研数据结构 C 语言示例

本目录面向中国考研《数据结构》复习，使用标准 C11 实现常见结构和算法。代码优先展现核心思想与边界处理，不依赖第三方库，也不使用复杂的泛型框架。

## 目录结构

| 路径 | 内容 |
| --- | --- |
| `include/ds.h` | 类型定义、容量约定和公开接口 |
| `src/linear.c` | 顺序表、带头结点单链表 |
| `src/stack_queue.c` | 顺序栈、链栈、循环队列 |
| `src/string.c` | 定长串、朴素匹配、KMP |
| `src/tree.c` | 二叉树先序建树、三种遍历、结点数和高度 |
| `src/graph.c` | 邻接矩阵、深度优先与广度优先遍历 |
| `src/search_sort.c` | 顺序/折半查找和常用排序 |
| `tests/test_ds.c` | 边界条件和基本行为测试 |

## 编译和运行

需要 CMake 3.16 或更新版本，以及支持 C11 的 C 编译器（例如 GCC、Clang 或 Visual C）。

```sh
cmake -S data-structures -B data-structures/build
cmake --build data-structures/build
ctest --test-dir data-structures/build --output-on-failure
```

也可以在本目录下创建构建目录：`cmake -S . -B build`，然后构建并运行 `ctest --test-dir build --output-on-failure`。Windows 下可使用 Visual Studio Developer Command Prompt、Ninja 或已配置好的 CMake 生成器。

## 约定与错误处理

* 顺序表、顺序栈和循环队列的容量上限为 `DS_MAX_SIZE`（100）；定长串最多保存 `DS_STRING_MAX`（256）个字符；图最多有 `DS_GRAPH_MAX_VERTICES`（20）个顶点。
* 除树遍历的回调接口外，线性结构位置、字符串匹配结果、查找结果都采用从 0 开始的下标。插入位置允许等于元素个数（追加），读取/删除位置必须小于元素个数。
* `bool` 操作返回 `false` 表示参数无效、越界、结构已满/为空，或内存分配失败。删除、出栈、出队等函数要求提供非空输出指针；失败时不读取或写入无效结果。
* 单链表带一个不含数据的头结点；链表、链栈使用动态分配，使用完毕必须调用对应的 `destroy` 函数。
* 循环队列使用一个空闲单元区分队满与队空，所以数组长度是 `DS_MAX_SIZE + 1`，实际可存放 `DS_MAX_SIZE` 个元素。
* 二叉树由先序序列建成，空孩子用调用者指定的 `null_marker` 表示。例子通常用 `INT_MIN`（需包含 `<limits.h>`），因此该标记值不能同时作为树结点数据。建树游标 `index` 在调用前指向待解析项，成功后指向树编码结束处；编码不足或内存分配失败时返回 `false` 并释放本次已分配的结点。
* 图遍历从指定起点出发，仅访问该起点所在连通分量（有向图中为可达顶点）。邻接矩阵中顶点编号为 `0..vertex_count-1`；无向图添加边时会同时设置两个方向。
* 折半查找要求输入按非降序排列。查找接口未找到返回 `-1`；遇到重复关键字时折半查找返回其中一个匹配位置，顺序查找返回第一个。

## 接口速查

所有类型和函数声明都集中在 `include/ds.h`。插入、删除、出栈、出队等可能失败的修改接口返回 `bool`；读取/删除/弹出操作通过调用者传入的输出指针返回元素。

| 模块 | 主要接口 | 说明 |
| --- | --- | --- |
| 顺序表 | `sqlist_init`、`sqlist_insert`、`sqlist_delete`、`sqlist_get` | 插入位置 `0..length`；删除、读取位置 `0..length-1` |
| 单链表 | `linklist_init`、`linklist_insert`、`linklist_delete`、`linklist_get`、`linklist_destroy` | 带头结点；位置从首个数据结点起算 |
| 顺序栈/链栈 | `seqstack_init/push/pop/peek`、`linkstack_init/push/pop/destroy` | `pop`/`peek` 将栈顶元素写入输出参数 |
| 循环队列 | `cirqueue_init`、`cirqueue_enqueue`、`cirqueue_dequeue`、`cirqueue_is_empty` | 先进先出；队列满/空时入队/出队返回 `false` |
| 串 | `ds_string_from_cstr`、`ds_string_find_naive`、`ds_string_find_kmp` | 构造时拒绝超长输入；匹配返回 0-based 起点或 `-1` |
| 二叉树 | `bitree_build_preorder`、`bitree_destroy`、`bitree_preorder/inorder/postorder`、`bitree_size`、`bitree_height` | 遍历通过 `TreeVisitFn` 回调逐个传出结点值 |
| 图 | `graph_init`、`graph_add_edge`、`graph_dfs`、`graph_bfs` | 遍历通过 `GraphVisitFn` 回调传出顶点编号 |
| 查找 | `search_sequential`、`search_binary` | 返回匹配下标或 `-1`；折半查找要求有序 |
| 排序 | `sort_insertion`、`sort_selection`、`sort_bubble`、`sort_shell`、`sort_quick`、`sort_heap`、`sort_merge` | 原地升序排序；归并排序申请临时空间 |

## 实现与复杂度

`n` 表示线性结构/串/排序的数据量，`V` 为顶点数，`E` 为边数，`h` 为树高。

| 内容 | 核心操作 | 时间复杂度 | 额外空间 |
| --- | --- | --- | --- |
| 顺序表 | 按下标读取 O(1)；插入/删除 O(n) | O(1) / O(n) | O(1)，固定容量 |
| 带头结点单链表 | 按位置访问/插入/删除 O(n) | O(n) | 每个数据结点 O(1) |
| 顺序栈、链栈 | 入栈/出栈 O(1) | O(1) | 顺序栈固定容量；链栈每元素 O(1) |
| 循环队列 | 入队/出队 O(1) | O(1) | 固定容量 |
| 朴素串匹配 | 逐位置比较 | 最坏 O(nm) | O(1) |
| KMP | 前缀函数 + 一次扫描 | O(n+m) | O(m) |
| 二叉树遍历/计数/求高 | 递归访问每个结点 | O(n) | O(h) 递归栈 |
| 邻接矩阵 DFS/BFS | 扫描每个顶点的整行 | O(V²) | O(V) |
| 顺序查找 | 逐项比较 | O(n) | O(1) |
| 折半查找 | 每次排除一半区间 | O(log n) | O(1) |
| 直接插入排序 | 逐项插入已排序前缀 | 最好 O(n)，最坏 O(n²) | O(1) |
| 简单选择排序 | 每趟选择最小值 | O(n²) | O(1) |
| 冒泡排序 | 相邻交换，有序时提前结束 | 最好 O(n)，最坏 O(n²) | O(1) |
| 希尔排序 | 缩小增量的分组插入 | 取决于增量序列；此实现最坏 O(n²) | O(1) |
| 快速排序 | 三路划分，较短分区递归 | 平均 O(n log n)，最坏 O(n²) | O(log n) 递归栈 |
| 堆排序 | 建堆后反复取堆顶 | O(n log n) | O(1) |
| 归并排序 | 分治后合并有序区间 | O(n log n) | O(n) |

## 最小使用示例

```c
#include "ds.h"

int main(void)
{
    SqList list;
    int removed;

    sqlist_init(&list);
    if (!sqlist_insert(&list, 0, 42)) {
        return 1;
    }
    if (!sqlist_delete(&list, 0, &removed)) {
        return 1;
    }
    return removed == 42 ? 0 : 1;
}
```

可将该文件与 CMake 生成的 `ds` 静态库链接，也可以将 `src` 下的 C 文件直接加入自己的编译命令。
