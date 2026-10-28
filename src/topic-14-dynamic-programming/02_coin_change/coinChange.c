/* 거스름돈 — 동적 계획법으로 최소 동전 개수를 구한다.
 *
 * 같은 점화식을 메모이제이션(하향식)과 타뷸레이션(상향식) 두 가지로
 * 푼다. 하향식은 필요한 금액만 풀고, 상향식은 모든 금액을 채운다.
 * 주제 06에서 그리디가 4개를 낸 {50, 40, 10}, 80원도 다시 푼다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-14-dynamic-programming/02_coin_change/coinChange.out
 */
#include <limits.h>
#include <stdio.h>

#define MAX_AMOUNT 1000
#define INF INT_MAX /* 만들 수 없는 금액 */

static int memo[MAX_AMOUNT + 1];
static int dp[MAX_AMOUNT + 1];
static int choice[MAX_AMOUNT + 1]; /* choice[a]: a원에서 마지막에 낸 동전 */
static int trace;                   /* 1이면 새로 푸는 금액을 찍는다 */
static int solved;                  /* 메모가 새로 푼 금액의 수 */

/* 하향식. memo[a]가 -1이면 아직 모른다. */
int coinChangeMemo(int amount, const int coins[], int m) {
    /* 1. 기저 */
    if (amount == 0) {
        return 0;
    }
    if (amount < 0) {
        return INF;
    }
    /* 2. 이미 구한 금액이면 바로 돌려준다 */
    if (memo[amount] != -1) {
        return memo[amount];
    }
    if (trace) {
        printf("  solve %d\n", amount);
    }
    solved++;
    /* 3. 동전마다 하나씩 내 보고 나머지를 재귀로 */
    int best = INF;
    for (int i = 0; i < m; i++) {
        int r = coinChangeMemo(amount - coins[i], coins, m);
        if (r != INF && r + 1 < best) {
            best = r + 1;
        }
    }
    /* 4. 적어 두고 돌려준다 */
    memo[amount] = best;
    return best;
}

/* 상향식. dp[0..amount]를 작은 금액부터 채운다. */
int coinChangeTab(int amount, const int coins[], int m) {
    /* 1. 초기화 */
    dp[0] = 0;
    for (int a = 1; a <= amount; a++) {
        dp[a] = INF;
        choice[a] = 0;
    }
    /* 2. 계산 */
    for (int a = 1; a <= amount; a++) {
        for (int i = 0; i < m; i++) {
            int c = coins[i];
            if (c <= a && dp[a - c] != INF && dp[a - c] + 1 < dp[a]) {
                dp[a] = dp[a - c] + 1;
                choice[a] = c;
            }
        }
    }
    /* 3. 반환 */
    return dp[amount];
}

static void printCell(int v, int first) {
    if (!first) {
        printf(", ");
    }
    if (v == INF) {
        printf("INF");
    } else {
        printf("%d", v);
    }
}

static void run(const int coins[], int m, int amount, int show) {
    printf("coins = {");
    for (int i = 0; i < m; i++) {
        printf(i == 0 ? "%d" : ", %d", coins[i]);
    }
    printf("}, amount = %d\n", amount);

    for (int a = 0; a <= amount; a++) {
        memo[a] = -1;
    }
    trace = show;
    solved = 0;
    int r = coinChangeMemo(amount, coins, m);
    printf("memo -> ");
    printCell(r, 1);
    printf(" coins, solved %d amounts\n", solved);
    if (show) {
        printf("  memo = [");
        for (int a = 0; a <= amount; a++) {
            printCell(memo[a], a == 0);
        }
        printf("]\n");
    }

    int t = coinChangeTab(amount, coins, m);
    if (t == INF) {
        printf("tab  -> INF, filled %d amounts\n", amount);
    } else {
        printf("tab  -> %d coins:", t);
        /* 4. 경로 복원: choice를 거꾸로 따라간다 */
        for (int a = amount; a > 0; a -= choice[a]) {
            printf(a == amount ? " %d" : " + %d", choice[a]);
        }
        printf(", filled %d amounts\n", amount);
    }
    if (show) {
        printf("  dp   = [");
        for (int a = 0; a <= amount; a++) {
            printCell(dp[a], a == 0);
        }
        printf("]\n");
    }
}

int main(void) {
    int small[] = {2, 3, 5};
    int odd[] = {50, 40, 10};

    run(small, 3, 6, 1);
    printf("\n");
    run(odd, 3, 80, 0);
    return 0;
}
