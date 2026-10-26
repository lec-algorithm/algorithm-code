/* 깊이 우선 검색(DFS) — 스택으로 한 갈래를 끝까지 파고든 뒤 돌아온다.
 *
 * 1. BFS와 같은 그래프에서 정점 0부터 DFS. 꺼낼 때마다 스택을 찍는다.
 * 2. 응용: 정점 13개 그래프의 연결 요소를 재귀 DFS로 센다.
 * 3. 응용: 과목 7개의 선수 관계(방향 그래프)를 위상 정렬한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/02_dfs/dfs.out
 */
#include <stdio.h>

#define MAX_V 16
#define MAX_STACK 64

typedef struct {
    int n;
    int adj[MAX_V][MAX_V];
    int deg[MAX_V];
} Graph;

static int marked[MAX_V];
static int from[MAX_V];

/* 간선 하나를 넣는다. 무방향이면 반대쪽에도 넣는다. 목록은 오름차순을 지킨다. */
static void insertSorted(Graph *g, int u, int v) {
    int i = g->deg[u]++;
    while (i > 0 && g->adj[u][i - 1] > v) {
        g->adj[u][i] = g->adj[u][i - 1];
        i--;
    }
    g->adj[u][i] = v;
}

static void build(Graph *g, int n, const int edges[][2], int m, int directed) {
    g->n = n;
    for (int v = 0; v < n; v++) {
        g->deg[v] = 0;
    }
    for (int i = 0; i < m; i++) {
        insertSorted(g, edges[i][0], edges[i][1]);
        if (!directed) {
            insertSorted(g, edges[i][1], edges[i][0]);
        }
    }
}

static void printStack(const int stack[][2], int top) {
    printf("stack [");
    for (int i = 0; i < top; i++) {
        printf(i > 0 ? " %d" : "%d", stack[i][0]);
    }
    printf("]\n");
}

/* s에서 스택 DFS. 스택 칸에는 (정점, 넣은 정점)을 둔다. */
void dfs(const Graph *g, int s, int order[]) {
    int stack[MAX_STACK][2];
    int top = 0, n = 0;
    for (int v = 0; v < g->n; v++) {
        marked[v] = 0;
        from[v] = -1;
    }
    stack[top][0] = s;
    stack[top][1] = -1;
    top++;
    printf("push %d, ", s);
    printStack(stack, top);

    while (top > 0) {
        top--;
        int v = stack[top][0];
        int p = stack[top][1];
        if (marked[v]) {
            printf("pop %d: already visited, ", v);
            printStack(stack, top);
            continue;
        }
        marked[v] = 1;
        from[v] = p;
        order[n++] = v;
        printf("pop %d: visit %d, check", v, v);
        for (int i = 0; i < g->deg[v]; i++) {
            printf(" %d", g->adj[v][i]);
        }
        int pushed = 0;
        for (int i = 0; i < g->deg[v]; i++) {
            int w = g->adj[v][i];
            if (!marked[w]) {
                printf(pushed++ == 0 ? " -> push %d" : " %d", w);
                stack[top][0] = w;
                stack[top][1] = v;
                top++;
            }
        }
        printf(", ");
        printStack(stack, top);
    }
}

/* 재귀 DFS: v에서 닿는 정점에 모두 같은 id를 준다. */
static void visit(const Graph *g, int v, int c, int id[]) {
    marked[v] = 1;
    id[v] = c;
    for (int i = 0; i < g->deg[v]; i++) {
        int w = g->adj[v][i];
        if (!marked[w]) {
            visit(g, w, c, id);
        }
    }
}

int components(const Graph *g, int id[]) {
    int count = 0;
    for (int v = 0; v < g->n; v++) {
        marked[v] = 0;
    }
    for (int v = 0; v < g->n; v++) {
        if (!marked[v]) {
            visit(g, v, count, id);
            count++;
        }
    }
    return count;
}

/* 나가는 간선을 다 본 뒤에 번호를 준다. 먼저 끝나는 정점이 뒤 번호를 받는다. */
static void finish(const Graph *g, int s, int f[], int *label) {
    marked[s] = 1;
    for (int i = 0; i < g->deg[s]; i++) {
        int v = g->adj[s][i];
        if (!marked[v]) {
            finish(g, v, f, label);
        }
    }
    f[s] = (*label)--;
}

void topologicalSort(const Graph *g, int f[], int order[]) {
    int label = g->n;
    for (int v = 0; v < g->n; v++) {
        marked[v] = 0;
    }
    for (int v = 0; v < g->n; v++) {
        if (!marked[v]) {
            finish(g, v, f, &label);
        }
    }
    for (int v = 0; v < g->n; v++) {
        order[f[v] - 1] = v;
    }
}

int main(void) {
    static Graph g, g13, dag;

    /* 1. BFS 예제와 같은 그래프 */
    const int edges[][2] = {
        {0, 1}, {0, 2}, {0, 5}, {0, 6}, {2, 3}, {3, 4}, {3, 5}, {4, 5}, {4, 6},
    };
    build(&g, 7, edges, 9, 0);
    int order[MAX_V];
    printf("DFS(0)\n");
    dfs(&g, 0, order);
    printf("order:");
    for (int i = 0; i < g.n; i++) {
        printf(" %d", order[i]);
    }
    printf("\n\n  v  from\n");
    for (int v = 0; v < g.n; v++) {
        if (from[v] == -1) {
            printf("  %d     -\n", v);
        } else {
            printf("  %d  %4d\n", v, from[v]);
        }
    }

    /* 2. 연결 요소: 정점 13개, 간선 13개 */
    const int edges13[][2] = {
        {0, 1}, {0, 2}, {0, 5}, {0, 6}, {3, 4}, {3, 5}, {4, 5},
        {4, 6}, {7, 8}, {9, 10}, {9, 11}, {9, 12}, {11, 12},
    };
    build(&g13, 13, edges13, 13, 0);
    int id[MAX_V];
    int count = components(&g13, id);
    printf("\nconnected components (V = 13, E = 13)\n");
    for (int c = 0; c < count; c++) {
        printf("  id %d:", c);
        for (int v = 0; v < g13.n; v++) {
            if (id[v] == c) {
                printf(" %d", v);
            }
        }
        printf("\n");
    }
    printf("count = %d\n", count);

    /* 3. 위상 정렬: 과목 7개, 선수 관계 11개 (u -> v: u를 먼저 듣는다) */
    const char *names[] = {
        "Algorithms", "Complexity Theory", "Artificial Intelligence", "Intro to CS",
        "Cryptography", "Scientific Computing", "Advanced Programming",
    };
    const int prereq[][2] = {
        {0, 1}, {0, 2}, {0, 5}, {1, 4}, {3, 2}, {3, 4},
        {3, 5}, {3, 6}, {5, 2}, {6, 0}, {6, 4},
    };
    build(&dag, 7, prereq, 11, 1);
    int f[MAX_V];
    int topo[MAX_V];
    topologicalSort(&dag, f, topo);
    printf("\ntopological sort (V = 7, E = 11, directed)\n  f:");
    for (int v = 0; v < dag.n; v++) {
        printf(" %d=%d", v, f[v]);
    }
    printf("\n  order:");
    for (int i = 0; i < dag.n; i++) {
        printf(" %d", topo[i]);
    }
    printf("\n");
    for (int i = 0; i < dag.n; i++) {
        printf("  %d. %s\n", i + 1, names[topo[i]]);
    }
    return 0;
}
