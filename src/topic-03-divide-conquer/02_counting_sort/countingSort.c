/* 카운팅 정렬 — 값을 비교하지 않고, 값마다 몇 개 있는지 세서 자리를 계산한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-03-divide-conquer/02_counting_sort/countingSort.out
 */
#include <stdio.h>

#define MAX_KEY 5  /* 값의 범위: 0 이상 MAX_KEY 이하 */

/* a를 정렬한 결과를 b에 쓴다. moves에는 b에 원소를 적은 횟수를 누적한다.
 * 원소끼리 비교하는 일이 없으므로 비교 횟수는 항상 0이다. */
void countingSort(const int a[], int b[], int n, int count[], int *moves) {
    for (int v = 0; v <= MAX_KEY; v++) {
        count[v] = 0;
    }
    for (int i = 0; i < n; i++) {         /* 1) 세기 */
        count[a[i]]++;
    }
    for (int v = 1; v <= MAX_KEY; v++) {  /* 2) 누적 */
        count[v] += count[v - 1];
    }
    for (int i = n - 1; i >= 0; i--) {    /* 3) 뒤에서부터 배치: 안정 */
        count[a[i]]--;
        b[count[a[i]]] = a[i];
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
    int a[] = {1, 0, 1, 5, 4, 3, 1, 4, 5, 2, 1};
    int n = sizeof(a) / sizeof(a[0]);
    int b[11];
    int count[MAX_KEY + 1];
    int moves = 0;

    printArray("before", a, n);
    countingSort(a, b, n, count, &moves);
    printArray("after ", b, n);
    printf("comparisons = 0, moves = %d\n", moves);
    return 0;
}
