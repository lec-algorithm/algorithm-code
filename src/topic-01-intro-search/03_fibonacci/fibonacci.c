/* 피보나치 — 같은 수열을 재귀와 반복으로.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-01-intro-search/03_fibonacci/fibonacci.out
 */
#include <stdio.h>
#include <time.h>

/* 호출 횟수를 세어 재귀가 얼마나 많이 갈라지는지 본다. */
static long calls;

/* 정의를 그대로 옮긴 재귀. O(2^n) */
long fibonacciRecursive(int n) {
    calls++;
    if (n <= 1) {
        return n;
    }
    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
}

/* 앞의 두 항만 들고 나아가는 반복. O(n) */
long fibonacciIterative(int n) {
    if (n <= 1) {
        return n;
    }
    long prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        long next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

/* 함수 하나를 재고 걸린 시간을 밀리초로 돌려준다. */
static double timeIt(long (*fn)(int), int n, long *out) {
    clock_t t0 = clock();
    *out = fn(n);
    return (double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
}

int main(void) {
    printf(" n   재귀 호출 수        재귀(ms)   반복(ms)   F(n)\n");
    printf("--- --------------- ---------- ---------- ----------\n");

    for (int n = 10; n <= 40; n += 10) {
        long rec, itr;

        calls = 0;
        double tRec = timeIt(fibonacciRecursive, n, &rec);
        double tItr = timeIt(fibonacciIterative, n, &itr);

        printf("%3d %15ld %10.2f %10.4f %10ld\n", n, calls, tRec, tItr, itr);

        if (rec != itr) { /* 두 구현은 같은 답을 내야 한다 */
            printf("  !! 값이 다르다: %ld vs %ld\n", rec, itr);
        }
    }

    printf("\nn이 10 늘 때마다 재귀 호출 수가 대략 100배가 된다.\n");
    printf("반복은 n에 비례해 늘 뿐이다.\n");
    return 0;
}
