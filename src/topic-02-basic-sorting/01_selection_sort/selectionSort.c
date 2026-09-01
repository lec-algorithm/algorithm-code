/* 선택 정렬 — 남은 것 중 최솟값을 골라 앞으로 보낸다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-02-basic-sorting/01_selection_sort/selectionSort.out
 */
#include <stdio.h>

/* a를 오름차순으로 정렬한다.
 * comparisons에는 비교 횟수를, moves에는 원소를 옮겨 적은 횟수를 누적한다.
 * 교환 한 번은 temp를 거치므로 이동 3회로 센다. */
void selectionSort(int a[], int n, int *comparisons, int *moves) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (a[j] < a[min]) {
                min = j;
            }
        }
        if (min != i) {
            int temp = a[i];
            a[i] = a[min];
            a[min] = temp;
            *moves += 3;
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
    int a[] = {6, 8, 5, 9, 10, 1, 7, 2, 4, 3};
    int n = sizeof(a) / sizeof(a[0]);
    int comparisons = 0;
    int moves = 0;

    printArray("before", a, n);
    selectionSort(a, n, &comparisons, &moves);
    printArray("after ", a, n);
    printf("comparisons = %d, moves = %d\n", comparisons, moves);
    return 0;
}
