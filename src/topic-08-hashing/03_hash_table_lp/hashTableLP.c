/* 선형 탐지 해시 테이블 — 충돌하면 배열의 다음 칸으로 간다.
 *
 * 리스트 없이 배열 하나에 키를 직접 넣는다(개방형 주소 지정). 해시가 가리킨
 * 칸이 차 있으면 빈칸이 나올 때까지 오른쪽으로 한 칸씩 간다. 지울 때는 같은
 * 무리에서 뒤따르던 키들을 꺼냈다가 다시 넣는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-08-hashing/03_hash_table_lp/hashTableLP.out
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY '\0'

typedef struct {
    int m;       /* 배열 크기 */
    int n;       /* 저장된 키 수 */
    char *keys;  /* keys[i] == EMPTY이면 빈칸 */
    int *values;
} ProbingTable;

/* 02 예제와 같은 해시 코드. 원본 강의 그림의 해시값(M = 16)이 이 코드를
 * M으로 나눈 나머지와 같다. 표에 없는 글자는 ASCII 코드를 그대로 쓴다. */
static const char LETTERS[] = "ACEHLMPRSXY";
static const int CODES[] = {20, 69, 10, 4, 38, 49, 78, 14, 22, 47, 52};

int hashCode(char key) {
    const char *p = strchr(LETTERS, key);
    return p != NULL ? CODES[p - LETTERS] : (unsigned char)key;
}

int hashOf(char key, int m) {
    return hashCode(key) % m;
}

ProbingTable *createTable(int m) {
    ProbingTable *t = malloc(sizeof(ProbingTable));
    t->m = m;
    t->n = 0;
    t->keys = calloc(m, sizeof(char)); /* 모든 칸을 빈칸으로 */
    t->values = calloc(m, sizeof(int));
    return t;
}

void freeTable(ProbingTable *t) {
    free(t->keys);
    free(t->values);
    free(t);
}

ProbingTable *copyTable(const ProbingTable *t) {
    ProbingTable *c = createTable(t->m);
    c->n = t->n;
    memcpy(c->keys, t->keys, t->m * sizeof(char));
    memcpy(c->values, t->values, t->m * sizeof(int));
    return c;
}

void resize(ProbingTable *t, int newM);

/* key에 value를 넣는다. 새 키면 1, 값만 바꿨으면 0을 돌려준다.
 * 들여다본 칸 수를 *probes에, 키가 놓인 칸을 *slot에 담는다. */
int put(ProbingTable *t, char key, int value, int *probes, int *slot) {
    if (t->n * 10 > t->m * 7) { /* 적재율 N/M이 0.7을 넘으면 두 배로 */
        resize(t, 2 * t->m);
    }
    int i = hashOf(key, t->m);
    *probes = 1;
    while (t->keys[i] != EMPTY) {
        if (t->keys[i] == key) {
            t->values[i] = value;
            *slot = i;
            return 0;
        }
        i = (i + 1) % t->m; /* 끝에 닿으면 0번 칸으로 */
        (*probes)++;
    }
    t->keys[i] = key;
    t->values[i] = value;
    t->n++;
    *slot = i;
    return 1;
}

/* key를 찾으면 1을 돌려주고 값을 *value에 담는다. 빈칸을 만나면 0 */
int get(const ProbingTable *t, char key, int *value, int *probes) {
    int i = hashOf(key, t->m);
    *probes = 1;
    while (t->keys[i] != EMPTY) {
        if (t->keys[i] == key) {
            *value = t->values[i];
            return 1;
        }
        i = (i + 1) % t->m;
        (*probes)++;
    }
    return 0;
}

/* t의 키를 0번 칸부터 차례로 크기 newM인 새 배열에 다시 넣는다 */
void resize(ProbingTable *t, int newM) {
    ProbingTable *bigger = createTable(newM);
    int probes, slot;
    for (int i = 0; i < t->m; i++) {
        if (t->keys[i] != EMPTY) {
            put(bigger, t->keys[i], t->values[i], &probes, &slot);
        }
    }
    free(t->keys);
    free(t->values);
    *t = *bigger; /* 새 배열을 넘겨받고 껍데기만 버린다 */
    free(bigger);
}

