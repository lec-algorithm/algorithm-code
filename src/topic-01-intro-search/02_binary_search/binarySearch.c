/* 이진 검색 — 정렬된 배열에서 범위를 절반씩 줄여 간다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-01-intro-search/02_binary_search/binarySearch.out
 */
#include <stdio.h>

/* key를 찾으면 그 인덱스를, 없으면 -1을 돌려준다.
 * comparisons에는 비교한 횟수를 누적한다.
 * a는 반드시 오름차순으로 정렬되어 있어야 한다. */
int binarySearch(const int a[], int n, int key, int *comparisons) {
    int lo = 0;
    int hi = n - 1;

    while (lo <= hi) {
        /* (lo + hi) / 2로 쓰면 두 값이 클 때 int 범위를 넘을 수 있다. */
        int mid = lo + (hi - lo) / 2;
        (*comparisons)++;

        if (a[mid] == key) {
            return mid;
        } else if (a[mid] < key) {
            lo = mid + 1; /* 오른쪽 절반만 남긴다 */
        } else {
            hi = mid - 1; /* 왼쪽 절반만 남긴다 */
        }
    }
    return -1;
}

int main(void) {
    int a[] = {6, 13, 14, 25, 33, 43, 51, 53, 64, 72, 84, 93, 95, 96, 101};
    int n = (int)(sizeof(a) / sizeof(a[0]));
    int key = 51;
    int comparisons = 0;

    int index = binarySearch(a, n, key, &comparisons);

    printf("n = %d, key = %d\n", n, key);
    printf("index = %d, comparisons = %d\n", index, comparisons);
    return 0;
}
