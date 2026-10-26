/* A* 알고리즘 — f = g + h가 가장 작은 정점부터 열어 목표 쪽을 먼저 찾는다.
 *
 * Dijkstra와 같은 정점 8개, 간선 16개 그래프에서 0에서 6까지 간다.
 * h는 정점마다 6까지 남은 거리의 추정값이다. 이 h는 실제 거리를 넘지
 * 않지만(admissible) 간선 두 곳에서 일관성(consistent)이 깨진다.
 * 그래서 닫힌 정점을 다시 열지 않으면 26, 다시 열면 25가 나온다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/07_a_star/aStar.out
 */
#include <stdio.h>

#define MAX_V 16
#define MAX_HEAP 64
#define INF 1000000

typedef struct {
    int v, w;
} Arc;

typedef struct {
    int n;
    Arc adj[MAX_V][MAX_V];
    int deg[MAX_V];
} Graph;

typedef struct {
    int key, seq, v; /* key가 같으면 먼저 넣은 것(seq가 작은 것)이 먼저 */
} Item;

static Item heap[MAX_HEAP];
static int heapSize = 0;
static int seqCounter = 0;

/* ---- 최소 힙 (04_min_heap과 같은 절차, 원소만 Item) ---- */
static int less(Item a, Item b) {
    return a.key != b.key ? a.key < b.key : a.seq < b.seq;
}

static void push(int key, int v) {
    int i = heapSize++;
    heap[i].key = key;
    heap[i].seq = seqCounter++;
    heap[i].v = v;
    while (i > 0 && less(heap[i], heap[(i - 1) / 2])) {
        Item t = heap[i];
        heap[i] = heap[(i - 1) / 2];
        heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static Item pop(void) {
    Item top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    for (;;) {
        int m = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < heapSize && less(heap[l], heap[m])) m = l;
        if (r < heapSize && less(heap[r], heap[m])) m = r;
        if (m == i) break;
        Item t = heap[i];
        heap[i] = heap[m];
        heap[m] = t;
        i = m;
    }
    return top;
}

/* ---- 그래프 ---- */
static void build(Graph *g, int n, const int edges[][3], int m) {
    g->n = n;
    for (int v = 0; v < n; v++) {
        g->deg[v] = 0;
    }
    for (int k = 0; k < m; k++) {
        int u = edges[k][0];
        int i = g->deg[u]++;
        while (i > 0 && g->adj[u][i - 1].v > edges[k][1]) { /* 번호 오름차순 */
            g->adj[u][i] = g->adj[u][i - 1];
            i--;
        }
        g->adj[u][i].v = edges[k][1];
        g->adj[u][i].w = edges[k][2];
    }
}

/* t에 닿으면 g[t]를 돌려주고 경로를 path에, 길이를 *len에 담는다. */
int aStar(const Graph *gr, const int h[], int s, int t, int reopen, int path[], int *len) {
    int g[MAX_V], cameFrom[MAX_V], closed[MAX_V];
    int expanded[MAX_HEAP];
    int ne = 0;
    for (int v = 0; v < gr->n; v++) {
        g[v] = INF;
        cameFrom[v] = -1;
        closed[v] = 0;
    }
    heapSize = 0;
    seqCounter = 0;
    g[s] = 0;
    push(h[s], s);

    while (heapSize > 0) {
        Item it = pop();
        int u = it.v;
        if (closed[u]) {
            printf("pop %d (f %d): skip, already closed\n", u, it.key);
            continue;
        }
        printf("pop %d (f %d = %d + %d)", u, it.key, g[u], h[u]);
        if (u == t) {
            printf(": goal\n");
            int rev[MAX_V];
            int n = 0;
            for (int x = t; x != -1; x = cameFrom[x]) {
                rev[n++] = x;
            }
            for (int i = 0; i < n; i++) {
                path[i] = rev[n - 1 - i];
            }
            *len = n;
            printf("expanded:");
            for (int i = 0; i < ne; i++) {
                printf(" %d", expanded[i]);
            }
            printf("\n");
            return g[t];
        }
        printf(": close\n");
        closed[u] = 1;
        expanded[ne++] = u;
        for (int i = 0; i < gr->deg[u]; i++) {
            int v = gr->adj[u][i].v;
            int ng = g[u] + gr->adj[u][i].w;
            if (ng >= g[v]) {
                if (!closed[v]) {
                    printf("  %d: g %d >= %d, keep\n", v, ng, g[v]);
                }
                continue;
            }
            if (closed[v] && !reopen) {
                printf("  %d: g %d < %d, but %d is closed\n", v, ng, g[v], v);
                continue;
            }
            if (closed[v]) {
                printf("  %d: g %d < %d, reopen, f %d\n", v, ng, g[v], ng + h[v]);
                closed[v] = 0;
            } else {
                printf("  %d: g %d, f %d\n", v, ng, ng + h[v]);
            }
            g[v] = ng;
            cameFrom[v] = u;
            push(ng + h[v], v);
        }
    }
    *len = 0;
    return INF;
}

static void printPath(const int path[], int len, int cost) {
    printf("path:");
    for (int i = 0; i < len; i++) {
        printf(" %d", path[i]);
    }
    printf(", cost %d\n", cost);
}

int main(void) {
    static Graph g;
    const int edges[][3] = {
        {0, 1, 5}, {0, 4, 9}, {0, 7, 8}, {1, 2, 12}, {1, 3, 15}, {1, 7, 4},
        {2, 3, 3}, {2, 6, 11}, {3, 6, 9}, {4, 5, 4}, {4, 6, 20}, {4, 7, 5},
        {5, 2, 1}, {5, 6, 13}, {7, 5, 6}, {7, 2, 7},
    };
    const int h[] = {14, 10, 4, 2, 11, 6, 0, 8};
    build(&g, 8, edges, 16);

    printf("V = 8, E = 16, start 0, goal 6\n");
    printf("h:");
    for (int v = 0; v < g.n; v++) {
        printf(" %d=%d", v, h[v]);
    }
    printf("\ninconsistent edges:");
    for (int u = 0; u < g.n; u++) {
        for (int i = 0; i < g.deg[u]; i++) {
            int v = g.adj[u][i].v;
            int w = g.adj[u][i].w;
            if (h[u] > w + h[v]) {
                printf(" %d->%d (%d > %d + %d)", u, v, h[u], w, h[v]);
            }
        }
    }
    printf("\n");

    int path[MAX_V], len, cost;
    printf("\nwithout reopening closed nodes\n");
    cost = aStar(&g, h, 0, 6, 0, path, &len);
    printPath(path, len, cost);

    printf("\nwith reopening\n");
    cost = aStar(&g, h, 0, 6, 1, path, &len);
    printPath(path, len, cost);
    return 0;
}
