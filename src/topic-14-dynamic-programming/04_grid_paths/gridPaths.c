/* 격자 경로 — 장애물을 피해 (0, 0)에서 (5, 5)까지 가는 경로의 수.
 *
 * 한 번에 x+1 또는 y+1로만 움직인다. 메모이제이션(여기서 끝까지)과
 * 타뷸레이션(처음부터 여기까지) 두 방향으로 센다. 메모 없는 재귀의
 * 호출 수도 함께 찍는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-14-dynamic-programming/04_grid_paths/gridPaths.out
 */
#include <stdio.h>

#define MAX 16

static const int obstacles[][2] = {{2, 2}, {3, 3}, {4, 4}};
static const int numObstacles = 3;

static long long memo[MAX][MAX];
static long long dp[MAX][MAX];
static int useMemo; /* 0이면 표 없이 순수 재귀 */
static long calls;  /* pathsMemo가 불린 횟수 */
static int solved;  /* 메모가 새로 센 칸의 수 */

static int isObstacle(int x, int y) {
    for (int k = 0; k < numObstacles; k++) {
        if (obstacles[k][0] == x && obstacles[k][1] == y) {
            return 1;
        }
    }
    return 0;
}

/* (x, y)에서 (tx, ty)까지 가는 경로의 수 */
long long pathsMemo(int x, int y, int tx, int ty) {
    calls++;
    /* 목표 지점 도달 */
    if (x == tx && y == ty) {
        return 1;
    }
    /* 경계를 벗어나거나 장애물 */
    if (x > tx || y > ty || isObstacle(x, y)) {
        return 0;
    }
    if (useMemo && memo[x][y] != -1) {
        return memo[x][y];
    }
    long long count = pathsMemo(x + 1, y, tx, ty) + pathsMemo(x, y + 1, tx, ty);
    if (useMemo) {
        memo[x][y] = count;
        solved++;
    }
    return count;
}

/* dp[i][j] = (0, 0)에서 (i, j)까지 오는 경로의 수 */
long long pathsTab(int tx, int ty) {
    for (int i = 0; i <= tx; i++) {
        for (int j = 0; j <= ty; j++) {
            if (isObstacle(i, j)) {
                dp[i][j] = 0;
            } else if (i == 0 && j == 0) {
                dp[i][j] = 1;
            } else {
                long long up = i > 0 ? dp[i - 1][j] : 0;
                long long left = j > 0 ? dp[i][j - 1] : 0;
                dp[i][j] = up + left;
            }
        }
    }
    return dp[tx][ty];
}

int main(void) {
    int tx = 5, ty = 5;
    printf("grid %d x %d, (0, 0) -> (%d, %d), obstacles", tx + 1, ty + 1, tx, ty);
    for (int k = 0; k < numObstacles; k++) {
        printf(" (%d, %d)", obstacles[k][0], obstacles[k][1]);
    }
    printf("\n\n");

    useMemo = 0;
    calls = 0;
    long long plain = pathsMemo(0, 0, tx, ty);
    printf("plain recursion = %lld, calls = %ld\n", plain, calls);

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            memo[i][j] = -1;
        }
    }
    useMemo = 1;
    calls = 0;
    solved = 0;
    long long m = pathsMemo(0, 0, tx, ty);
    printf("memo            = %lld, calls = %ld, solved %d cells\n", m, calls, solved);

    long long t = pathsTab(tx, ty);
    printf("tab             = %lld\n", t);
    printf("\n");

    /* 표를 찍는다. 장애물은 # */
    printf("  x\\y");
    for (int j = 0; j <= ty; j++) {
        printf("%5d", j);
    }
    printf("\n");
    for (int i = 0; i <= tx; i++) {
        printf("%4d ", i);
        for (int j = 0; j <= ty; j++) {
            if (isObstacle(i, j)) {
                printf("%5s", "#");
            } else {
                printf("%5lld", dp[i][j]);
            }
        }
        printf("\n");
    }
    return 0;
}
