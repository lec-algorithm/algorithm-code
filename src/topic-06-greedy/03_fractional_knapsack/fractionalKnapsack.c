/* 분할 배낭 — 용량이 정해진 배낭에 가치를 최대로 담는다.
 *
 * 물건을 쪼갤 수 있다. 단위 무게당 가치가 높은 물건부터 담는다 (그리디).
 * 쪼갤 수 없게 바꾸면 같은 순서가 최적을 놓치는 것도 함께 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-06-greedy/03_fractional_knapsack/fractionalKnapsack.out
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 8

typedef struct {
    int weight;
    int value;
} Item;

/* 단위 무게당 가치 내림차순. 나눗셈 없이 교차곱으로 비교한다. */
static int compareByRatio(const void *a, const void *b) {
    const Item *x = (const Item *)a;
    const Item *y = (const Item *)b;
    long lhs = (long)y->value * x->weight;
    long rhs = (long)x->value * y->weight;
    return (lhs > rhs) - (lhs < rhs);
}

/* items를 단위 무게당 가치로 정렬하고 담은 가치의 합을 돌려준다.
 * taken[i]에 items[i]를 얼마만큼(0~1) 담았는지 적는다. */
double fractionalKnapsack(Item items[], int n, int capacity, double taken[]) {
    qsort(items, n, sizeof(Item), compareByRatio);

    double total = 0.0;
    for (int i = 0; i < n; i++) {
        taken[i] = 0.0;
    }
    for (int i = 0; i < n; i++) {
        if (capacity == 0) {
            break;
        }
        if (items[i].weight <= capacity) {
            taken[i] = 1.0;
            total += items[i].value;
            capacity -= items[i].weight;
        } else {
            taken[i] = (double)capacity / items[i].weight;
            total += items[i].value * taken[i];
            capacity = 0;
        }
    }
    return total;
}

/* 같은 순서로 담되 쪼개지 않는다 (0/1 배낭에 그리디를 쓴 경우). */
int zeroOneGreedy(Item items[], int n, int capacity) {
    qsort(items, n, sizeof(Item), compareByRatio);

    int total = 0;
    for (int i = 0; i < n; i++) {
        if (items[i].weight <= capacity) {
            total += items[i].value;
            capacity -= items[i].weight;
        }
    }
    return total;
}

static void run(Item items[], int n, int capacity) {
    double taken[MAX_ITEMS];
    double total = fractionalKnapsack(items, n, capacity, taken);

    printf("capacity = %d\n", capacity);
    for (int i = 0; i < n; i++) {
        if (taken[i] > 0.0) {
            printf("  take (w=%d, v=%d, v/w=%.1f) x %.2f\n",
                   items[i].weight, items[i].value,
                   (double)items[i].value / items[i].weight, taken[i]);
        }
    }
    printf("  total value = %.2f\n", total);
}

int main(void) {
    Item items[] = {{20, 100}, {30, 120}, {10, 60}};
    int n = sizeof(items) / sizeof(items[0]);

    run(items, n, 15);
    printf("\n");
    run(items, n, 50);
    printf("\n0/1 greedy, capacity = 50\n");
    printf("  total value = %d\n", zeroOneGreedy(items, n, 50));
    return 0;
}
