/* 순차 검색 — 앞에서부터 하나씩 비교한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-01-intro-search/01_sequential_search/sequentialSearch.out
 */
#include <stdio.h>

/* key를 찾으면 그 인덱스를, 없으면 -1을 돌려준다.
 * comparisons에는 비교한 횟수를 누적한다. */
int sequentialSearch(const int a[], int n, int key, int *comparisons) {
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (a[i] == key) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    int a[] = {6, 13, 14, 25, 33, 43, 51, 53, 64, 72, 84, 93, 95, 96, 97};
    int n = sizeof(a) / sizeof(a[0]);
    int key = 51;
    int comparisons = 0;

    int index = sequentialSearch(a, n, key, &comparisons);

    printf("n = %d, key = %d\n", n, key);
    printf("index = %d, comparisons = %d\n", index, comparisons);
    return 0;
}
