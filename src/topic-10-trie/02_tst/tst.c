/* TST(ternary search tree) — 노드마다 링크 3개로 줄인 트라이.
 *
 * 노드는 문자 하나를 갖고, 그보다 작은 문자는 왼쪽, 큰 문자는 오른쪽, 같으면
 * 가운데로 내려가 다음 문자를 본다. 삽입(한 단어는 한 걸음씩), 검색, 접두사로
 * 모두 찾기를 돌리고, 같은 키를 담는 데 링크가 몇 개 드는지 센다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-10-trie/02_tst/tst.out
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 64
#define MAX_RESULTS 32

typedef struct TSTNode {
    char data;               /* 이 노드가 나타내는 문자 */
    bool hasValue;           /* 이 노드에서 끝나는 키가 있는가 */
    int value;               /* 그 키의 값 */
    struct TSTNode *left;    /* data보다 작은 문자 (같은 자리) */
    struct TSTNode *middle;  /* 다음 자리의 문자 */
    struct TSTNode *right;   /* data보다 큰 문자 (같은 자리) */
} TSTNode;

typedef struct {
    char word[MAX_WORD];
    int value;
} WordValuePair;

TSTNode *createTSTNode(char data) {
    TSTNode *node = malloc(sizeof(TSTNode));
    if (node == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    node->data = data;
    node->hasValue = false;
    node->value = 0;
    node->left = NULL;
    node->middle = NULL;
    node->right = NULL;
    return node;
}

/* 갱신된 서브트리의 루트를 돌려준다. trace면 비교 한 번마다 한 줄 찍는다. */
TSTNode *insert(TSTNode *root, const char *word, int value, int index, bool trace) {
    int last = (int)strlen(word) - 1;
    char c = word[index];
    if (root == NULL) {
        root = createTSTNode(c); /* 아무것도 없으면 새 노드 */
        if (trace) {
            printf("  %c       new node", c);
            if (index == last) {
                printf(" = %d", value);
            }
            printf("\n");
        }
        if (index < last) {
            root->middle = insert(root->middle, word, value, index + 1, trace);
        } else {
            root->hasValue = true;
            root->value = value;
        }
        return root;
    }
    if (c < root->data) { /* 작으면 왼쪽, 같은 자리 */
        if (trace) {
            printf("  %c < %c   left\n", c, root->data);
        }
        root->left = insert(root->left, word, value, index, trace);
    } else if (c > root->data) { /* 크면 오른쪽, 같은 자리 */
        if (trace) {
            printf("  %c > %c   right\n", c, root->data);
        }
        root->right = insert(root->right, word, value, index, trace);
    } else if (index < last) { /* 같으면 가운데, 다음 자리 */
        if (trace) {
            printf("  %c = %c   middle\n", c, root->data);
        }
        root->middle = insert(root->middle, word, value, index + 1, trace);
    } else { /* 키의 마지막 문자: 값을 둔다 */
        if (trace) {
            printf("  %c = %c   value = %d\n", c, root->data, value);
        }
        root->hasValue = true;
        root->value = value;
    }
    return root;
}

/* word의 마지막 문자에 해당하는 노드, 없으면 NULL. visited에 거친 노드 수를 담는다. */
TSTNode *getNode(TSTNode *root, const char *word, int *visited) {
    int last = (int)strlen(word) - 1;
    TSTNode *node = root;
    TSTNode *found = NULL;
    int index = 0;
    int count = 0;
    while (node != NULL) {
        count++;
        char c = word[index];
        if (c < node->data) {
            node = node->left;
        } else if (c > node->data) {
            node = node->right;
        } else if (index == last) {
            found = node;
            break;
        } else {
            node = node->middle;
            index++;
        }
    }
    if (visited != NULL) {
        *visited = count;
    }
    return found;
}

/* word의 값, 없으면 -1 */
int search(TSTNode *root, const char *word, int *visited) {
    TSTNode *node = getNode(root, word, visited);
    if (node != NULL && node->hasValue) {
        return node->value;
    }
    return -1;
}

/* 왼쪽, 자기, 가운데, 오른쪽 순서로 돌면 사전 순이 된다. */
void getAllWordsHelper(TSTNode *node, char *prefix, int prefixLen,
                       WordValuePair results[], int *count) {
    if (node == NULL) {
        return;
    }
    getAllWordsHelper(node->left, prefix, prefixLen, results, count);
    prefix[prefixLen] = node->data;
    prefix[prefixLen + 1] = '\0';
    if (node->hasValue && *count < MAX_RESULTS) {
        strcpy(results[*count].word, prefix);
        results[*count].value = node->value;
        (*count)++;
    }
    getAllWordsHelper(node->middle, prefix, prefixLen + 1, results, count);
    prefix[prefixLen] = '\0'; /* 되돌린다 */
    getAllWordsHelper(node->right, prefix, prefixLen, results, count);
}

int getWordsWithPrefix(TSTNode *root, const char *prefix, WordValuePair results[]) {
    int count = 0;
    TSTNode *prefixNode = getNode(root, prefix, NULL); /* 1. 접두사의 마지막 문자 노드 */
    if (prefixNode == NULL) {
        return 0;
    }
    if (prefixNode->hasValue) { /* 접두사 자체가 키인 경우 */
        strcpy(results[count].word, prefix);
        results[count].value = prefixNode->value;
        count++;
    }
    char buffer[MAX_WORD];
    strcpy(buffer, prefix);
    getAllWordsHelper(prefixNode->middle, buffer, (int)strlen(prefix), results,
                      &count); /* 2. 가운데 아래만 */
    return count;
}

int countNodes(const TSTNode *node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->middle) + countNodes(node->right);
}

