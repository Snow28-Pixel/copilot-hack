#include "ds.h"

static void dfs_visit(const Graph *graph, size_t vertex, bool *visited,
                      GraphVisitFn visit, void *context)
{
    size_t next;

    visited[vertex] = true;
    visit(vertex, context);
    for (next = 0; next < graph->vertex_count; ++next) {
        if (graph->adjacent[vertex][next] != 0 && !visited[next]) {
            dfs_visit(graph, next, visited, visit, context);
        }
    }
}

bool graph_init(Graph *graph, size_t vertex_count, bool directed)
{
    size_t row;
    size_t column;

    if (graph == NULL || vertex_count > DS_GRAPH_MAX_VERTICES) {
        return false;
    }
    graph->vertex_count = vertex_count;
    graph->directed = directed;
    for (row = 0; row < DS_GRAPH_MAX_VERTICES; ++row) {
        for (column = 0; column < DS_GRAPH_MAX_VERTICES; ++column) {
            graph->adjacent[row][column] = 0;
        }
    }
    return true;
}

bool graph_add_edge(Graph *graph, size_t from, size_t to)
{
    if (graph == NULL || from >= graph->vertex_count ||
        to >= graph->vertex_count) {
        return false;
    }
    graph->adjacent[from][to] = 1;
    if (!graph->directed) {
        graph->adjacent[to][from] = 1;
    }
    return true;
}

bool graph_dfs(const Graph *graph, size_t start, GraphVisitFn visit,
               void *context)
{
    bool visited[DS_GRAPH_MAX_VERTICES] = { false };

    if (graph == NULL || visit == NULL ||
        start >= graph->vertex_count ||
        graph->vertex_count > DS_GRAPH_MAX_VERTICES) {
        return false;
    }
    dfs_visit(graph, start, visited, visit, context);
    return true;
}

bool graph_bfs(const Graph *graph, size_t start, GraphVisitFn visit,
               void *context)
{
    bool visited[DS_GRAPH_MAX_VERTICES] = { false };
    size_t queue[DS_GRAPH_MAX_VERTICES];
    size_t front = 0;
    size_t rear = 0;

    if (graph == NULL || visit == NULL ||
        start >= graph->vertex_count ||
        graph->vertex_count > DS_GRAPH_MAX_VERTICES) {
        return false;
    }

    /* 入队时就标记，避免同一顶点被多个相邻点重复加入队列。 */
    visited[start] = true;
    queue[rear++] = start;
    while (front < rear) {
        size_t vertex = queue[front++];
        size_t next;

        visit(vertex, context);
        for (next = 0; next < graph->vertex_count; ++next) {
            if (graph->adjacent[vertex][next] != 0 && !visited[next]) {
                visited[next] = true;
                queue[rear++] = next;
            }
        }
    }
    return true;
}
