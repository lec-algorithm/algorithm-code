/* Kruskal 알고리즘 — 가벼운 간선부터, 사이클을 만들지 않으면 넣는다.
 *
 * 정점 8개, 간선 16개 그래프의 최소 스패닝 트리를 구한다. 간선을 가중치
 * 순으로 정렬하고, 두 끝이 다른 덩어리(연결 요소)에 있으면 넣고 덩어리를
 * 합친다. 같은 덩어리면 사이클이 생기므로 버린다.
 *
 * 가중치는 원래 표(0.16 등)에 100을 곱한 정수로 다룬다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/03_mst_kruskal/kruskal.out
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_V 16
#define MAX_E 64

typedef struct {
    int u, v, w;
} Edge;

static int comp[MAX_V];

static int byWeight(const void *a, const void *b) {
    return ((const Edge *)a)->w - ((const Edge *)b)->w;
}

static void printWeight(int w) {
    printf("%d.%02d", w / 100, w % 100);
}

static void printComp(int n) {
    printf("comp [");
    for (int x = 0; x < n; x++) {
        printf(x > 0 ? " %d" : "%d", comp[x]);
    }
    printf("]");
}

/* 덩어리 a와 b를 합친다. 번호는 둘 중 작은 쪽(덩어리의 가장 작은 정점). */
static void merge(int n, int a, int b) {
    int small = a < b ? a : b;
    for (int x = 0; x < n; x++) {
        if (comp[x] == a || comp[x] == b) {
            comp[x] = small;
        }
    }
}

/* edges를 정렬해 Kruskal을 돌리고, 고른 간선을 mst에 담아 개수를 돌려준다. */
int kruskal(int n, Edge edges[], int m, Edge mst[]) {
    int k = 0;
    qsort(edges, (size_t)m, sizeof(Edge), byWeight);
    for (int v = 0; v < n; v++) {
        comp[v] = v;
    }
    for (int i = 0; i < m; i++) {
        Edge e = edges[i];
        printf("  %d-%d ", e.u, e.v);
        printWeight(e.w);
        if (comp[e.u] != comp[e.v]) {
            mst[k++] = e;
            merge(n, comp[e.u], comp[e.v]);
            printf("  add   ");
            printComp(n);
            printf("\n");
            if (k == n - 1) {
                printf("%d edges = V - 1, stop\n", k);
                break;
            }
        } else {
            printf("  skip  (%d and %d both in %d, cycle)\n", e.u, e.v, comp[e.u]);
        }
    }
    return k;
}

int main(void) {
    /* 입력 순서는 정렬되어 있지 않다. */
    Edge edges[MAX_E] = {
        {4, 5, 35}, {4, 7, 37}, {5, 7, 28}, {0, 7, 16}, {1, 5, 32}, {0, 4, 38},
        {2, 3, 17}, {1, 7, 19}, {0, 2, 26}, {1, 2, 36}, {1, 3, 29}, {2, 7, 34},
        {6, 2, 40}, {3, 6, 52}, {6, 0, 58}, {6, 4, 93},
    };
    const int n = 8, m = 16;
    Edge mst[MAX_V];

    printf("V = %d, E = %d\n", n, m);
    printf("edges by weight\n");
    int k = kruskal(n, edges, m, mst);

    int total = 0;
    printf("mst:");
    for (int i = 0; i < k; i++) {
        printf(" %d-%d", mst[i].u, mst[i].v);
        total += mst[i].w;
    }
    printf("\ntotal weight = ");
    printWeight(total);
    printf("\n");
    return 0;
}
