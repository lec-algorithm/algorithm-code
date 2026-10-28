/* LSD 기수 정렬 — 가장 낮은 자리부터 한 자리씩 안정 카운팅 정렬을 반복한다.
 *
 * 정수(10진 자리)와 길이가 같은 문자열(문자 한 글자가 한 자리) 두 가지를 정렬한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/01_lsd_radix_sort/lsdRadixSort.out
 */
#include <stdio.h>

#define RADIX 10   /* 10진수 한 자리의 값: 0~9 */
#define R 256      /* 문자 한 글자의 값: 0~255 */
#define MAX_N 16

static int width;  /* 출력할 때 맞출 자릿수 */

static void printNumbers(const char *label, const int a[], int n) {
    printf("%s:", label);
    for (int i = 0; i < n; i++) {
        printf(" %0*d", width, a[i]);
    }
    printf("\n");
}

static void printStrings(const char *label, char *s[], int n) {
    printf("%s:", label);
    for (int i = 0; i < n; i++) {
        printf(" %s", s[i]);
    }
    printf("\n");
}

static int getMax(const int a[], int n) {
    int max = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

/* exp 자리(1, 10, 100, ...)의 숫자를 키로 a를 안정 카운팅 정렬한다. */
void countingSortByDigit(int a[], int n, int exp) {
    int output[MAX_N];
    int count[RADIX] = {0};

    for (int i = 0; i < n; i++) {               /* 1) 세기 */
        count[(a[i] / exp) % RADIX]++;
    }
    for (int d = 1; d < RADIX; d++) {           /* 2) 누적 */
        count[d] += count[d - 1];
    }
    for (int i = n - 1; i >= 0; i--) {          /* 3) 뒤에서부터 배치: 안정 */
        int d = (a[i] / exp) % RADIX;
        output[--count[d]] = a[i];
    }
    for (int i = 0; i < n; i++) {
        a[i] = output[i];
    }
}

/* 가장 낮은 자리부터 자리마다 countingSortByDigit을 부른다. 패스마다 한 줄 찍는다. */
void lsdRadixSort(int a[], int n) {
    int max = getMax(a, n);
    for (int exp = 1; max / exp > 0; exp *= RADIX) {
        countingSortByDigit(a, n, exp);
        char label[32];
        snprintf(label, sizeof(label), "exp %-4d", exp);
        printNumbers(label, a, n);
    }
}

/* 길이가 모두 w인 문자열 s를 마지막 문자부터 한 글자씩 안정 카운팅 정렬한다. */
void lsdStringSort(char *s[], int n, int w) {
    char *aux[MAX_N];
    for (int d = w - 1; d >= 0; d--) {
        int count[R] = {0};
        for (int i = 0; i < n; i++) {
            count[(unsigned char)s[i][d]]++;
        }
        for (int c = 1; c < R; c++) {
            count[c] += count[c - 1];
        }
        for (int i = n - 1; i >= 0; i--) {
            aux[--count[(unsigned char)s[i][d]]] = s[i];
        }
        for (int i = 0; i < n; i++) {
            s[i] = aux[i];
        }
        char label[32];
        snprintf(label, sizeof(label), "char %d", d);
        printStrings(label, s, n);
    }
}

int main(void) {
    int a[] = {437, 42, 531, 8, 74};
    int n = sizeof(a) / sizeof(a[0]);
    width = 3;

    printNumbers("input   ", a, n);
    lsdRadixSort(a, n);
    printNumbers("sorted  ", a, n);
    printf("\n");

    char *s[] = {"dab", "add", "cab", "fad", "fee", "bad",
                 "dad", "bee", "fed", "bed", "ebb", "ace"};
    int m = sizeof(s) / sizeof(s[0]);

    printStrings("input ", s, m);
    lsdStringSort(s, m, 3);
    printStrings("sorted", s, m);
    return 0;
}
