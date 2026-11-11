/* LSD와 MSD의 비용 세기 — 자리 읽기, 호출, 칸 훑기(호출 x k), 삽입 비교.
 *
 * 기수 k와, 삽입 정렬로 넘기는 구간 크기(cutoff)를 바꾸며 센다. 문자열 MSD는 읽은 글자를 센다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/08_msd_cost/msdCost.out
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_N 1000
#define MAX_K 256
#define MAX_LEVEL 12
#define MAX_WORDS 16

typedef struct {
    long reads, calls, scans, ins;
    long byLevel[MAX_LEVEL];   /* 자리별 호출 수 */
} Cost;

static uint32_t gOutput[MAX_N];

/* 시드가 고정된 무작위 값. 0 이상 2^32 미만이고 Python 구현과 같은 값이 나온다. */
static void randomValues(uint32_t a[], int n) {
    uint32_t s = 0x2f6e2b1u;
    for (int i = 0; i < n; i++) {
        s += 0x6d2b79f5u;
        uint32_t x = (s ^ (s >> 15)) * (1u | s);
        x = (x + (x ^ (x >> 7)) * (61u | x)) ^ x;
        a[i] = x ^ (x >> 14);
    }
}

static uint32_t maxOf(const uint32_t a[], int n) {
    uint32_t max = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

static int widthOf(uint32_t max, int k) {
    int d = 1;
    for (uint64_t e = (uint64_t)k; max >= e; e *= (uint64_t)k) {
        d++;
    }
    return d;
}

static uint32_t power(int k, int e) {
    uint32_t p = 1;
    for (int i = 0; i < e; i++) {
        p *= (uint32_t)k;
    }
    return p;
}

static void insertionSort(uint32_t a[], int lo, int hi, Cost *c) {
    for (int i = lo + 1; i <= hi; i++) {
        uint32_t v = a[i];
        int j = i - 1;
        while (j >= lo) {
            c->ins++;
            if (a[j] <= v) {
                break;
            }
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = v;
    }
}

static void msdRecursive(uint32_t a[], int lo, int hi, uint32_t exp, int k, int cutoff, int level, Cost *c) {
    if (lo >= hi || exp == 0) {
        return;
    }
    if (hi - lo + 1 <= cutoff) {
        insertionSort(a, lo, hi, c);
        return;
    }

    c->calls++;
    c->byLevel[level]++;
    c->scans += k;

    int size[MAX_K] = {0};
    int pos[MAX_K];
    for (int i = lo; i <= hi; i++) {
        size[(a[i] / exp) % (uint32_t)k]++;
        c->reads++;
    }
    pos[0] = size[0];
    for (int d = 1; d < k; d++) {
        pos[d] = pos[d - 1] + size[d];
    }
    for (int i = hi; i >= lo; i--) {
        gOutput[--pos[(a[i] / exp) % (uint32_t)k]] = a[i];
    }
    for (int i = lo; i <= hi; i++) {
        a[i] = gOutput[i - lo];
    }

    int start = lo;
    for (int d = 0; d < k; d++) {
        if (size[d] > 1) {
            msdRecursive(a, start, start + size[d] - 1, exp / (uint32_t)k, k, cutoff, level + 1, c);
        }
        start += size[d];
    }
}

static long total(const Cost *c) {
    return c->reads + c->scans + c->ins;
}

static void printRow(const char *name, const Cost *c) {
    printf("  %-8s reads %5ld  calls %3ld  scans %6ld  ins %5ld  total %6ld",
           name, c->reads, c->calls, c->scans, c->ins, total(c));
}

/* 값들을 기수 k의 자리로 보여 준다. 가장 큰 값의 자릿수에 맞춘다. */
static void printDigits(const uint32_t a[], int n, int k) {
    int width = widthOf(maxOf(a, n), k);
    printf("  k = %-3d %d digit%s ", k, width, width == 1 ? " " : "s");
    for (int i = 0; i < n; i++) {
        printf(" (");
        for (int p = width - 1; p >= 0; p--) {
            printf("%u%s", (a[i] / power(k, p)) % (uint32_t)k, p == 0 ? "" : ", ");
        }
        printf(")");
    }
    printf("\n");
}

static void printValues(const uint32_t a[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%s%u", i == 0 ? "" : " ", a[i]);
    }
}

static void compare(const uint32_t values[], int n, int k, int cutoff) {
    uint32_t a[MAX_N];
    int width = widthOf(maxOf(values, n), k);

    Cost lsd = {.reads = (long)n * width, .calls = width, .scans = (long)width * k};
    Cost msd = {0};
    memcpy(a, values, (size_t)n * sizeof(uint32_t));
    msdRecursive(a, 0, n - 1, power(k, width - 1), k, cutoff, 0, &msd);

    printRow("LSD", &lsd);
    printf("\n");
    char name[24];
    if (cutoff > 1) {
        snprintf(name, sizeof(name), "MSD <=%d", cutoff);
    } else {
        snprintf(name, sizeof(name), "MSD");
    }
    printRow(name, &msd);
    printf("  [");
    for (int i = 0; i < MAX_LEVEL && msd.byLevel[i] > 0; i++) {
        printf("%s%ld", i == 0 ? "" : " + ", msd.byLevel[i]);
    }
    printf("]\n");
}

/* 문자열 MSD. 끝난 단어는 어떤 글자보다 앞에 두고, 둘 이상인 글자 버킷만 다음 글자로 간다. */
static const char *gWords[MAX_WORDS];
static long gRead, gCalls;

static void msdWords(int lo, int hi, int pos) {
    gCalls++;
    int size[MAX_K + 1] = {0};
    int key[MAX_WORDS];
    for (int i = lo; i <= hi; i++) {
        size_t len = strlen(gWords[i]);
        key[i] = (size_t)pos < len ? (unsigned char)gWords[i][pos] + 1 : 0;
        if ((size_t)pos < len) {
            gRead++;
        }
        size[key[i]]++;
    }
    const char *out[MAX_WORDS];
    int at = 0;
    for (int d = 0; d <= MAX_K; d++) {
        for (int i = lo; i <= hi; i++) {
            if (key[i] == d) {
                out[at++] = gWords[i];
            }
        }
    }
    memcpy(&gWords[lo], out, (size_t)at * sizeof(const char *));

    int start = lo;
    for (int d = 0; d <= MAX_K; d++) {
        if (d > 0 && size[d] > 1) {
            msdWords(start, start + size[d] - 1, pos + 1);
        }
        start += size[d];
    }
}

static void countWords(const char *const words[], int n) {
    long all = 0;
    printf("  ");
    for (int i = 0; i < n; i++) {
        gWords[i] = words[i];
        all += (long)strlen(words[i]);
        printf("%s%s", i == 0 ? "" : " ", words[i]);
    }
    gRead = 0;
    gCalls = 0;
    msdWords(0, n - 1, 0);
    printf(": read %ld of %ld, calls %ld\n", gRead, all, gCalls);
}

int main(void) {
    static const uint32_t deck[] = {170, 45, 75, 90, 2, 802, 24, 66};
    static const uint32_t same[] = {111, 112, 113, 114};
    static const uint32_t apart[] = {100, 200, 300, 400};
    static const uint32_t apart256[] = {100, 300, 600, 900};
    static const uint32_t equal[] = {555, 555, 555, 555};
    static const struct { const uint32_t *a; int n; } inputs[] = {
        {deck, 8}, {same, 4}, {apart, 4}, {apart256, 4}, {equal, 4},
    };
    static const int radixes[] = {10, 256};

    printf("digits\n");
    for (int i = 0; i < 5; i++) {
        printf("  ");
        printValues(inputs[i].a, inputs[i].n);
        printf("\n");
        for (int r = 0; r < 2; r++) {
            printf("  ");
            printDigits(inputs[i].a, inputs[i].n, radixes[r]);
        }
    }

    printf("\ncost\n");
    for (int i = 0; i < 5; i++) {
        for (int r = 0; r < 2; r++) {
            printValues(inputs[i].a, inputs[i].n);
            printf(", k = %d\n", radixes[r]);
            compare(inputs[i].a, inputs[i].n, radixes[r], 0);
        }
    }

    static uint32_t values[MAX_N];
    randomValues(values, MAX_N);
    static const struct { int k, cutoff; } cases[] = {
        {10, 0}, {10, 9}, {10, 32}, {256, 0}, {256, 9}, {256, 16},
    };
    uint32_t min = values[0];
    for (int i = 1; i < MAX_N; i++) {
        if (values[i] < min) {
            min = values[i];
        }
    }
    printf("\n1000 random values, %u to %u\n", min, maxOf(values, MAX_N));
    for (int i = 0; i < 6; i++) {
        printf("k = %d\n", cases[i].k);
        compare(values, MAX_N, cases[i].k, cases[i].cutoff);
    }

    printf("\nstrings\n");
    static const char *const shells[] = {"she", "shells", "sea", "by", "the", "shore"};
    static const char *const animals[] = {"ant", "bee", "cat", "dog", "elk", "fox"};
    static const char *const seas[] = {"sea", "sea", "sea", "sea"};
    static const char *const cars[] = {"car", "card", "care", "cat"};
    countWords(shells, 6);
    countWords(animals, 6);
    countWords(seas, 4);
    countWords(cars, 4);
    return 0;
}
