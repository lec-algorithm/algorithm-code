/* 최소 힙 — 우선순위 큐의 바탕이 되는 완전 이진 트리.
 *
 * 1부터 9가 든 힙에 10과 0을 넣고(sift up), 최솟값을 하나 꺼낸다(sift down).
 * 비교와 교환을 한 줄씩 찍는다. 끝으로 남은 것을 모두 꺼내 오름차순으로
 * 나오는지 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/04_min_heap/minHeap.out
 */
#include <stdio.h>

#define CAPACITY 16

typedef struct {
    int a[CAPACITY];
    int size;
} Heap;

static int verbose = 1; /* 0이면 비교·교환을 찍지 않는다(마지막 확인용) */

static void swap(Heap *h, int i, int j) {
    int t = h->a[i];
    h->a[i] = h->a[j];
    h->a[j] = t;
}

static void printHeap(const Heap *h) {
    printf("heap [");
    for (int i = 0; i < h->size; i++) {
        printf(i > 0 ? " %d" : "%d", h->a[i]);
    }
    printf("]\n");
}

/* i에 있는 값을 부모보다 작지 않을 때까지 올린다. */
void heapifyUp(Heap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->a[i] >= h->a[p]) {
            if (verbose) printf("  %d (index %d) vs parent %d (index %d): stay\n", h->a[i], i, h->a[p], p);
            return;
        }
        if (verbose) printf("  %d (index %d) vs parent %d (index %d): swap\n", h->a[i], i, h->a[p], p);
        swap(h, i, p);
        i = p;
    }
    if (verbose) printf("  reached the root\n");
}

int insert(Heap *h, int x) {
    if (h->size == CAPACITY) {
        return 0;
    }
    h->a[h->size] = x;
    h->size++;
    printf("insert %d at index %d\n", x, h->size - 1);
    heapifyUp(h, h->size - 1);
    return 1;
}

/* i에 있는 값을 두 자식보다 크지 않을 때까지 내린다. */
void heapifyDown(Heap *h, int i) {
    for (;;) {
        int m = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < h->size && h->a[l] < h->a[m]) {
            m = l;
        }
        if (r < h->size && h->a[r] < h->a[m]) {
            m = r;
        }
        if (verbose) {
            printf("  %d (index %d) vs children", h->a[i], i);
            if (l < h->size) {
                printf(" %d (index %d)", h->a[l], l);
            }
            if (r < h->size) {
                printf(", %d (index %d)", h->a[r], r);
            }
            if (l >= h->size) {
                printf(" none");
            }
            if (m == i) {
                printf(": stay\n");
            } else {
                printf(": swap with %d\n", h->a[m]);
            }
        }
        if (m == i) {
            return;
        }
        swap(h, i, m);
        i = m;
    }
}

int extractMin(Heap *h) {
    int min = h->a[0];
    h->a[0] = h->a[h->size - 1];
    h->size--;
    if (verbose) printf("extract min %d, move last %d to the root\n", min, h->a[0]);
    if (h->size > 0) {
        heapifyDown(h, 0);
    }
    return min;
}

int main(void) {
    Heap h = {{1, 2, 3, 4, 5, 6, 7, 8, 9}, 9};
    printHeap(&h);
    printf("\n");

    insert(&h, 10);
    printHeap(&h);
    printf("\n");

    insert(&h, 0);
    printHeap(&h);
    printf("\n");

    extractMin(&h);
    printHeap(&h);
    printf("\n");

    verbose = 0;
    printf("extract all:");
    while (h.size > 0) {
        printf(" %d", extractMin(&h));
    }
    printf("\n");
    return 0;
}
