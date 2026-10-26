/* Dijkstra 알고리즘 — 지금 가장 가까운 정점의 거리를 확정해 나간다.
 *
 * 1. 정점 8개, 간선 16개 방향 그래프에서 정점 0으로부터의 최단 거리를
 *    구한다. 힙에서 꺼낼 때와 거리를 고칠 때마다 한 줄씩 찍는다.
 * 2. 음의 가중치가 있는 정점 4개짜리 그래프에서 틀린 답이 나오는 것을 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/06_dijkstra/dijkstra.out
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

static int weightOf(const Graph *g, int u, int v) {
    for (int i = 0; i < g->deg[u]; i++) {
        if (g->adj[u][i].v == v) {
            return g->adj[u][i].w;
        }
    }
    return INF;
}

void dijkstra(const Graph *g, int s, int dist[], int from[]) {
    int done[MAX_V];
    for (int v = 0; v < g->n; v++) {
        dist[v] = INF;
        from[v] = -1;
        done[v] = 0;
    }
    heapSize = 0;
    dist[s] = 0;
    push(0, s);

    while (heapSize > 0) {
        Item it = pop();
        int u = it.v;
        if (done[u]) {
            printf("pop %d (dist %d): skip, already done\n", u, it.key);
            continue;
        }
        done[u] = 1;
        printf("pop %d (dist %d): done\n", u, it.key);
        for (int i = 0; i < g->deg[u]; i++) {
            int v = g->adj[u][i].v;
            int nd = dist[u] + g->adj[u][i].w;
            if (done[v]) {
                if (nd < dist[v]) {
                    printf("  %d->%d: %d < %d, but %d is already done\n", u, v, nd, dist[v], v);
                }
            } else if (nd < dist[v]) {
                dist[v] = nd;
                from[v] = u;
                push(nd, v);
                printf("  %d->%d: dist[%d] = %d via %d\n", u, v, v, nd, u);
            } else {
                printf("  %d->%d: %d >= %d, keep\n", u, v, nd, dist[v]);
            }
        }
    }
}

static int pathTo(const int from[], int t, int path[]) {
    int rev[MAX_V];
    int n = 0;
    for (int x = t; x != -1; x = from[x]) {
        rev[n++] = x;
    }
    for (int i = 0; i < n; i++) {
        path[i] = rev[n - 1 - i];
    }
    return n;
}

int main(void) {
    static Graph g, neg;
    int dist[MAX_V], from[MAX_V], path[MAX_V];

    const int edges[][3] = {
        {0, 1, 5}, {0, 4, 9}, {0, 7, 8}, {1, 2, 12}, {1, 3, 15}, {1, 7, 4},
        {2, 3, 3}, {2, 6, 11}, {3, 6, 9}, {4, 5, 4}, {4, 6, 20}, {4, 7, 5},
        {5, 2, 1}, {5, 6, 13}, {7, 5, 6}, {7, 2, 7},
    };
    build(&g, 8, edges, 16);
    printf("V = 8, E = 16, start 0\n");
    dijkstra(&g, 0, dist, from);

    printf("\n  v  dist  from\n");
    for (int v = 0; v < g.n; v++) {
        if (from[v] == -1) {
            printf("  %d  %4d     -\n", v, dist[v]);
        } else {
            printf("  %d  %4d  %4d\n", v, dist[v], from[v]);
        }
    }
    int n = pathTo(from, 6, path);
    printf("path 0 -> 6:");
    for (int i = 0; i < n; i++) {
        printf(" %d", path[i]);
    }
    printf(" (%d)\n", dist[6]);

    /* 음의 가중치: 2->3이 -9 */
    const int negEdges[][3] = {{0, 1, 4}, {0, 3, 2}, {1, 2, 6}, {2, 3, -9}};
    build(&neg, 4, negEdges, 4);
    printf("\nnegative weight: 0->1 4, 0->3 2, 1->2 6, 2->3 -9\n");
    dijkstra(&neg, 0, dist, from);
    int real = weightOf(&neg, 0, 1) + weightOf(&neg, 1, 2) + weightOf(&neg, 2, 3);
    printf("dijkstra says dist[3] = %d, but 0->1->2->3 costs %d\n", dist[3], real);
    return 0;
}
