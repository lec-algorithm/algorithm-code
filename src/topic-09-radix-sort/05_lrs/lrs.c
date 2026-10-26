/* 가장 긴 반복 부분 문자열 (LRS) — 한 문자열에 두 번 이상 나오는 가장 긴 부분 문자열.
 *
 * 접미사를 정렬하면 같은 부분 문자열로 시작하는 접미사들이 이웃하게 모인다.
 * 그래서 LCP 배열의 최댓값이 답이다. 모든 위치 쌍을 비교하는 방법과 견준다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/05_lrs/lrs.out
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 64

static const char *gText;

static int compareSuffix(const void *a, const void *b) {
    return strcmp(gText + *(const int *)a, gText + *(const int *)b);
}

/* 접미사 배열 (03_suffix_array). 여기서는 단순한 방법으로 만든다. */
void buildSuffixArray(const char *text, int sa[], int n) {
    for (int i = 0; i < n; i++) {
        sa[i] = i;
    }
    gText = text;
    qsort(sa, n, sizeof(int), compareSuffix);
}

/* Kasai 알고리즘 (04_lcp). lcp[0]은 -1. */
void buildLcp(const char *text, const int sa[], int n, int lcp[]) {
    int rank[MAX_N];
    for (int j = 0; j < n; j++) {
        rank[sa[j]] = j;
    }
    lcp[0] = -1;
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] == 0) {
            h = 0;
            continue;
        }
        int j = sa[rank[i] - 1];
        while (i + h < n && j + h < n && text[i + h] == text[j + h]) {
            h++;
        }
        lcp[rank[i]] = h;
        if (h > 0) {
            h--;
        }
    }
}

/* LCP의 최댓값을 돌려주고 그 자리 j를 at에 남긴다. 같은 최댓값이면 앞의 것. */
int findLongestRepeatedSubstring(const int lcp[], int n, int *at) {
    int best = 0;
    *at = 0;
    for (int j = 1; j < n; j++) {
        if (lcp[j] > best) {
            best = lcp[j];
            *at = j;
        }
    }
    return best;
}

/* 모든 위치 쌍 (i, j)의 공통 접두사를 센다. 길이를 돌려주고 시작·쌍 수·비교 수를 남긴다. */
int lrsBruteForce(const char *text, int n, int *start, int *pairs, int *compares) {
    int best = 0;
    *start = 0;
    *pairs = 0;
    *compares = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            (*pairs)++;
            int h = 0;
            while (j + h < n) {
                (*compares)++;
                if (text[i + h] != text[j + h]) {
                    break;
                }
                h++;
            }
            if (h > best) {
                best = h;
                *start = i;
            }
        }
    }
    return best;
}

static void report(const char *text) {
    int n = (int)strlen(text);
    int sa[MAX_N], lcp[MAX_N];
    buildSuffixArray(text, sa, n);
    buildLcp(text, sa, n, lcp);

    printf("text = %s (n = %d)\n", text, n);
    printf("   j  SA[j]  LCP[j]  suffix\n");
    for (int j = 0; j < n; j++) {
        if (j == 0) {
            printf("  %2d  %5d  %6s  %s\n", j, sa[j], "-", text + sa[j]);
        } else {
            printf("  %2d  %5d  %6d  %s\n", j, sa[j], lcp[j], text + sa[j]);
        }
    }

    int at;
    int best = findLongestRepeatedSubstring(lcp, n, &at);
    printf("suffix sort: max LCP = %d at j = %d -> \"%.*s\" (positions %d, %d), %d adjacent pairs\n",
           best, at, best, text + sa[at], sa[at - 1], sa[at], n - 1);

    int start, pairs, compares;
    int length = lrsBruteForce(text, n, &start, &pairs, &compares);
    printf("brute force: \"%.*s\", %d pairs, %d char compares\n",
           length, text + start, pairs, compares);
}

int main(void) {
    report("banana$");
    printf("\n");
    report("aacaagtttacaagc$");
    return 0;
}
