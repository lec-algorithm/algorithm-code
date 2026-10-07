/* 활동 선택 — 회의실 하나에 겹치지 않는 회의를 가장 많이 넣는다.
 *
 * 언제나 가장 일찍 끝나는 회의부터 고른다 (그리디).
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-06-greedy/02_activity_selection/activitySelection.out
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} Activity;

/* 끝 시각 오름차순. 끝이 같으면 시작 시각 오름차순. */
static int compareByEnd(const void *a, const void *b) {
    const Activity *x = (const Activity *)a;
    const Activity *y = (const Activity *)b;
    if (x->end != y->end) {
        return x->end - y->end;
    }
    return x->start - y->start;
}

/* activities를 끝 시각으로 정렬하고, 고른 활동을 selected에 담는다.
 * 고른 개수를 돌려준다. */
int activitySelection(Activity activities[], int n, Activity selected[]) {
    qsort(activities, n, sizeof(Activity), compareByEnd);

    int count = 0;
    selected[count++] = activities[0];
    int lastEnd = activities[0].end;
    for (int i = 1; i < n; i++) {
        if (activities[i].start >= lastEnd) {
            selected[count++] = activities[i];
            lastEnd = activities[i].end;
        }
    }
    return count;
}

static void printActivities(const char *label, const Activity a[], int n) {
    printf("%s:", label);
    for (int i = 0; i < n; i++) {
        printf(" (%d,%d)", a[i].start, a[i].end);
    }
    printf("\n");
}

int main(void) {
    Activity activities[] = {{5, 8}, {1, 3}, {8, 9}, {2, 5}, {6, 7}, {4, 6}};
    int n = sizeof(activities) / sizeof(activities[0]);
    Activity selected[sizeof(activities) / sizeof(activities[0])];

    printActivities("input   ", activities, n);
    int count = activitySelection(activities, n, selected);
    printActivities("by end  ", activities, n);
    printActivities("selected", selected, count);
    printf("count = %d\n", count);
    return 0;
}
