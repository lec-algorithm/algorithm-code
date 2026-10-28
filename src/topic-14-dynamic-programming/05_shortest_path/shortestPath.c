/* 최단 경로 — 간선을 정확히 k개 써서 u에서 v로 가는 가장 싼 길.
 *
 * 순수 재귀와 메모이제이션으로 같은 답을 구하고, 메모 표에 적힌
 * 칸을 찍는다. 가장 싼 첫 걸음(next)을 함께 적어 두었다가 길을
 * 복원한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-14-dynamic-programming/05_shortest_path/shortestPath.out
 */
#include <limits.h>
#include <stdio.h>

#define V 6         /* 정점 수 */
#define MAX_K 8     /* k의 최댓값 */
#define INF INT_MAX /* 갈 수 없음 */

/* graph[u][x]: u -> x 간선의 비용. 0이면 간선이 없다. */
static const int graph[V][V] = {
    {0, 1, 2, 0, 0, 0}, /* 0 -> 1 (1), 0 -> 2 (2) */
    {0, 0, 0, 6, 0, 0}, /* 1 -> 3 (6) */
    {0, 2, 0, 4, 3, 0}, /* 2 -> 1 (2), 2 -> 3 (4), 2 -> 4 (3) */
    {0, 0, 0, 0, 0, 3}, /* 3 -> 5 (3) */
    {0, 0, 0, 1, 0, 5}, /* 4 -> 3 (1), 4 -> 5 (5) */
    {0, 0, 0, 0, 0, 0}, /* 5: 나가는 간선 없음 */
};

static int memo[MAX_K + 1][V][V]; /* -1이면 아직 모른다 */
static int next[MAX_K + 1][V][V]; /* 가장 싼 첫 걸음 */
static int useMemo;               /* 0이면 표 없이 순수 재귀 */
static long calls;                /* shortestPath가 불린 횟수 */

static void printCost(int c) {
    if (c == INF) {
        printf("INF");
    } else {
        printf("%d", c);
    }
}

int shortestPath(int u, int v, int k) {
    calls++;
    /* 1. 기저: 간선을 다 썼다 */
    if (k == 0) {
        return u == v ? 0 : INF;
    }
    /* 2. 이미 구한 (k, u, v)면 바로 */
    if (useMemo && memo[k][u][v] != -1) {
        printf("  hit memo[%d][%d][%d] = ", k, u, v);
        printCost(memo[k][u][v]);
        printf("\n");
        return memo[k][u][v];
    }
    /* 3. 첫 간선 u -> x를 하나씩 골라 본다 */
    int best = INF;
    for (int x = 0; x < V; x++) {
        if (graph[u][x] == 0) {
            continue;
        }
        int r = shortestPath(x, v, k - 1);
        if (r != INF && graph[u][x] + r < best) {
            best = graph[u][x] + r;
            next[k][u][v] = x;
        }
    }
    /* 4. 적어 두고 돌려준다 */
    if (useMemo) {
        memo[k][u][v] = best;
    }
    return best;
}

int main(void) {
    int src = 0, dst = 3, k = 3;

    printf("edges:");
    int first = 1;
    for (int u = 0; u < V; u++) {
        for (int x = 0; x < V; x++) {
            if (graph[u][x] != 0) {
                printf(first ? " %d->%d (%d)" : ", %d->%d (%d)", u, x, graph[u][x]);
                first = 0;
            }
        }
    }
    printf("\n%d -> %d with exactly %d edges\n\n", src, dst, k);

    useMemo = 0;
    calls = 0;
    int plain = shortestPath(src, dst, k);
    printf("plain recursion = ");
    printCost(plain);
    printf(", calls = %ld\n", calls);

    for (int a = 0; a <= MAX_K; a++) {
        for (int b = 0; b < V; b++) {
            for (int c = 0; c < V; c++) {
                memo[a][b][c] = -1;
            }
        }
    }
    useMemo = 1;
    calls = 0;
    int m = shortestPath(src, dst, k);
    printf("memo            = ");
    printCost(m);
    printf(", calls = %ld\n", calls);

    /* 메모 표에 적힌 칸 (v = dst) */
    for (int a = 1; a <= k; a++) {
        printf("  k=%d:", a);
        int firstCell = 1;
        for (int u = 0; u < V; u++) {
            if (memo[a][u][dst] != -1) {
                printf(firstCell ? " %d->%d = " : ", %d->%d = ", u, dst);
                printCost(memo[a][u][dst]);
                firstCell = 0;
            }
        }
        printf("\n");
    }

    /* 5. 경로 복원: 적어 둔 첫 걸음을 따라간다 */
    if (m == INF) {
        printf("no path\n");
        return 0;
    }
    int path[MAX_K + 1];
    path[0] = src;
    int u = src;
    for (int a = k; a > 0; a--) {
        u = next[a][u][dst];
        path[k - a + 1] = u;
    }
    printf("path:");
    for (int i = 0; i <= k; i++) {
        printf(i == 0 ? " %d" : " -> %d", path[i]);
    }
    printf(" (");
    for (int i = 0; i < k; i++) {
        printf(i == 0 ? "%d" : " + %d", graph[path[i]][path[i + 1]]);
    }
    printf(" = %d)\n", m);
    return 0;
}
