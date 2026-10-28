/* LCP 배열 (Kasai 알고리즘) — 정렬된 접미사 중 이웃한 둘의 최장 공통 접두사 길이.
 *
 * 텍스트 위치 순서로 접미사를 처리하면서, 앞 단계에서 센 길이 h를 하나만
 * 줄여 이어 쓴다. 이웃 쌍마다 처음부터 비교하는 방법과 글자 비교 횟수를 견준다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/04_lcp/lcp.out
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 64

static const char *gText;

static int compareSuffix(const void *a, const void *b) {
    return strcmp(gText + *(const int *)a, gText + *(const int *)b);
}

/* 접미사 배열은 03_suffix_array에서 만들었다. 여기서는 단순한 방법으로 만든다. */
void buildSuffixArray(const char *text, int sa[], int n) {
    for (int i = 0; i < n; i++) {
        sa[i] = i;
    }
    gText = text;
    qsort(sa, n, sizeof(int), compareSuffix);
}

/* Kasai 알고리즘. lcp[0]은 정의하지 않으므로 -1을 넣는다. 글자 비교 횟수를 돌려준다.
 * 처리한 위치마다 한 줄씩 찍는다. */
int buildLcpKasai(const char *text, const int sa[], int n, int lcp[]) {
    int rank[MAX_N];
    int compares = 0;
    for (int j = 0; j < n; j++) {
        rank[sa[j]] = j;
    }
    lcp[0] = -1;

    printf("   i  suffix    rank  j  start h  lcp  compares\n");
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] == 0) {
            h = 0;
            printf("  %2d  %-8s  %4d  -        -    -         -\n", i, text + i, rank[i]);
            continue;
        }
        int j = sa[rank[i] - 1];
        int start = h;
        int before = compares;
        while (i + h < n && j + h < n) {
            compares++;
            if (text[i + h] != text[j + h]) {
                break;
            }
            h++;
        }
        lcp[rank[i]] = h;
        printf("  %2d  %-8s  %4d  %d  %7d  %3d  %8d\n",
               i, text + i, rank[i], j, start, h, compares - before);
        if (h > 0) {
            h--;
        }
    }
    return compares;
}

/* 비교용: 이웃 쌍마다 처음부터 센다. 글자 비교 횟수를 돌려준다. */
int countNaiveCompares(const char *text, const int sa[], int n) {
    int compares = 0;
    for (int j = 1; j < n; j++) {
        int a = sa[j - 1], b = sa[j], h = 0;
        while (a + h < n && b + h < n) {
            compares++;
            if (text[a + h] != text[b + h]) {
                break;
            }
            h++;
        }
    }
    return compares;
}

int main(void) {
    const char *text = "banana$";
    int n = (int)strlen(text);
    int sa[MAX_N], lcp[MAX_N];

    buildSuffixArray(text, sa, n);
    printf("text = %s\n", text);
    printf("SA   =");
    for (int j = 0; j < n; j++) {
        printf(" %d", sa[j]);
    }
    printf("\n\n");

    int kasai = buildLcpKasai(text, sa, n, lcp);
    printf("\n");

    printf("   j  SA[j]  LCP[j]  suffix\n");
    for (int j = 0; j < n; j++) {
        if (j == 0) {
            printf("  %2d  %5d  %6s  %s\n", j, sa[j], "-", text + sa[j]);
        } else {
            printf("  %2d  %5d  %6d  %s\n", j, sa[j], lcp[j], text + sa[j]);
        }
    }
    printf("char compares: kasai %d, naive %d\n", kasai, countNaiveCompares(text, sa, n));
    return 0;
}
