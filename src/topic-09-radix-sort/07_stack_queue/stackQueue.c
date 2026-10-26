/* 스택과 큐 — LIFO와 FIFO. 그리고 LSD 기수 정렬의 버킷으로 썼을 때의 차이.
 *
 * 버킷을 큐로 쓰면 같은 숫자끼리 들어온 순서가 지켜져(안정) 정렬이 되고,
 * 스택으로 쓰면 순서가 뒤집혀(불안정) 정렬이 깨진다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-09-radix-sort/07_stack_queue/stackQueue.out
 */
#include <stdio.h>

#define CAPACITY 16
#define RADIX 10
#define WIDTH 3

typedef struct {
    int items[CAPACITY];
    int top;              /* 다음에 넣을 자리 = 들어 있는 개수 */
} Stack;

typedef struct {
    int items[CAPACITY];
    int head;             /* 다음에 꺼낼 자리 */
    int tail;             /* 다음에 넣을 자리 */
} Queue;

void stackInit(Stack *s) { s->top = 0; }
void push(Stack *s, int x) { s->items[s->top++] = x; }
int pop(Stack *s) { return s->items[--s->top]; }
int stackEmpty(const Stack *s) { return s->top == 0; }

void queueInit(Queue *q) { q->head = q->tail = 0; }
void enqueue(Queue *q, int x) { q->items[q->tail++] = x; }
int dequeue(Queue *q) { return q->items[q->head++]; }
int queueEmpty(const Queue *q) { return q->head == q->tail; }

static void printNumbers(const char *label, const int a[], int n) {
    printf("%s:", label);
    for (int i = 0; i < n; i++) {
        printf(" %0*d", WIDTH, a[i]);
    }
    printf("\n");
}

/* useQueue가 1이면 큐 버킷, 0이면 스택 버킷으로 LSD 기수 정렬을 한다. 패스마다 한 줄 찍는다. */
void bucketRadixSort(int a[], int n, int useQueue) {
    const char *kind = useQueue ? "queue" : "stack";
    int max = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    for (int exp = 1; max / exp > 0; exp *= RADIX) {
        Queue queues[RADIX];
        Stack stacks[RADIX];
        for (int d = 0; d < RADIX; d++) {
            queueInit(&queues[d]);
            stackInit(&stacks[d]);
        }
        for (int i = 0; i < n; i++) {                 /* 흩기 */
            int d = (a[i] / exp) % RADIX;
            if (useQueue) {
                enqueue(&queues[d], a[i]);
            } else {
                push(&stacks[d], a[i]);
            }
        }
        int k = 0;
        for (int d = 0; d < RADIX; d++) {             /* 모으기 */
            if (useQueue) {
                while (!queueEmpty(&queues[d])) {
                    a[k++] = dequeue(&queues[d]);
                }
            } else {
                while (!stackEmpty(&stacks[d])) {
                    a[k++] = pop(&stacks[d]);
                }
            }
        }
        char label[32];
        snprintf(label, sizeof(label), "%s exp %-3d", kind, exp);
        printNumbers(label, a, n);
    }
}

static int isSorted(const int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    const char letters[] = {'A', 'B', 'C', 'D'};
    Stack s;
    Queue q;
    stackInit(&s);
    queueInit(&q);

    printf("push   :");
    for (int i = 0; i < 4; i++) {
        push(&s, letters[i]);
        printf(" %c", letters[i]);
    }
    printf("\npop    :");
    while (!stackEmpty(&s)) {
        printf(" %c", pop(&s));
    }
    printf("\nenqueue:");
    for (int i = 0; i < 4; i++) {
        enqueue(&q, letters[i]);
        printf(" %c", letters[i]);
    }
    printf("\ndequeue:");
    while (!queueEmpty(&q)) {
        printf(" %c", dequeue(&q));
    }
    printf("\n\n");

    int input[] = {437, 42, 531, 8, 74};
    int n = sizeof(input) / sizeof(input[0]);
    int a[CAPACITY], b[CAPACITY];
    for (int i = 0; i < n; i++) {
        a[i] = input[i];
        b[i] = input[i];
    }

    printNumbers("input        ", input, n);
    bucketRadixSort(a, n, 1);
    bucketRadixSort(b, n, 0);
    printf("queue sorted = %s, stack sorted = %s\n",
           isSorted(a, n) ? "yes" : "no", isSorted(b, n) ? "yes" : "no");
    return 0;
}
