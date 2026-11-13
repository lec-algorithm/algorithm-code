/* 최소 스패닝 트리 시간 재기 — Kruskal 두 가지, Prim 두 가지를 희소 그래프와 완전 그래프에서.
 *
 *   Kruskal comp   정렬 + comp 배열 합치기(03_mst_kruskal)   O(E log E + V^2)
 *   Kruskal UF     정렬 + union-find(08_union_find)           O(E log E)
 *   Prim heap      최소 힙(05_mst_prim)                       O(E log V)
 *   Prim array     key 배열 V칸을 훑어 고른다                 O(V^2 + E)
 *
 * 그래프는 셋이다. 희소(E = 4V), 무작위 가중치 완전 그래프, 그리고 간선마다 key가
 * 줄어 힙에 가장 나쁜 완전 그래프.
 *
 * 값은 밀리초다. 컴퓨터마다 다르므로 비율을 본다. 네 방법이 낸 총 가중치가
 * 같은지 확인한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/09_mst_timing/mstTiming.out
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_SECONDS 0.2
#define INF 0x7fffffff

typedef struct {
    int u, v, w;
} Edge;

typedef struct {
    int v, w;
} Arc;

typedef struct {
    int key, v;
} Item;

/* 재는 동안 쓰는 그래프. 간선 목록과, 그것으로 만든 인접 목록(CSR) */
static int V, E;
static Edge *edges, *work;
static int *start; /* 정점 u의 이웃은 arcs[start[u]] .. arcs[start[u + 1] - 1] */
static Arc *arcs;
static int *parent, *size, *key;
static char *inTree;
static Item *heap;

/* ---- 난수와 그래프 ---- */
static unsigned int seed = 2026u;

static int rnd(int k) {
    seed = 1664525u * seed + 1013904223u;
    return (int)(seed % (unsigned int)k);
}

static void makeSparse(int n) {
    V = n;
    E = 4 * n;
    edges = malloc(sizeof(Edge) * (size_t)E);
    int e = 0;
    for (int v = 1; v < n; v++) { /* 앞의 정점에 이어 연결을 보장한다 */
        int u = rnd(v);
        edges[e++] = (Edge){u, v, rnd(1000000)};
    }
    while (e < E) {
        int u = rnd(n), v = rnd(n);
        if (u == v) continue;
        edges[e++] = (Edge){u, v, rnd(1000000)};
    }
}

static void makeComplete(int n) {
    V = n;
    E = n * (n - 1) / 2;
    edges = malloc(sizeof(Edge) * (size_t)E);
    int e = 0;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            edges[e++] = (Edge){u, v, rnd(1000000)};
        }
    }
}

/* 힙에 가장 나쁜 완전 그래프. u < v인 간선의 가중치를 (V - u) x V + v로 둔다.
 * Prim은 0, 1, 2, … 순으로 붙이고, u를 붙일 때마다 남은 모든 정점의 key가 줄어
 * 간선마다 한 번씩 힙에 넣는다. */
static void makeWorstComplete(int n) {
    V = n;
    E = n * (n - 1) / 2;
    edges = malloc(sizeof(Edge) * (size_t)E);
    int e = 0;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            edges[e++] = (Edge){u, v, (n - u) * n + v};
        }
    }
}

static void buildAdjacency(void) {
    start = calloc((size_t)V + 1, sizeof(int));
    arcs = malloc(sizeof(Arc) * 2 * (size_t)E);
    for (int i = 0; i < E; i++) {
        start[edges[i].u + 1]++;
        start[edges[i].v + 1]++;
    }
    for (int u = 0; u < V; u++) start[u + 1] += start[u];
    int *fill = malloc(sizeof(int) * (size_t)V);
    memcpy(fill, start, sizeof(int) * (size_t)V);
    for (int i = 0; i < E; i++) {
        Edge e = edges[i];
        arcs[fill[e.u]++] = (Arc){e.v, e.w};
        arcs[fill[e.v]++] = (Arc){e.u, e.w};
    }
    free(fill);
    work = malloc(sizeof(Edge) * (size_t)E);
    parent = malloc(sizeof(int) * (size_t)V);
    size = malloc(sizeof(int) * (size_t)V);
    key = malloc(sizeof(int) * (size_t)V);
    inTree = malloc((size_t)V);
    heap = malloc(sizeof(Item) * (2 * (size_t)E + 1));
}

static void freeGraph(void) {
    free(edges); free(work); free(start); free(arcs);
    free(parent); free(size); free(key); free(inTree); free(heap);
}

/* ---- Kruskal ---- */
static int byWeight(const void *a, const void *b) {
    const Edge *x = a, *y = b;
    if (x->w != y->w) return x->w < y->w ? -1 : 1;
    if (x->u != y->u) return x->u < y->u ? -1 : 1;
    return (x->v > y->v) - (x->v < y->v);
}

static long long kruskalComp(void) {
    int *comp = parent;
    memcpy(work, edges, sizeof(Edge) * (size_t)E);
    qsort(work, (size_t)E, sizeof(Edge), byWeight);
    for (int v = 0; v < V; v++) comp[v] = v;
    long long total = 0;
    int k = 0;
    for (int i = 0; i < E && k < V - 1; i++) {
        int a = comp[work[i].u], b = comp[work[i].v];
        if (a == b) continue;
        total += work[i].w;
        k++;
        int small = a < b ? a : b;
        for (int x = 0; x < V; x++) { /* 합치기: V칸을 다 훑는다 */
            if (comp[x] == a || comp[x] == b) comp[x] = small;
        }
    }
    return total;
}

