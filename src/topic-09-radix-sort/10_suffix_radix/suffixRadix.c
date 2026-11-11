/* 접미사 배열 — 더블링의 짝을 qsort 대신 LSD 기수 정렬 두 패스로 정렬한다.
 *
 * 짝 (순위, k/2칸 뒤의 순위)을 두 자리 수로 보고 뒷자리, 앞자리 순으로 안정 카운팅 정렬한다.
 * 결과가 접미사를 그대로 정렬한 것과 같은지 확인한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/10_suffix_radix/suffixRadix.out
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 64
#define MAX_VALUE 258   /* 값의 범위: -1을 0으로 민 문자 코드 또는 순위 */

static const char *gText;

/* sa를 key[sa[j]]로 안정 카운팅 정렬한다. key는 0 이상이다. */
static void countingPass(int sa[], int n, const int key[]) {
    int count[MAX_VALUE + 1] = {0};
    int output[MAX_N];
    for (int j = 0; j < n; j++) {
        count[key[sa[j]]]++;
    }
    for (int v = 1; v <= MAX_VALUE; v++) {
        count[v] += count[v - 1];
    }
    for (int j = n - 1; j >= 0; j--) {
        output[--count[key[sa[j]]]] = sa[j];
    }
    memcpy(sa, output, (size_t)n * sizeof(int));
}

/* 라운드마다 k와 서로 다른 순위의 수를 찍는다. 돌린 패스 수를 돌려준다. */
int buildSuffixArrayRadix(const char *text, int sa[], int n) {
    int rank[MAX_N], newRank[MAX_N], key0[MAX_N], key1[MAX_N];
    for (int i = 0; i < n; i++) {
        rank[i] = (unsigned char)text[i];
    }

    int passes = 0;
    for (int k = 2;; k *= 2) {
        int half = k / 2;
        for (int i = 0; i < n; i++) {
            sa[i] = i;
            key0[i] = rank[i] + 1;
            key1[i] = (i + half < n) ? rank[i + half] + 1 : 0;
        }
        countingPass(sa, n, key1);   /* 뒷자리 */
        countingPass(sa, n, key0);   /* 앞자리 */
        passes += 2;

        newRank[sa[0]] = 0;
        for (int j = 1; j < n; j++) {
            int same = key0[sa[j]] == key0[sa[j - 1]] && key1[sa[j]] == key1[sa[j - 1]];
            newRank[sa[j]] = newRank[sa[j - 1]] + (same ? 0 : 1);
        }
        memcpy(rank, newRank, (size_t)n * sizeof(int));

        int distinct = rank[sa[n - 1]] + 1;
        printf("  k = %-2d distinct ranks = %2d of %d\n", k, distinct, n);
        if (distinct == n) {
            return passes;
        }
    }
}

static int compareSuffix(const void *a, const void *b) {
    return strcmp(gText + *(const int *)a, gText + *(const int *)b);
}

static void check(const char *text) {
    int n = (int)strlen(text);
    int sa[MAX_N], naive[MAX_N];

    printf("%s (n = %d)\n", text, n);
    int passes = buildSuffixArrayRadix(text, sa, n);

    gText = text;
    for (int i = 0; i < n; i++) {
        naive[i] = i;
    }
    qsort(naive, (size_t)n, sizeof(int), compareSuffix);

    printf("  SA =");
    for (int j = 0; j < n; j++) {
        printf(" %d", sa[j]);
    }
    printf("\n  passes = %d, same as plain sort = %s\n",
           passes, memcmp(sa, naive, (size_t)n * sizeof(int)) == 0 ? "yes" : "no");
}

int main(void) {
    check("banana$");
    check("mississippi$");
    check("aaaaaaaa$");
    return 0;
}
