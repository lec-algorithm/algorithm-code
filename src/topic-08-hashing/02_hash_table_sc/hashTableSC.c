/* 개별 체이닝 해시 테이블 — 배열의 칸마다 연결 리스트를 둔다.
 *
 * 해시로 칸을 찾고(1단계), 그 칸의 리스트를 순차 탐색한다(2단계).
 * 새 키는 리스트 맨 앞에 붙이고, 이미 있는 키는 값만 바꾼다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-08-hashing/02_hash_table_sc/hashTableSC.out
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    int m;        /* 배열 크기 */
    int n;        /* 저장된 키 수 */
    Node **heads; /* heads[i]: i번 칸 리스트의 맨 앞 노드 */
} ChainingTable;

/* 이 예제의 키는 대문자 한 글자다. 글자마다 정수 해시 코드를 정해 두었다.
 * 원본 강의 그림의 해시값(M = 5)이 이 코드를 M으로 나눈 나머지와 같다.
 * 표에 없는 글자는 ASCII 코드를 그대로 쓴다. */
static const char LETTERS[] = "ACEHLMPRSXY";
static const int CODES[] = {20, 69, 10, 4, 38, 49, 78, 14, 22, 47, 52};

int hashCode(char key) {
    const char *p = strchr(LETTERS, key);
    return p != NULL ? CODES[p - LETTERS] : (unsigned char)key;
}

int hashOf(char key, int m) {
    return hashCode(key) % m;
}

ChainingTable *createTable(int m) {
    ChainingTable *t = malloc(sizeof(ChainingTable));
    t->m = m;
    t->n = 0;
    t->heads = calloc(m, sizeof(Node *)); /* 모든 칸을 빈 리스트(NULL)로 */
    return t;
}

/* key에 value를 넣는다. 새 키면 1, 값만 바꿨으면 0을 돌려준다.
 * 키를 몇 번 비교했는지 *compares에 담는다. */
int put(ChainingTable *t, char key, int value, int *compares) {
    int i = hashOf(key, t->m);
    *compares = 0;
    for (Node *x = t->heads[i]; x != NULL; x = x->next) {
        (*compares)++;
        if (x->key == key) {
            x->value = value;
            return 0;
        }
    }
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->next = t->heads[i]; /* 맨 앞에 붙인다 */
    t->heads[i] = node;
    t->n++;
    return 1;
}

/* key를 찾으면 1을 돌려주고 값을 *value에 담는다. 없으면 0 */
int get(const ChainingTable *t, char key, int *value, int *compares) {
    int i = hashOf(key, t->m);
    *compares = 0;
    for (Node *x = t->heads[i]; x != NULL; x = x->next) {
        (*compares)++;
        if (x->key == key) {
            *value = x->value;
            return 1;
        }
    }
    return 0;
}

/* key를 지우면 1, 없으면 0 */
int delete(ChainingTable *t, char key, int *compares) {
    int i = hashOf(key, t->m);
    *compares = 0;
    Node *prev = NULL;
    for (Node *x = t->heads[i]; x != NULL; prev = x, x = x->next) {
        (*compares)++;
        if (x->key == key) {
            if (prev == NULL) {
                t->heads[i] = x->next; /* 맨 앞 노드 */
            } else {
                prev->next = x->next; /* 중간이나 끝 노드 */
            }
            free(x);
            t->n--;
            return 1;
        }
    }
    return 0;
}

void freeTable(ChainingTable *t) {
    for (int i = 0; i < t->m; i++) {
        Node *x = t->heads[i];
        while (x != NULL) {
            Node *next = x->next;
            free(x);
            x = next;
        }
    }
    free(t->heads);
    free(t);
}

static void printChain(const ChainingTable *t, int i) {
    printf("[%d]", i);
    if (t->heads[i] == NULL) {
        printf(" -");
    }
    for (Node *x = t->heads[i]; x != NULL; x = x->next) {
        printf(" %c%d%s", x->key, x->value, x->next != NULL ? " ->" : "");
    }
    printf("\n");
}

static void printTable(const ChainingTable *t) {
    for (int i = 0; i < t->m; i++) {
        printChain(t, i);
    }
}

int main(void) {
    const char *keys = "SEARCHEXAMPLE";
    ChainingTable *t = createTable(5);
    int compares, value;

    printf("M = %d, keys", t->m);
    for (int i = 0; keys[i] != '\0'; i++) {
        printf(" %c", keys[i]);
    }
    printf("\n\n");

    for (int i = 0; keys[i] != '\0'; i++) { /* 값은 등장 순서 0, 1, 2, ... */
        int isNew = put(t, keys[i], i, &compares);
        printf("put %c=%-2d  hash %d  compares %d  %-6s  ", keys[i], i,
               hashOf(keys[i], t->m), compares, isNew ? "new" : "update");
        printChain(t, hashOf(keys[i], t->m));
    }

    printf("\nN = %d, M = %d, N/M = %.1f\n", t->n, t->m, (double)t->n / t->m);
    printTable(t);

    const char queries[] = {'E', 'Y'};
    printf("\n");
    for (int q = 0; q < 2; q++) {
        char key = queries[q];
        int found = get(t, key, &value, &compares);
        printf("get %c: hash %d, compares %d -> ", key, hashOf(key, t->m), compares);
        if (found) {
            printf("%d\n", value);
        } else {
            printf("not found\n");
        }
    }

    int deleted = delete(t, 'E', &compares);
    printf("\ndelete E: hash %d, compares %d -> %s\n", hashOf('E', t->m), compares,
           deleted ? "deleted" : "not found");
    printf("N = %d\n", t->n);
    printTable(t);

    freeTable(t);
    return 0;
}
