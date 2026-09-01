/* 셸 정렬 — 멀리 떨어진 원소끼리 먼저 삽입 정렬해 두고, 간격을 줄여 간다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-02-basic-sorting/03_shell_sort/shellSort.out
 */
#include <stdio.h>

/* a를 오름차순으로 정렬한다. gap을 절반씩 줄인다 (10개면 5, 2, 1).
 * comparisons에는 비교 횟수를, moves에는 원소를 옮겨 적은 횟수를 누적한다. */
void shellSort(int a[], int n, int *comparisons, int *moves) {
    for (int gap = n / 2; gap >= 1; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int key = a[i];
            (*moves)++;
            int j = i - gap;
            while (j >= 0) {
                (*comparisons)++;
                if (a[j] <= key) {
                    break;
                }
                a[j + gap] = a[j];
                (*moves)++;
                j -= gap;
            }
            a[j + gap] = key;
            (*moves)++;
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
    shellSort(a, n, &comparisons, &moves);
    printArray("after ", a, n);
    printf("comparisons = %d, moves = %d\n", comparisons, moves);
    return 0;
}
