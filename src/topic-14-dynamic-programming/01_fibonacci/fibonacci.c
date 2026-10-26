/* 피보나치 — 재귀, 메모이제이션, 타뷸레이션.
 *
 * 주제 01의 재귀가 느린 까닭은 같은 항을 몇 번이고 다시 구하기
 * 때문이었다. 답을 적어 두면(메모) 재귀도 O(n)이 된다. 작은 항부터
 * 표를 채워도(타뷸레이션) 같다. 호출 수를 세어 셋을 견준다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-14-dynamic-programming/01_fibonacci/fibonacci.out
 */
#include <stdio.h>

#define MAX_N 91 /* F(90)까지. F(92)를 넘으면 long long이 넘친다 */

static long long calls; /* 함수가 불린 횟수 */
static long long memo[MAX_N];
static long long dp[MAX_N];

/* 정의를 그대로 옮긴 재귀. O(2^n) */
long long fibRecursive(int n) {
    calls++;
    if (n <= 1) {
        return n;
    }
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

/* 메모이제이션. memo[k]가 -1이면 아직 모른다는 뜻이다. O(n) */
long long fibMemo(int n) {
    calls++;
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n]; /* 이미 구한 값: 다시 갈라지지 않는다 */
    }
    memo[n] = fibMemo(n - 1) + fibMemo(n - 2);
    return memo[n];
}

/* 타뷸레이션. dp[0]부터 dp[n]까지 빠짐없이 채운다. O(n) */
long long fibTabulation(int n) {
    if (n <= 1) {
        return n;
    }
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

static void resetMemo(void) {
    for (int i = 0; i < MAX_N; i++) {
        memo[i] = -1;
    }
}

static void printArray(const char *label, const long long a[], int n) {
    printf("  %-10s = [", label);
    for (int i = 0; i <= n; i++) {
        printf(i == 0 ? "%lld" : ", %lld", a[i]);
    }
    printf("]\n");
}

int main(void) {
    int n = 5;
    printf("F(%d)\n", n);

    calls = 0;
    long long r = fibRecursive(n);
    printf("  %-10s = %lld, calls = %lld\n", "recursive", r, calls);

    resetMemo();
    calls = 0;
    long long m = fibMemo(n);
    printf("  %-10s = %lld, calls = %lld\n", "memo", m, calls);
    printArray("memo", memo, n);

    long long t = fibTabulation(n);
    printf("  %-10s = %lld\n", "tabulation", t);
    printArray("dp", dp, n);

    printf("\n");
    printf(" n  recursive calls  memo calls                 F(n)\n");
    int ns[] = {10, 20, 30, 90};
    for (int i = 0; i < 4; i++) {
        int k = ns[i];
        char rec[24] = "-";
        if (k <= 30) { /* 90이면 재귀는 끝나지 않는다 */
            calls = 0;
            fibRecursive(k);
            snprintf(rec, sizeof rec, "%lld", calls);
        }
        resetMemo();
        calls = 0;
        long long value = fibMemo(k);
        printf("%3d %16s %11lld %20lld\n", k, rec, calls, value);
    }
    return 0;
}
