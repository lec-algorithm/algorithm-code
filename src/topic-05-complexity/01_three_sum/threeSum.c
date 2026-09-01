/* 1-sum, 2-sum, 3-sum — 연산을 세서 실행 시간의 수학적 모델을 만든다.
 *
 * 대표 연산으로 배열 접근을 센다. n을 두 배로 하면 접근이 몇 배가
 * 되는지(배가 실험)가 시간 복잡도의 차수를 알려준다. 입력은 주제 04의
 * minstd 생성기로 만들어 C와 Python이 같은 배열을 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-05-complexity/01_three_sum/threeSum.out
 */
#include <stdint.h>
#include <stdio.h>

#define MAX_N 400

static int64_t state = 1;

static int64_t nextRandom(void) {
    state = state * 16807 % 2147483647;
    return state;
}

/* -100 이상 100 이하의 정수 n개를 만든다. */
static void fillArray(int a[], int n) {
    state = 1;
    for (int i = 0; i < n; i++) {
        a[i] = (int)(nextRandom() % 201) - 100;
    }
}

/* 0인 원소의 개수. accesses에 배열 접근 횟수를 누적한다. */
long oneSum(const int a[], int n, long *accesses) {
    long count = 0;
    for (int i = 0; i < n; i++) {
        (*accesses)++;
        if (a[i] == 0) {
            count++;
        }
    }
    return count;
}

/* 합이 0이 되는 쌍의 개수. */
long twoSum(const int a[], int n, long *accesses) {
    long count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            *accesses += 2;
            if (a[i] + a[j] == 0) {
                count++;
            }
        }
    }
    return count;
}

/* 합이 0이 되는 셋의 개수. */
long threeSum(const int a[], int n, long *accesses) {
    long count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                *accesses += 3;
                if (a[i] + a[j] + a[k] == 0) {
                    count++;
                }
            }
        }
    }
    return count;
}

int main(void) {
    int a[MAX_N];
    long acc1;
    long prev2 = 0;
    long prev3 = 0;

    fillArray(a, 100);
    acc1 = 0;
    printf("N = 100\n");
    printf("1-sum: count = %ld, accesses = %ld\n", oneSum(a, 100, &acc1), acc1);
    acc1 = 0;
    printf("2-sum: count = %ld, accesses = %ld\n", twoSum(a, 100, &acc1), acc1);
    acc1 = 0;
    printf("3-sum: count = %ld, accesses = %ld\n", threeSum(a, 100, &acc1), acc1);

    printf("\ndoubling N: accesses ratio\n");
    printf("%6s %12s %12s\n", "N", "2-sum", "3-sum");
    for (int n = 100; n <= MAX_N; n *= 2) {
        long acc2 = 0;
        long acc3 = 0;
        fillArray(a, n);
        twoSum(a, n, &acc2);
        threeSum(a, n, &acc3);
        if (prev2 > 0) {
            printf("%6d %12.2f %12.2f\n", n,
                   (double)acc2 / (double)prev2,
                   (double)acc3 / (double)prev3);
        }
        prev2 = acc2;
        prev3 = acc3;
    }
    return 0;
}
