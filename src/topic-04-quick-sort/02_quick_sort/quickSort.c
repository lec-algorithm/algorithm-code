/* 퀵 정렬 — 피벗을 제자리에 놓고, 양쪽을 각각 정렬한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-04-quick-sort/02_quick_sort/quickSort.out
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

/* A[lo]를 피벗으로 삼아 작은 것은 왼쪽, 나머지는 오른쪽으로 보내고
 * 피벗을 그 경계에 놓는다. 피벗의 최종 위치를 돌려준다. */
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

/* a[lo..hi]를 오름차순으로 정렬한다. */
void quickSort(int a[], int lo, int hi, int *comparisons, int *moves) {
    if (lo >= hi) {
        return;
    }
    int p = partition(a, lo, hi, comparisons, moves);
    quickSort(a, lo, p - 1, comparisons, moves);
    quickSort(a, p + 1, hi, comparisons, moves);
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
    int comparisons = 0;
    int moves = 0;

    printArray("before", a, n);
    quickSort(a, 0, n - 1, &comparisons, &moves);
    printArray("after ", a, n);
    printf("comparisons = %d, moves = %d\n", comparisons, moves);
    return 0;
}
