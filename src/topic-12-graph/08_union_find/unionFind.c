/* union-find로 하는 Kruskal — 덩어리를 트리로 들고, 뿌리가 같으면 같은 덩어리다.
 *
 * 03_mst_kruskal과 같은 정점 8개, 간선 16개 그래프를 쓴다. 덩어리 번호를 배열
 * 전체에 고쳐 쓰는 대신, 정점마다 부모(parent) 하나만 둔다. 부모를 따라 올라가
 * 닿는 뿌리가 그 덩어리의 이름이다.
 *   - 크기로 합치기(union by size): 작은 트리를 큰 트리의 뿌리 밑에 단다.
 *   - 경로 압축(path compression): Find가 지나온 정점을 뿌리에 바로 붙인다.
 *
 * 뒷부분은 무작위 큰 그래프에서 comp 배열이 훑는 칸 수와 union-find가 따라간
 * 링크 수를 센다. 시간이 아니라 세는 값이라 C와 Python의 출력이 같다.
 *
 * 가중치는 원래 표(0.16 등)에 100을 곱한 정수로 다룬다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/08_union_find/unionFind.out
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int u, v, w;
} Edge;

static int *parent;
static int *size;
static long long links; /* Find가 따라간 부모 링크 수 */

/* 추적용: 마지막 Find가 지나온 경로를 "3>2>0"처럼 적는다. */
static int tracing = 0;
static char path[128];

static int byWeight(const void *a, const void *b) {
    const Edge *x = a, *y = b;
    if (x->w != y->w) return x->w < y->w ? -1 : 1;
    if (x->u != y->u) return x->u < y->u ? -1 : 1;
    return (x->v > y->v) - (x->v < y->v);
}

static int find(int x) {
    int root = x;
    if (tracing) {
        snprintf(path, sizeof path, "%d", x);
    }
    while (parent[root] != root) {
        root = parent[root];
        links++;
        if (tracing) {
            size_t len = strlen(path);
            snprintf(path + len, sizeof path - len, ">%d", root);
        }
    }
    while (parent[x] != root) { /* 경로 압축 */
        int next = parent[x];
        parent[x] = root;
        x = next;
    }
    return root;
}

/* a, b는 서로 다른 뿌리. 붙인 쪽(아래로 간 뿌리)을 돌려준다. */
static int unite(int a, int b) {
    if (size[a] < size[b] || (size[a] == size[b] && a > b)) {
        int t = a;
        a = b;
        b = t;
    }
    parent[b] = a;
    size[a] += size[b];
    return b;
}

static void reset(int n) {
    for (int v = 0; v < n; v++) {
        parent[v] = v;
        size[v] = 1;
    }
    links = 0;
}

static void printWeight(int w) {
    printf("%d.%02d", w / 100, w % 100);
}

static void printParent(int n) {
    printf("parent [");
    for (int x = 0; x < n; x++) {
        printf(x > 0 ? " %d" : "%d", parent[x]);
    }
    printf("]");
}

/* 작은 그래프: 한 줄에 간선 하나씩, 두 Find의 경로와 결과를 찍는다. */
static int kruskalTrace(int n, Edge edges[], int m, Edge mst[]) {
    int k = 0;
    char pu[128];
    qsort(edges, (size_t)m, sizeof(Edge), byWeight);
    reset(n);
    tracing = 1;
    printf("edge      find u    find v    result\n");
    for (int i = 0; i < m; i++) {
        Edge e = edges[i];
        int before[16];
        memcpy(before, parent, sizeof(int) * (size_t)n);
        int a = find(e.u);
        strcpy(pu, path);
        int b = find(e.v);
        printf("%d-%d ", e.u, e.v);
        printWeight(e.w);
        printf("  %-9s %-9s ", pu, path);
        char result[32];
        if (a != b) {
            mst[k++] = e;
            int low = unite(a, b);
            snprintf(result, sizeof result, "add   %d under %d", low, parent[low]);
        } else {
            snprintf(result, sizeof result, "skip  root %d", a);
        }
        if (memcmp(before, parent, sizeof(int) * (size_t)n) != 0) {
            printf("%-15s  ", result); /* 부모가 바뀐 줄만 parent를 찍는다 */
            printParent(n);
        } else {
            printf("%s", result);
        }
        printf("\n");
        if (k == n - 1) {
            printf("%d edges = V - 1, stop\n", k);
            break;
        }
    }
    tracing = 0;
    return k;
}

