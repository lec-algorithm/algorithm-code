/* 0/1 배낭 — 물건을 통째로만 담아 가치의 합을 최대로.
 *
 * 주제 06에서 그리디가 160에서 멈춘 문제다. dp[i][c] 표를 채우고,
 * 아래에서 위로 거꾸로 따라가 어떤 물건을 담았는지 복원한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-14-dynamic-programming/06_knapsack/knapsack.out
 */
#include <stdio.h>

#define MAX_ITEMS 8
#define MAX_W 100

/* dp[i][c]: 물건 1..i만 써서 용량 c에 담을 수 있는 최대 가치 */
static int dp[MAX_ITEMS + 1][MAX_W + 1];

/* 물건은 1번부터 센다. w[0], v[0]은 쓰지 않는다. */
int knapsack(const int w[], const int v[], int n, int capacity) {
    /* 1. 물건이 없으면 가치 0 */
    for (int c = 0; c <= capacity; c++) {
        dp[0][c] = 0;
    }
    /* 2. 물건을 하나씩 늘려 가며 */
    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= capacity; c++) {
            dp[i][c] = dp[i - 1][c]; /* 안 담는 경우 */
            if (w[i] <= c && dp[i - 1][c - w[i]] + v[i] > dp[i][c]) {
                dp[i][c] = dp[i - 1][c - w[i]] + v[i]; /* 담는 경우 */
            }
        }
    }
    return dp[n][capacity];
}

int main(void) {
    int w[] = {0, 10, 20, 30};
    int v[] = {0, 60, 100, 120};
    int n = 3, capacity = 50, step = 10;

    printf("capacity = %d\n", capacity);
    printf("items:");
    for (int i = 1; i <= n; i++) {
        printf(" (w=%d, v=%d)", w[i], v[i]);
    }
    printf("\n\n");

    int best = knapsack(w, v, n, capacity);

    printf("dp (columns every %d)\n", step);
    printf("  i\\c");
    for (int c = 0; c <= capacity; c += step) {
        printf("%5d", c);
    }
    printf("\n");
    for (int i = 0; i <= n; i++) {
        printf("  %-3d", i);
        for (int c = 0; c <= capacity; c += step) {
            printf("%5d", dp[i][c]);
        }
        printf("\n");
    }
    printf("\nbest = %d\n", best);

    /* 3. 경로 복원: 위 칸과 다르면 그 물건을 담은 것 */
    int c = capacity;
    for (int i = n; i >= 1; i--) {
        if (dp[i][c] != dp[i - 1][c]) {
            printf("  take item %d (w=%d, v=%d)\n", i, w[i], v[i]);
            c -= w[i];
        }
    }
    return 0;
}
