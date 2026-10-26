/* Prim 알고리즘 — 정점 하나에서 시작해 트리에 가장 가까운 정점을 붙여 간다.
 *
 * Kruskal과 같은 정점 8개, 간선 16개 그래프를 정점 0에서 시작한다.
 * 최소 힙(우선순위 큐)에서 key가 가장 작은 정점을 꺼내 트리에 붙이고,
 * 그 정점의 이웃 중 더 가벼운 간선을 찾으면 key와 via를 고쳐 힙에 넣는다.
 *
 * 가중치는 원래 표(0.16 등)에 100을 곱한 정수로 다룬다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/05_mst_prim/prim.out
 */
#include <stdio.h>

#define MAX_V 16
#define MAX_HEAP 64
#define INF 1000000

typedef struct {
    int v, w;
} Arc;

typedef struct {
    int key, seq, v; /* key가 같으면 먼저 넣은 것(seq가 작은 것)이 먼저 */
} Item;

static Arc adj[MAX_V][MAX_V];
static int deg[MAX_V];

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
static void addArc(int u, int v, int w) {
    int i = deg[u]++;
    while (i > 0 && adj[u][i - 1].v > v) { /* 이웃은 번호 오름차순 */
        adj[u][i] = adj[u][i - 1];
        i--;
    }
    adj[u][i].v = v;
    adj[u][i].w = w;
}

static void printWeight(int w) {
    printf("%d.%02d", w / 100, w % 100);
}

int prim(int n, int s, int key[], int via[]) {
    int inTree[MAX_V];
    int edges = 0, total = 0;
    for (int v = 0; v < n; v++) {
        key[v] = INF;
        via[v] = -1;
        inTree[v] = 0;
    }
    key[s] = 0;
    push(0, s);

    while (heapSize > 0 && edges < n - 1) {
        Item it = pop();
        int u = it.v;
        printf("pop %d (key ", u);
        printWeight(it.key);
        if (inTree[u]) {
            printf("): skip, already in tree\n");
            continue;
        }
        inTree[u] = 1;
        if (u == s) {
            printf("): start\n");
        } else {
            printf("): add %d-%d\n", via[u], u);
            edges++;
            total += it.key;
        }
        for (int i = 0; i < deg[u]; i++) {
            int v = adj[u][i].v;
            int w = adj[u][i].w;
            if (!inTree[v] && w < key[v]) {
                key[v] = w;
                via[v] = u;
                push(w, v);
                printf("  %d: key ", v);
                printWeight(w);
                printf(" via %d\n", u);
            }
        }
    }
    printf("%d edges = V - 1, stop\n", edges);
    return total;
}

int main(void) {
    const int edges[][3] = {
        {4, 5, 35}, {4, 7, 37}, {5, 7, 28}, {0, 7, 16}, {1, 5, 32}, {0, 4, 38},
        {2, 3, 17}, {1, 7, 19}, {0, 2, 26}, {1, 2, 36}, {1, 3, 29}, {2, 7, 34},
        {6, 2, 40}, {3, 6, 52}, {6, 0, 58}, {6, 4, 93},
    };
    const int n = 8, m = 16;
    for (int i = 0; i < m; i++) {
        addArc(edges[i][0], edges[i][1], edges[i][2]);
        addArc(edges[i][1], edges[i][0], edges[i][2]);
    }

    int key[MAX_V], via[MAX_V];
    printf("V = %d, E = %d, start 0\n", n, m);
    int total = prim(n, 0, key, via);

    printf("\n  v  key   via\n");
    for (int v = 0; v < n; v++) {
        printf("  %d  ", v);
        printWeight(key[v]);
        if (via[v] == -1) {
            printf("  -\n");
        } else {
            printf("  %d\n", via[v]);
        }
    }
    printf("total weight = ");
    printWeight(total);
    printf("\n");
    return 0;
}
