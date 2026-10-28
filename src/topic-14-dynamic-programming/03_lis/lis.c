/* 최장 증가 부분수열(LIS) — 메모이제이션과 타뷸레이션.
 *
 * 메모는 원소마다 "고른다 / 안 고른다"로 갈라지는 재귀에 (i, prev)
 * 표를 붙인다. 표가 실제로 몇 번 쓰이는지 찍어 본다. 타뷸레이션은
 * dp[i] = arr[i]로 끝나는 가장 긴 길이를 채우고, prev를 거꾸로 따라가
 * 수열 하나를 복원한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-14-dynamic-programming/03_lis/lis.out
 */
#include <stdio.h>
#include <string.h>

#define MAX_N 100

static int memo[MAX_N][MAX_N + 1]; /* memo[i][prev + 1] */
static int useMemo;                /* 0이면 표 없이 순수 재귀 */
static long calls;                 /* lisHelper가 불린 횟수 */
static int hits;                   /* 표에서 바로 꺼낸 횟수 */

static int max(int a, int b) {
    return a > b ? a : b;
}

/* arr[i..n-1]에서 더 이을 수 있는 최대 길이. prev는 마지막으로 고른 원소 */
int lisHelper(const int arr[], int n, int i, int prev) {
    calls++;
    /* 1. 기저 */
    if (i == n) {
        return 0;
    }
    /* 2. 메모 확인 */
    if (useMemo && memo[i][prev + 1] != -1) {
        hits++;
        printf("  hit memo[%d][%d] = %d\n", i, prev + 1, memo[i][prev + 1]);
        return memo[i][prev + 1];
    }
    /* 3. 고르지 않는 경우와 고르는 경우 */
    int exclude = lisHelper(arr, n, i + 1, prev);
    int include = 0;
    if (prev == -1 || arr[i] > arr[prev]) {
        include = 1 + lisHelper(arr, n, i + 1, i);
    }
    /* 4. 둘 중 큰 것을 적어 둔다 */
    int best = max(include, exclude);
    if (useMemo) {
        memo[i][prev + 1] = best;
    }
    return best;
}

int lisMemo(const int arr[], int n) {
    memset(memo, -1, sizeof(memo));
    return lisHelper(arr, n, 0, -1);
}

/* dp[i]: arr[i]로 끝나는 가장 긴 증가 부분수열의 길이. 끝 인덱스를 돌려준다. */
int lisTab(const int arr[], int n, int dp[], int prev[]) {
    /* 1. 초기화 */
    for (int i = 0; i < n; i++) {
        dp[i] = 1;
        prev[i] = -1;
    }
    /* 2. 앞의 원소를 모두 훑는다 */
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                prev[i] = j;
            }
        }
    }
    /* 3. 최댓값 찾기 (같으면 앞의 것) */
    int end = 0;
    for (int i = 1; i < n; i++) {
        if (dp[i] > dp[end]) {
            end = i;
        }
    }
    return end;
}

static void printArray(const char *label, const int a[], int n) {
    printf("%s[", label);
    for (int i = 0; i < n; i++) {
        printf(i == 0 ? "%d" : ", %d", a[i]);
    }
    printf("]\n");
}

int main(void) {
    int arr[] = {10, 9, 2, 5, 3, 7, 101, 18};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));
    printArray("arr = ", arr, n);
    printf("\n");

    useMemo = 0;
    calls = 0;
    int plain = lisHelper(arr, n, 0, -1);
    printf("plain recursion = %d, calls = %ld\n", plain, calls);

    printf("memo hits (i, prev + 1)\n");
    useMemo = 1;
    calls = 0;
    hits = 0;
    int m = lisMemo(arr, n);
    int filled = 0;
    for (int i = 0; i < n; i++) {
        for (int p = 0; p <= n; p++) {
            if (memo[i][p] != -1) {
                filled++;
            }
        }
    }
    printf("memo = %d, calls = %ld, hits = %d, filled %d of %d\n",
           m, calls, hits, filled, n * (n + 1));
    printf("\n");

    int dp[MAX_N], prev[MAX_N], seq[MAX_N];
    int end = lisTab(arr, n, dp, prev);
    printf("tab  = %d\n", dp[end]);
    printArray("  dp   = ", dp, n);
    printArray("  prev = ", prev, n);
    /* 4. 경로 복원: prev를 거꾸로 따라가며 모은다 */
    int len = 0;
    for (int k = end; k != -1; k = prev[k]) {
        seq[len++] = arr[k];
    }
    printf("  one LIS:");
    for (int k = len - 1; k >= 0; k--) {
        printf(" %d", seq[k]);
    }
    printf("\n");
    return 0;
}
