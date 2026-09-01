/* 버블 정렬 — 이웃끼리 비교해서 큰 것을 뒤로 보낸다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-02-basic-sorting/04_bubble_sort/bubbleSort.out
 */
#include <stdio.h>

/* a를 오름차순으로 정렬한다.
 * comparisons에는 비교 횟수를, moves에는 원소를 옮겨 적은 횟수를 누적한다.
 * 교환 한 번은 temp를 거치므로 이동 3회로 센다. */
void bubbleSort(int a[], int n, int *comparisons, int *moves) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparisons)++;
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                *moves += 3;
            }
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
    bubbleSort(a, n, &comparisons, &moves);
    printArray("after ", a, n);
    printf("comparisons = %d, moves = %d\n", comparisons, moves);
    return 0;
}
