/* MSD 기수 정렬 — 가장 높은 자리부터 버킷으로 나누고, 버킷마다 다음 자리로 재귀한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/02_msd_radix_sort/msdRadixSort.out
 */
#include <stdio.h>

#define RADIX 10
#define MAX_N 16
#define WIDTH 3            /* 출력할 때 맞출 자릿수 */

static int examined = 0;   /* 자리를 읽은 횟수: 카운팅 정렬이 원소 하나를 셀 때마다 1 */

static void printRange(const int a[], int lo, int hi) {
    for (int i = lo; i <= hi; i++) {
        printf(" %0*d", WIDTH, a[i]);
    }
    printf("\n");
}

/* a[lo..hi]를 exp 자리로 안정 카운팅 정렬하고, 숫자마다 몇 개였는지 size에 남긴다. */
void countingSortByDigit(int a[], int lo, int hi, int exp, int size[]) {
    int output[MAX_N];
    int count[RADIX] = {0};

    for (int i = lo; i <= hi; i++) {            /* 1) 세기 */
        count[(a[i] / exp) % RADIX]++;
        examined++;
    }
    for (int d = 0; d < RADIX; d++) {
        size[d] = count[d];
    }
    for (int d = 1; d < RADIX; d++) {           /* 2) 누적 */
        count[d] += count[d - 1];
    }
    for (int i = hi; i >= lo; i--) {            /* 3) 뒤에서부터 배치: 안정 */
        int d = (a[i] / exp) % RADIX;
        output[--count[d]] = a[i];
    }
    for (int i = lo; i <= hi; i++) {
        a[i] = output[i - lo];
    }
}

/* a[lo..hi]를 exp 자리부터 정렬한다. depth는 들여쓰기용 재귀 깊이다. */
void msdRecursive(int a[], int lo, int hi, int exp, int depth) {
    if (lo >= hi || exp == 0) {
        return;
    }

    int size[RADIX];
    countingSortByDigit(a, lo, hi, exp, size);

    printf("%*sexp %-3d [%d..%d]:", depth * 2, "", exp, lo, hi);
    printRange(a, lo, hi);
    printf("%*s  buckets:", depth * 2, "");
    for (int d = 0; d < RADIX; d++) {
        if (size[d] > 0) {
            printf(" %d=%d", d, size[d]);
        }
    }
    printf("\n");

    int start = lo;
    for (int d = 0; d < RADIX; d++) {
        if (size[d] > 1) {
            msdRecursive(a, start, start + size[d] - 1, exp / RADIX, depth + 1);
        }
        start += size[d];
    }
}

void msdRadixSort(int a[], int n) {
    int max = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    int exp = 1;
    while (max / exp >= RADIX) {
        exp *= RADIX;
    }
    msdRecursive(a, 0, n - 1, exp, 0);
}

int main(void) {
    int a[] = {170, 45, 75, 90, 2, 802, 24, 66};
    int n = sizeof(a) / sizeof(a[0]);

    printf("input :");
    printRange(a, 0, n - 1);
    msdRadixSort(a, n);
    printf("sorted:");
    printRange(a, 0, n - 1);
    printf("digits examined = %d (LSD: %d x %d = %d)\n",
           examined, n, WIDTH, n * WIDTH);
    return 0;
}