/* ---- 큰 그래프 ---- */
static unsigned int seed;

static int rnd(int k) {
    seed = 1664525u * seed + 1013904223u;
    return (int)(seed % (unsigned int)k);
}

/* 1234567 -> "1,234,567" */
static const char *withCommas(long long x, char out[32]) {
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%lld", x), j = 0;
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) out[j++] = ',';
        out[j++] = buf[i];
    }
    out[j] = '\0';
    return out;
}

static void countLarge(int n) {
    int m = 4 * n;
    Edge *edges = malloc(sizeof(Edge) * (size_t)m);
    int e = 0;
    for (int v = 1; v < n; v++) { /* 앞의 정점에 이어 연결을 보장한다 */
        int u = rnd(v);
        edges[e++] = (Edge){u, v, rnd(1000000)};
    }
    while (e < m) {
        int u = rnd(n), v = rnd(n);
        if (u == v) continue;
        edges[e++] = (Edge){u, v, rnd(1000000)};
    }
    qsort(edges, (size_t)m, sizeof(Edge), byWeight);
    reset(n);
    long long finds = 0;
    int k = 0;
    for (int i = 0; i < m && k < n - 1; i++) {
        int a = find(edges[i].u), b = find(edges[i].v);
        finds += 2;
        if (a != b) {
            unite(a, b);
            k++;
        }
    }
    /* V, comp 배열이 훑는 칸(합치기 V-1번 x V칸), Find 수, 따라간 링크, Find 한 번에 평균 링크 */
    char c1[32], c2[32], c3[32], c4[32];
    printf("%9s %16s %9s %9s %10.2f\n", withCommas(n, c1), withCommas((long long)(n - 1) * n, c2),
           withCommas(finds, c3), withCommas(links, c4), (double)links / (double)finds);
    free(edges);
}

int main(void) {
    parent = malloc(sizeof(int) * 100000);
    size = malloc(sizeof(int) * 100000);

    /* 입력 순서는 정렬되어 있지 않다. */
    Edge edges[16] = {
        {4, 5, 35}, {4, 7, 37}, {5, 7, 28}, {0, 7, 16}, {1, 5, 32}, {0, 4, 38},
        {2, 3, 17}, {1, 7, 19}, {0, 2, 26}, {1, 2, 36}, {1, 3, 29}, {2, 7, 34},
        {6, 2, 40}, {3, 6, 52}, {6, 0, 58}, {6, 4, 93},
    };
    const int n = 8, m = 16;
    Edge mst[16];

    printf("V = %d, E = %d, union by size + path compression\n", n, m);
    int k = kruskalTrace(n, edges, m, mst);
    int total = 0;
    printf("mst:");
    for (int i = 0; i < k; i++) {
        printf(" %d-%d", mst[i].u, mst[i].v);
        total += mst[i].w;
    }
    printf("\ntotal weight = ");
    printWeight(total);
    printf("\nlinks followed = %lld (comp array: %d merges x %d cells = %d)\n", links, k, n, k * n);

    printf("\nrandom connected graphs, E = 4V, seed 2026\n");
    printf("%9s %16s %9s %9s %10s\n", "V", "comp cells", "finds", "links", "links/find");
    seed = 2026u;
    int sizes[] = {1000, 10000, 100000};
    for (int i = 0; i < 3; i++) {
        countLarge(sizes[i]);
    }
    free(parent);
    free(size);
    return 0;
}