static int find(int x) {
    int root = x;
    while (parent[root] != root) root = parent[root];
    while (parent[x] != root) {
        int next = parent[x];
        parent[x] = root;
        x = next;
    }
    return root;
}

static long long kruskalUF(void) {
    memcpy(work, edges, sizeof(Edge) * (size_t)E);
    qsort(work, (size_t)E, sizeof(Edge), byWeight);
    for (int v = 0; v < V; v++) {
        parent[v] = v;
        size[v] = 1;
    }
    long long total = 0;
    int k = 0;
    for (int i = 0; i < E && k < V - 1; i++) {
        int a = find(work[i].u), b = find(work[i].v);
        if (a == b) continue;
        total += work[i].w;
        k++;
        if (size[a] < size[b]) {
            int t = a;
            a = b;
            b = t;
        }
        parent[b] = a;
        size[a] += size[b];
    }
    return total;
}

/* ---- Prim ---- */
static int less(Item a, Item b) {
    return a.key != b.key ? a.key < b.key : a.v < b.v;
}

static long long primHeap(void) {
    int n = 0;
    for (int v = 0; v < V; v++) {
        key[v] = INF;
        inTree[v] = 0;
    }
    key[0] = 0;
    heap[n++] = (Item){0, 0};
    long long total = 0;
    while (n > 0) {
        Item top = heap[0];
        heap[0] = heap[--n];
        for (int i = 0;;) { /* sift down */
            int l = 2 * i + 1, r = l + 1, m = i;
            if (l < n && less(heap[l], heap[m])) m = l;
            if (r < n && less(heap[r], heap[m])) m = r;
            if (m == i) break;
            Item t = heap[i];
            heap[i] = heap[m];
            heap[m] = t;
            i = m;
        }
        int u = top.v;
        if (inTree[u]) continue; /* 더 가벼운 간선으로 이미 붙었다 */
        inTree[u] = 1;
        total += top.key;
        for (int j = start[u]; j < start[u + 1]; j++) {
            Arc a = arcs[j];
            if (!inTree[a.v] && a.w < key[a.v]) {
                key[a.v] = a.w;
                int i = n++; /* sift up */
                heap[i] = (Item){a.w, a.v};
                while (i > 0 && less(heap[i], heap[(i - 1) / 2])) {
                    Item t = heap[i];
                    heap[i] = heap[(i - 1) / 2];
                    heap[(i - 1) / 2] = t;
                    i = (i - 1) / 2;
                }
            }
        }
    }
    return total;
}

static long long primArray(void) {
    for (int v = 0; v < V; v++) {
        key[v] = INF;
        inTree[v] = 0;
    }
    key[0] = 0;
    long long total = 0;
    for (int step = 0; step < V; step++) {
        int u = -1;
        for (int v = 0; v < V; v++) { /* V칸을 훑어 가장 작은 key */
            if (!inTree[v] && (u < 0 || key[v] < key[u])) u = v;
        }
        inTree[u] = 1;
        total += key[u];
        for (int j = start[u]; j < start[u + 1]; j++) {
            Arc a = arcs[j];
            if (!inTree[a.v] && a.w < key[a.v]) key[a.v] = a.w;
        }
    }
    return total;
}

/* ---- 재기 ---- */
static long long expected;

/* 한 번 구하는 데 걸린 평균 시간(ms). 짧으면 여러 번 돌린다. 첫 번은 재지 않고 데운다. */
static double measure(long long (*mst)(void)) {
    mst();
    double spent = 0;
    int runs = 0;
    do {
        clock_t t0 = clock();
        long long total = mst();
        spent += (double)(clock() - t0) / CLOCKS_PER_SEC;
        runs++;
        if (expected < 0) expected = total;
        if (total != expected) {
            printf("different total weight\n");
            exit(1);
        }
    } while (spent < MIN_SECONDS);
    return spent / runs * 1000;
}

static void row(void) {
    buildAdjacency();
    expected = -1;
    double a = measure(kruskalComp), b = measure(kruskalUF), c = measure(primHeap), d = measure(primArray);
    printf("%7d %9d  %12.3f  %10.3f  %9.3f  %10.3f\n", V, E, a, b, c, d);
    freeGraph();
}

int main(void) {
    static const int sparse[] = {2000, 8000, 32000};
    static const int dense[] = {500, 1000, 2000};
    const char *head = "%7s %9s  %12s  %10s  %9s  %10s\n";
    printf("sparse, E = 4V (ms)\n");
    printf(head, "V", "E", "Kruskal comp", "Kruskal UF", "Prim heap", "Prim array");
    for (int i = 0; i < 3; i++) {
        makeSparse(sparse[i]);
        row();
    }
    printf("\ncomplete, E = V(V-1)/2 (ms)\n");
    printf(head, "V", "E", "Kruskal comp", "Kruskal UF", "Prim heap", "Prim array");
    for (int i = 0; i < 3; i++) {
        makeComplete(dense[i]);
        row();
    }
    printf("\ncomplete, every edge lowers a key (ms)\n");
    printf(head, "V", "E", "Kruskal comp", "Kruskal UF", "Prim heap", "Prim array");
    for (int i = 0; i < 3; i++) {
        makeWorstComplete(dense[i]);
        row();
    }
    return 0;
}
