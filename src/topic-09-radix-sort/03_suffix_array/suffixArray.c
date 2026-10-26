/* 접미사 배열 — 문자열의 모든 접미사를 사전순으로 정렬한 시작 위치의 배열.
 *
 * 단순한 방법(접미사 문자열을 그대로 정렬)과 더블링(앞 2, 4, 8, ...글자의
 * 순위를 두 배씩 늘려 가며 정렬) 두 가지로 만들고 결과가 같은지 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/03_suffix_array/suffixArray.out
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 64

static const char *gText;           /* qsort 비교 함수가 볼 문자열 */
static int gKey0[MAX_N], gKey1[MAX_N];  /* qsort 비교 함수가 볼 짝 (위치별) */

static int compareSuffix(const void *a, const void *b) {
    int i = *(const int *)a;
    int j = *(const int *)b;
    return strcmp(gText + i, gText + j);
}

/* 접미사 문자열끼리 비교해 정렬한다. */
void buildSuffixArrayNaive(const char *text, int sa[], int n) {
    for (int i = 0; i < n; i++) {
        sa[i] = i;
    }
    gText = text;
    qsort(sa, n, sizeof(int), compareSuffix);
}

/* 짝 (key0, key1) 순. 같으면 시작 위치 순으로 두어 C와 Python의 순서를 맞춘다. */
static int compareKey(const void *a, const void *b) {
    int i = *(const int *)a;
    int j = *(const int *)b;
    if (gKey0[i] != gKey0[j]) {
        return gKey0[i] - gKey0[j];
    }
    if (gKey1[i] != gKey1[j]) {
        return gKey1[i] - gKey1[j];
    }
    return i - j;
}

/* 앞 k글자의 순위를 k = 2, 4, 8, ...로 늘려 가며 정렬한다. 라운드마다 표를 찍는다. */
void buildSuffixArrayDoubling(const char *text, int sa[], int n) {
    int rank[MAX_N], newRank[MAX_N];
    for (int i = 0; i < n; i++) {
        rank[i] = (unsigned char)text[i];
        sa[i] = i;
    }

    for (int k = 2;; k *= 2) {
        int half = k / 2;
        for (int i = 0; i < n; i++) {
            gKey0[i] = rank[i];
            gKey1[i] = (i + half < n) ? rank[i + half] : -1;
        }
        qsort(sa, n, sizeof(int), compareKey);

        newRank[sa[0]] = 0;
        for (int j = 1; j < n; j++) {
            int same = gKey0[sa[j]] == gKey0[sa[j - 1]] && gKey1[sa[j]] == gKey1[sa[j - 1]];
            newRank[sa[j]] = newRank[sa[j - 1]] + (same ? 0 : 1);
        }
        for (int i = 0; i < n; i++) {
            rank[i] = newRank[i];
        }

        printf("k = %d\n", k);
        printf("   j  SA[j]  key         rank  suffix\n");
        for (int j = 0; j < n; j++) {
            int s = sa[j];
            printf("  %2d  %5d  (%3d, %3d)  %4d  %s\n",
                   j, s, gKey0[s], gKey1[s], rank[s], text + s);
        }
        int distinct = rank[sa[n - 1]] + 1;
        printf("  distinct ranks = %d of %d\n", distinct, n);

        if (distinct == n) {
            break;
        }
    }
}

static void printArray(const char *label, const int a[], int n) {
    printf("%s =", label);
    for (int i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");
}

int main(void) {
    const char *text = "banana$";
    int n = (int)strlen(text);
    int naive[MAX_N], doubling[MAX_N];

    printf("text = %s (n = %d)\n", text, n);
    for (int i = 0; i < n; i++) {
        printf("  %d %s\n", i, text + i);
    }
    printf("\n");

    buildSuffixArrayNaive(text, naive, n);
    printArray("naive    SA", naive, n);
    printf("\n");

    buildSuffixArrayDoubling(text, doubling, n);
    printf("\n");
    printArray("doubling SA", doubling, n);
    printf("same = %s\n", memcmp(naive, doubling, n * sizeof(int)) == 0 ? "yes" : "no");
    return 0;
}