/* key를 지우면 1, 없으면 0. 뒤따르던 키를 다시 넣을 때마다 한 줄씩 찍는다. */
int delete(ProbingTable *t, char key, int *probes) {
    int i = hashOf(key, t->m);
    *probes = 1;
    while (t->keys[i] != key) {
        if (t->keys[i] == EMPTY) {
            return 0;
        }
        i = (i + 1) % t->m;
        (*probes)++;
    }
    t->keys[i] = EMPTY;
    t->n--;

    i = (i + 1) % t->m;
    while (t->keys[i] != EMPTY) { /* 같은 무리에서 뒤따르던 키들 */
        char k = t->keys[i];
        int v = t->values[i];
        int p, slot;
        t->keys[i] = EMPTY;
        t->n--;
        put(t, k, v, &p, &slot);
        printf("  reinsert %c: slot %d -> %d\n", k, i, slot);
        i = (i + 1) % t->m;
    }
    if (t->n > 0 && t->n <= t->m / 8) { /* 8분의 1 이하로 비면 절반으로 */
        resize(t, t->m / 2);
    }
    return 1;
}

/* 저장된 키를 모두 한 번씩 찾을 때 들여다보는 칸 수의 합 */
int totalHitProbes(const ProbingTable *t) {
    int total = 0, value, probes;
    for (int i = 0; i < t->m; i++) {
        if (t->keys[i] != EMPTY) {
            get(t, t->keys[i], &value, &probes);
            total += probes;
        }
    }
    return total;
}

static void printTable(const ProbingTable *t) {
    for (int start = 0; start < t->m; start += 16) {
        int end = start + 16 < t->m ? start + 16 : t->m;
        printf("slot ");
        for (int i = start; i < end; i++) printf("%3d", i);
        printf("\nkey  ");
        for (int i = start; i < end; i++) printf("%3c", t->keys[i] != EMPTY ? t->keys[i] : '-');
        printf("\nvalue");
        for (int i = start; i < end; i++) {
            if (t->keys[i] != EMPTY) {
                printf("%3d", t->values[i]);
            } else {
                printf("%3c", '-');
            }
        }
        printf("\n");
    }
}

static void printGet(const ProbingTable *t, char key) {
    int value, probes;
    int found = get(t, key, &value, &probes);
    printf("get %c: hash %d, probes %d -> ", key, hashOf(key, t->m), probes);
    if (found) {
        printf("%d\n", value);
    } else {
        printf("not found\n");
    }
}

static void printHits(const ProbingTable *t) {
    int total = totalHitProbes(t);
    printf("hits: %d probes for %d keys, %.2f per key\n", total, t->n, (double)total / t->n);
}

int main(void) {
    const char *keys = "SEARCHEXAMPLE";
    ProbingTable *t = createTable(16);
    int probes, slot;

    printf("M = %d, keys", t->m);
    for (int i = 0; keys[i] != '\0'; i++) {
        printf(" %c", keys[i]);
    }
    printf("\n\n");

    for (int i = 0; keys[i] != '\0'; i++) { /* 값은 등장 순서 0, 1, 2, ... */
        int isNew = put(t, keys[i], i, &probes, &slot);
        printf("put %c=%-2d  hash %2d  probes %d  slot %2d  %s\n", keys[i], i,
               hashOf(keys[i], t->m), probes, slot, isNew ? "new" : "update");
    }

    printf("\nN = %d, M = %d, N/M = %.4f\n", t->n, t->m, (double)t->n / t->m);
    printTable(t);

    printf("\n");
    printGet(t, 'H');
    printGet(t, 'Y');
    printHits(t);

    /* 적재율을 낮추면: 같은 키를 크기 32인 배열에 다시 넣는다 */
    ProbingTable *wide = copyTable(t);
    resize(wide, 32);
    printf("\nresize to M = %d, N/M = %.4f\n", wide->m, (double)wide->n / wide->m);
    printTable(wide);
    printHits(wide);
    freeTable(wide);

    /* 잘못된 삭제: C의 칸만 비운다 */
    ProbingTable *naive = copyTable(t);
    naive->keys[5] = EMPTY;
    naive->n--;
    printf("\nnaive delete C: just empty slot 5\n");
    printGet(naive, 'H');
    freeTable(naive);

    printf("\ndelete C: hash %d\n", hashOf('C', t->m));
    delete(t, 'C', &probes);
    printGet(t, 'H');
    printf("N = %d, M = %d\n", t->n, t->m);
    printTable(t);

    freeTable(t);
    return 0;
}
