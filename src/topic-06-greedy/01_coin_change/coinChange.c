/* 동전 거스름돈 — 언제나 가장 큰 동전부터 낸다 (그리디).
 *
 * 우리 동전(500·100·50·10)에서는 최적이다. 40원짜리 동전이 끼어
 * 있으면 같은 절차가 최적을 놓친다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-06-greedy/01_coin_change/coinChange.out
 */
#include <stdio.h>

#define MAX_COINS 8

/* coins는 큰 것부터 정렬되어 있다. used[i]에 coins[i]를 몇 개 냈는지
 * 적고, 쓴 동전의 총 개수를 돌려준다. */
int coinChange(const int coins[], int m, int amount, int used[]) {
    int count = 0;
    for (int i = 0; i < m; i++) {
        used[i] = amount / coins[i];
        count += used[i];
        amount -= used[i] * coins[i];
    }
    return count;
}

static void run(const int coins[], int m, int amount) {
    int used[MAX_COINS];
    int count = coinChange(coins, m, amount, used);

    printf("coins = {");
    for (int i = 0; i < m; i++) {
        printf(i == 0 ? "%d" : ", %d", coins[i]);
    }
    printf("}, amount = %d\n", amount);

    printf("greedy:");
    for (int i = 0; i < m; i++) {
        if (used[i] > 0) {
            printf(" %dx%d", coins[i], used[i]);
        }
    }
    printf(" -> %d coins\n", count);
}

int main(void) {
    int won[] = {500, 100, 50, 10};
    int odd[] = {50, 40, 10};

    run(won, 4, 1260);
    printf("\n");
    run(odd, 3, 80);
    return 0;
}
