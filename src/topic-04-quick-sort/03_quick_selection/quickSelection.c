/* 퀵 선택 — 다 정렬하지 않고 k번째로 작은 값을 찾는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-04-quick-sort/03_quick_selection/quickSelection.out
 */
#include <stdio.h>

static void swap(int a[], int i, int j, int *moves) {
    if (i != j) {
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
        *moves += 3;
    }
}

/* 퀵 정렬과 같은 파티션이다. 피벗의 최종 위치를 돌려준다. */
static int partition(int a[], int lo, int hi, int *comparisons, int *moves) {
    int pivot = a[lo];
    int i = lo;
    for (int j = lo + 1; j <= hi; j++) {
        (*comparisons)++;
        if (a[j] < pivot) {
            i++;
            swap(a, i, j, moves);
        }
    }
    swap(a, lo, i, moves);
    return i;
}

/* k번째로 작은 값을 돌려준다 (k는 1부터). 한쪽만 파고든다. */
int quickSelection(int a[], int n, int k, int *comparisons, int *moves) {
    int lo = 0;
    int hi = n - 1;
    while (1) {
        int p = partition(a, lo, hi, comparisons, moves);
        if (p == k - 1) {
            return a[p];
        }
        if (p > k - 1) {
            hi = p - 1;
        } else {
            lo = p + 1;
        }
    }
}

static void printArray(const char *label, const int a[], int n) {
    printf("%s:", label);
    for (int i = 0; i < n; i++) {
        printf(" %d", a[i]);
    }
    printf("\n");
}

int main(void) {
    int a[] = {2, 8, 5, 9, 1, 10, 7, 6, 4, 3};
    int n = sizeof(a) / sizeof(a[0]);
    int k = 5;
    int comparisons = 0;
    int moves = 0;

    printArray("before", a, n);
    int value = quickSelection(a, n, k, &comparisons, &moves);
    printArray("after ", a, n);
    printf("k = %d -> value = %d\n", k, value);
    printf("comparisons = %d, moves = %d\n", comparisons, moves);
    return 0;
}
