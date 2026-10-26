/* 가장 긴 공통 부분 문자열 (LCS) — 두 문자열에 함께 나오는 가장 긴 부분 문자열.
 *
 * 두 문자열을 구분자 '#'로 이어 붙인 문자열 하나의 접미사 배열과 LCP 배열을
 * 만든다. 이웃한 두 접미사가 **서로 다른 문자열에서 시작할 때만** LCP를
 * 후보로 삼는다. 같은 문자열 안의 반복은 공통 부분 문자열이 아니다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/06_lcs/lcs.out
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

/* 접미사가 어느 문자열에서 시작하는지: 1, 2. 구분자 '#'나 끝 '$'에서 시작하면 0. */
static int origin(int pos, int len1, int n) {
    if (pos < len1) {
        return 1;
    }
    if (pos == len1 || pos == n - 1) {
        return 0;
    }
    return 2;
}

/* LCS의 길이를 돌려주고 시작 위치를 start, 자리 j를 at에 남긴다. 이웃 쌍마다 한 줄씩 찍는다. */
int findLongestCommonSubstring(const char *s1, const char *s2, char text[], int *start, int *at) {
    int len1 = (int)strlen(s1);
    snprintf(text, MAX_N, "%s#%s$", s1, s2);
    int n = (int)strlen(text);
    int sa[MAX_N], lcp[MAX_N];
    buildSuffixArray(text, sa, n);
    buildLcp(text, sa, n, lcp);

    printf("text = %s (n = %d, s1 = 0..%d, s2 = %d..%d)\n", text, n, len1 - 1, len1 + 1, n - 2);
    printf("   j  SA[j]  from  LCP[j]  suffix\n");
    int best = 0;
    *at = 0;
    for (int j = 0; j < n; j++) {
        int f = origin(sa[j], len1, n);
        char tag[2] = {f == 0 ? '-' : (char)('0' + f), '\0'};
        if (j == 0) {
            printf("  %2d  %5d  %4s  %6s  %s\n", j, sa[j], tag, "-", text + sa[j]);
            continue;
        }
        int g = origin(sa[j - 1], len1, n);
        const char *mark = "";
        if (f != 0 && g != 0 && f != g) {     /* 서로 다른 문자열에서 시작한 이웃 */
            mark = "  *";
            if (lcp[j] > best) {
                best = lcp[j];
                *at = j;
            }
        }
        printf("  %2d  %5d  %4s  %6d  %s%s\n", j, sa[j], tag, lcp[j], text + sa[j], mark);
    }

    int top = 0;
    for (int j = 1; j < n; j++) {
        if (lcp[j] > top) {
            top = lcp[j];
        }
    }
    printf("max LCP over all pairs = %d\n", top);
    *start = sa[*at];
    return best;
}

int main(void) {
    const char *s1 = "mississippi";
    const char *s2 = "kiss";
    char text[MAX_N];
    int start, at;

    printf("s1 = %s, s2 = %s\n", s1, s2);
    int length = findLongestCommonSubstring(s1, s2, text, &start, &at);
    printf("LCS = \"%.*s\" (length %d) at j = %d\n", length, text + start, length, at);
    return 0;
}
