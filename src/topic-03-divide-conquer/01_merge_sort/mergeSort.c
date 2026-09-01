/* 병합 정렬 — 반으로 나눠 각각 정렬하고, 정렬된 두 절반을 합친다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-03-divide-conquer/01_merge_sort/mergeSort.out
 */
#include <stdio.h>

/* A[lo..mid]와 A[mid+1..hi]가 각각 정렬된 상태에서 하나로 합친다.
 * comparisons에는 비교 횟수를, moves에는 원소를 옮겨 적은 횟수를
 * 누적한다. temp로 한 번, 다시 a로 한 번 가므로 원소마다 이동 2회다. */
static void merge(int a[], int lo, int mid, int hi, int temp[],
                  int *comparisons, int *moves) {
    int i = lo;
    int j = mid + 1;
    int k = lo;
    while (i <= mid && j <= hi) {
        (*comparisons)++;
        if (a[i] <= a[j]) {   /* <=라서 같은 값이면 왼쪽이 먼저: 안정 */
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
        (*moves)++;
    }
    while (i <= mid) {
        temp[k++] = a[i++];
        (*moves)++;
    }
    while (j <= hi) {
        temp[k++] = a[j++];
        (*moves)++;
    }
    for (k = lo; k <= hi; k++) {
        a[k] = temp[k];
        (*moves)++;
    }
}

/* a[lo..hi]를 오름차순으로 정렬한다. */
void mergeSort(int a[], int lo, int hi, int temp[],
               int *comparisons, int *moves) {
    if (lo >= hi) {
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergeSort(a, lo, mid, temp, comparisons, moves);
    mergeSort(a, mid + 1, hi, temp, comparisons, moves);
    merge(a, lo, mid, hi, temp, comparisons, moves);
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
    int temp[10];
    int n = sizeof(a) / sizeof(a[0]);
    int comparisons = 0;
    int moves = 0;

    printArray("before", a, n);
    mergeSort(a, 0, n - 1, temp, &comparisons, &moves);
    printArray("after ", a, n);
    printf("comparisons = %d, moves = %d\n", comparisons, moves);
    return 0;
}
