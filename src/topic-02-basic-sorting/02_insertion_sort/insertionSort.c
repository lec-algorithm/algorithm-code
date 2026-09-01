/* 삽입 정렬 — 이미 정렬된 앞부분에 새 원소를 끼워 넣는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-02-basic-sorting/02_insertion_sort/insertionSort.out
 */
#include <stdio.h>

/* a를 오름차순으로 정렬한다.
 * comparisons에는 비교 횟수를, moves에는 원소를 옮겨 적은 횟수를 누적한다.
 * key를 꺼낼 때 1회, 한 칸 밀 때마다 1회, 끼워 넣을 때 1회로 센다. */
void insertionSort(int a[], int n, int *comparisons, int *moves) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        (*moves)++;
        int j = i - 1;
        while (j >= 0) {
            (*comparisons)++;
            if (a[j] <= key) {
                break;
            }
            a[j + 1] = a[j];
            (*moves)++;
            j--;
        }
        a[j + 1] = key;
        (*moves)++;
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
    insertionSort(a, n, &comparisons, &moves);
    printArray("after ", a, n);
    printf("comparisons = %d, moves = %d\n", comparisons, moves);
    return 0;
}
