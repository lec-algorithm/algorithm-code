/* 기수 정렬 시간 재기 — LSD, MSD, 작은 구간을 삽입 정렬로 넘기는 MSD (모두 R = 256).
 *
 * 값은 컴퓨터마다 다르므로 비율을 본다. 표의 값은 밀리초다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/09_radix_timing/radixTiming.out
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RADIX 256
#define DIGITS 4
#define CUTOFF 16
#define MIN_SECONDS 0.2   /* 이만큼 걸릴 때까지 되풀이해 평균을 낸다 */

static uint32_t *gBuffer;

/* 시드가 고정된 무작위 값. 0 이상 2^32 미만이다. */
static void randomValues(uint32_t a[], int n) {
    uint32_t s = 0x9e3779b9u;
    for (int i = 0; i < n; i++) {
        s += 0x6d2b79f5u;
        uint32_t x = (s ^ (s >> 15)) * (1u | s);
        x = (x + (x ^ (x >> 7)) * (61u | x)) ^ x;
        a[i] = x ^ (x >> 14);
    }
}

static void lsdSort(uint32_t a[], int n) {
    uint32_t *src = a;
    uint32_t *dst = gBuffer;
    for (int shift = 0; shift < 8 * DIGITS; shift += 8) {
        int count[RADIX] = {0};
        for (int i = 0; i < n; i++) {
            count[(src[i] >> shift) & 255]++;
        }
        for (int d = 1; d < RADIX; d++) {
            count[d] += count[d - 1];
        }
        for (int i = n - 1; i >= 0; i--) {
            dst[--count[(src[i] >> shift) & 255]] = src[i];
        }
        uint32_t *t = src;
        src = dst;
        dst = t;
    }
    /* 패스가 짝수 번이라 결과가 a로 돌아와 있다. */
}

static void msdRecursive(uint32_t a[], int lo, int hi, int shift, int cutoff) {
    if (lo >= hi || shift < 0) {
        return;
    }
    if (hi - lo + 1 <= cutoff) {
        for (int i = lo + 1; i <= hi; i++) {
            uint32_t v = a[i];
            int j = i - 1;
            while (j >= lo && a[j] > v) {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = v;
        }
        return;
    }

    int count[RADIX] = {0};
    int pos[RADIX];
    for (int i = lo; i <= hi; i++) {
        count[(a[i] >> shift) & 255]++;
    }
    int start = lo;
    for (int d = 0; d < RADIX; d++) {
        pos[d] = start;
        start += count[d];
    }
    for (int i = lo; i <= hi; i++) {
        gBuffer[pos[(a[i] >> shift) & 255]++] = a[i];
    }
    memcpy(&a[lo], &gBuffer[lo], (size_t)(hi - lo + 1) * sizeof(uint32_t));

    start = lo;
    for (int d = 0; d < RADIX; d++) {
        if (count[d] > 1) {
            msdRecursive(a, start, start + count[d] - 1, shift - 8, cutoff);
        }
        start += count[d];
    }
}

static void msdSort(uint32_t a[], int n) {
    msdRecursive(a, 0, n - 1, 8 * (DIGITS - 1), 0);
}

static void msdCutoffSort(uint32_t a[], int n) {
    msdRecursive(a, 0, n - 1, 8 * (DIGITS - 1), CUTOFF);
}

static int isSorted(const uint32_t a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            return 0;
        }
    }
    return 1;
}

/* 한 번 정렬하는 데 걸린 평균 시간(ms). 짧으면 여러 번 돌린다. */
static double measure(void (*sort)(uint32_t[], int), const uint32_t input[], uint32_t work[], int n) {
    double spent = 0;
    int runs = 0;
    do {
        memcpy(work, input, (size_t)n * sizeof(uint32_t));
        clock_t t0 = clock();
        sort(work, n);
        spent += (double)(clock() - t0) / CLOCKS_PER_SEC;
        runs++;
        if (!isSorted(work, n)) {
            printf("not sorted\n");
            exit(1);
        }
    } while (spent < MIN_SECONDS);
    return spent / runs * 1000;
}

int main(void) {
    static const int sizes[] = {1000, 10000, 100000, 1000000};
    printf("%9s  %10s  %10s  %12s\n", "n", "LSD", "MSD", "MSD <=16");
    for (int s = 0; s < 4; s++) {
        int n = sizes[s];
        uint32_t *input = malloc((size_t)n * sizeof(uint32_t));
        uint32_t *work = malloc((size_t)n * sizeof(uint32_t));
        gBuffer = malloc((size_t)n * sizeof(uint32_t));
        if (input == NULL || work == NULL || gBuffer == NULL) {
            return 1;
        }
        randomValues(input, n);
        printf("%9d  %10.3f  %10.3f  %12.3f\n", n,
               measure(lsdSort, input, work, n),
               measure(msdSort, input, work, n),
               measure(msdCutoffSort, input, work, n));
        free(input);
        free(work);
        free(gBuffer);
    }
    return 0;
}