/* 루트에서 가장 먼 노드까지 거치는 노드 수 */
int height(const TSTNode *node) {
    if (node == NULL) {
        return 0;
    }
    int h = height(node->left);
    int m = height(node->middle);
    int r = height(node->right);
    if (m > h) {
        h = m;
    }
    if (r > h) {
        h = r;
    }
    return 1 + h;
}

void freeTST(TSTNode *node) {
    if (node == NULL) {
        return;
    }
    freeTST(node->left);
    freeTST(node->middle);
    freeTST(node->right);
    free(node);
}

/* 자식을 왼쪽(L), 가운데(M), 오른쪽(R) 순서로 들여 써서 찍는다. */
void printTST(const TSTNode *node, int depth, const char *tag) {
    if (node == NULL) {
        return;
    }
    printf("%*s%s%c", 2 * depth, "", tag, node->data);
    if (node->hasValue) {
        printf(" = %d", node->value);
    }
    printf("\n");
    printTST(node->left, depth + 1, "L ");
    printTST(node->middle, depth + 1, "M ");
    printTST(node->right, depth + 1, "R ");
}

static int compareWords(const void *a, const void *b) {
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}

void printSpace(const char *label, const char *words[], int n) {
    TSTNode *root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, words[i], i, 0, false);
    }
    int nodes = countNodes(root);
    printf("  %-8s%3d%7d%7d%7d%8d\n", label, n, nodes, nodes * 3, nodes * 3 - (nodes - 1),
           height(root));
    freeTST(root);
}

int main(void) {
    const char *words[] = {"apple", "application", "apply", "applaud",
                           "applet", "appliance", "applicable"};
    const int values[] = {10, 20, 30, 40, 50, 60, 70};
    const int n = 7;

    printf("insert");
    for (int i = 0; i < n; i++) {
        printf(" %s=%d", words[i], values[i]);
    }
    printf("\n");
    TSTNode *root = NULL;
    for (int i = 0; i < n; i++) {
        bool trace = strcmp(words[i], "appliance") == 0;
        if (trace) {
            printf("insert %s=%d\n", words[i], values[i]);
        }
        root = insert(root, words[i], values[i], 0, trace);
    }

    printf("\n");
    printTST(root, 0, "");

    printf("\nsearch\n");
    const char *queries[] = {"applaud", "apply", "application", "app", "apples"};
    for (int i = 0; i < 5; i++) {
        int visited;
        int value = search(root, queries[i], &visited);
        char shown[16];
        if (value >= 0) {
            snprintf(shown, sizeof shown, "%d", value);
        } else {
            strcpy(shown, "none");
        }
        printf("  %-12s-> %-6s visited %d\n", queries[i], shown, visited);
    }

    printf("\n");
    const char *prefixes[] = {"app", "appli", "apple", "b"};
    for (int i = 0; i < 4; i++) {
        WordValuePair results[MAX_RESULTS];
        int count = getWordsWithPrefix(root, prefixes[i], results);
        printf("getWordsWithPrefix(%s):", prefixes[i]);
        if (count == 0) {
            printf(" (none)");
        }
        for (int k = 0; k < count; k++) {
            printf(" %s(%d)", results[k].word, results[k].value);
        }
        printf("\n");
    }
    freeTST(root);

    printf("\nspace (3 links per node)\n");
    printf("  keys      N  nodes  links   null  height\n");
    const char *sorted[7];
    memcpy(sorted, words, sizeof sorted);
    qsort(sorted, 7, sizeof sorted[0], compareWords);
    const char *byThe[] = {"by", "sea", "sells", "she", "shells", "the"};
    printSpace("apple..", words, n);
    printSpace("sorted", sorted, n);
    printSpace("by..the", byThe, 6);
    return 0;
}
