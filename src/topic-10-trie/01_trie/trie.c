/* 트라이(trie) — 키를 문자 하나씩 경로로 저장하는 트리.
 *
 * 노드마다 a-z 26개의 링크를 두고, 키의 마지막 문자에 해당하는 노드에 값을
 * 둔다. 삽입, 검색, 접두사로 모두 찾기, 삭제를 차례로 돌리고, 링크가 몇 개나
 * 비어 있는지와 검색 실패가 노드 몇 개에서 끝나는지 센다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-10-trie/01_trie/trie.out
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26 /* R: 알파벳 크기 (a-z) */
#define MAX_WORD 64
#define MAX_RESULTS 32

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE]; /* a-z 링크. 비어 있으면 NULL */
    bool hasValue;                            /* 이 노드에서 끝나는 키가 있는가 */
    int value;                                /* 그 키의 값 */
} TrieNode;

typedef struct {
    char word[MAX_WORD];
    int value;
} WordValuePair;

TrieNode *createTrieNode(void) {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (node == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    node->hasValue = false;
    node->value = 0;
    return node;
}

/* word를 문자 하나씩 따라 내려가며, 링크가 없으면 새 노드를 만든다. */
void insert(TrieNode *root, const char *word, int value) {
    TrieNode *current = root;
    for (const char *p = word; *p; p++) {
        int index = *p - 'a'; /* 문자를 배열 인덱스로 (a=0, b=1, ...) */
        if (current->children[index] == NULL) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->hasValue = true; /* 키의 마지막 문자에 해당하는 노드에 값 */
    current->value = value;
}

/* word의 값, 없으면 -1. visited가 NULL이 아니면 거친 노드 수(루트 포함)를 담는다. */
int search(TrieNode *root, const char *word, int *visited) {
    TrieNode *current = root;
    int count = 1;
    int result = -1;
    const char *p = word;
    for (; *p; p++) {
        int index = *p - 'a';
        if (current->children[index] == NULL) {
            break; /* 링크가 없다: 실패 */
        }
        current = current->children[index];
        count++;
    }
    if (*p == '\0' && current->hasValue) {
        result = current->value; /* 노드는 있지만 값이 없으면 실패 */
    }
    if (visited != NULL) {
        *visited = count;
    }
    return result;
}

/* node 아래의 키를 모두 모은다. a부터 z 순서라 결과가 사전 순이다. */
void getAllWordsHelper(TrieNode *node, char *prefix, int prefixLen,
                       WordValuePair results[], int *count) {
    if (node == NULL) {
        return;
    }
    if (node->hasValue && *count < MAX_RESULTS) {
        prefix[prefixLen] = '\0';
        strcpy(results[*count].word, prefix);
        results[*count].value = node->value;
        (*count)++;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            prefix[prefixLen] = (char)('a' + i);
            getAllWordsHelper(node->children[i], prefix, prefixLen + 1, results, count);
        }
    }
    prefix[prefixLen] = '\0'; /* 되돌린다 */
}

/* prefix로 시작하는 (키, 값)을 results에 담고 개수를 돌려준다. */
int getWordsWithPrefix(TrieNode *root, const char *prefix, WordValuePair results[]) {
    TrieNode *current = root;
    for (const char *p = prefix; *p; p++) { /* 1. 접두사의 마지막 문자 노드로 간다 */
        int index = *p - 'a';
        if (current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }
    char buffer[MAX_WORD];
    strcpy(buffer, prefix);
    int count = 0;
    getAllWordsHelper(current, buffer, (int)strlen(prefix), results, &count); /* 2. 모은다 */
    return count;
}

static bool hasChildren(const TrieNode *node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            return true;
        }
    }
    return false;
}

/* node를 부모에서 떼어 내도 되면 true. 떼어 낸 노드의 문자를 removed 뒤에 붙인다. */
bool deleteHelper(TrieNode *node, const char *word, int index, char *removed) {
    int wordLen = (int)strlen(word);
    if (index == wordLen) { /* 키의 마지막 문자 노드에 도착 */
        if (!node->hasValue) {
            return false; /* 없는 키 */
        }
        node->hasValue = false;
        node->value = 0;
        return !hasChildren(node);
    }
    int i = word[index] - 'a';
    TrieNode *child = node->children[i];
    if (child == NULL) {
        return false; /* 없는 키 */
    }
    if (deleteHelper(child, word, index + 1, removed)) {
        free(child); /* 값도 자식도 없는 노드를 떼어 낸다 */
        node->children[i] = NULL;
        size_t n = strlen(removed);
        removed[n] = word[index];
        removed[n + 1] = '\0';
        return !node->hasValue && !hasChildren(node);
    }
    return false;
}

void deleteWord(TrieNode *root, const char *word, char *removed) {
    removed[0] = '\0';
    deleteHelper(root, word, 0, removed); /* 루트는 지우지 않는다 */
}

/* 루트를 포함한 노드 수 */
int countNodes(const TrieNode *node) {
    if (node == NULL) {
        return 0;
    }
    int count = 1;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        count += countNodes(node->children[i]);
    }
    return count;
}

void freeTrie(TrieNode *node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(node->children[i]);
    }
    free(node);
}

/* 자식을 a부터 z 순서로, 깊이만큼 들여 써서 찍는다. 값이 있으면 = 값 */
void printTrie(const TrieNode *node, int depth) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        const TrieNode *child = node->children[i];
        if (child != NULL) {
            printf("%*s%c", 2 * depth, "", 'a' + i);
            if (child->hasValue) {
                printf(" = %d", child->value);
            }
            printf("\n");
            printTrie(child, depth + 1);
        }
    }
}

void printSearch(TrieNode *root, const char *word) {
    int visited;
    int value = search(root, word, &visited);
    char shown[16];
    if (value >= 0) {
        snprintf(shown, sizeof shown, "%d", value);
    } else {
        strcpy(shown, "none");
    }
    printf("  %-7s-> %-6s visited %d: root", word, shown, visited);
    for (int i = 0; i < visited - 1; i++) {
        printf(" %c", word[i]);
    }
    if (value < 0) {
        if (visited == (int)strlen(word) + 1) {
            printf(" (no value)");
        } else {
            printf(" (no link '%c')", word[visited - 1]);
        }
    }
    printf("\n");
}

void printSpace(const char *label, const char *words[], int n) {
    TrieNode *root = createTrieNode();
    int chars = 0;
    for (int i = 0; i < n; i++) {
        insert(root, words[i], i);
        chars += (int)strlen(words[i]);
    }
    int nodes = countNodes(root);
    printf("  %-8s%3d%7.2f%7d%7d%7d%7d%7d\n", label, n, (double)chars / n, nodes,
           nodes * ALPHABET_SIZE, nodes * ALPHABET_SIZE - (nodes - 1),
           ALPHABET_SIZE * n, ALPHABET_SIZE * chars);
    freeTrie(root);
}

/* 검색 실패 실험에 쓰는 난수. 두 구현이 같은 수열을 내도록 직접 만든다. */
static unsigned long long seed = 0;

static int nextRandom(void) {
    seed = (seed * 1103515245ULL + 12345ULL) % 2147483648ULL;
    return (int)(seed >> 16);
}

static void randomKey(char *key, int length) {
    for (int i = 0; i < length; i++) {
        key[i] = (char)('a' + nextRandom() % ALPHABET_SIZE);
    }
    key[length] = '\0';
}

/* 무작위 키 n개로 트라이를 만들고, 무작위 키로 검색할 때 거치는 노드 수의 평균 */
void searchMiss(int n, int length, int queries) {
    seed = 2026;
    TrieNode *root = createTrieNode();
    char key[MAX_WORD];
    for (int i = 0; i < n; i++) {
        randomKey(key, length);
        insert(root, key, i);
    }
    int total = 0;
    for (int q = 0; q < queries; q++) {
        int visited;
        randomKey(key, length);
        search(root, key, &visited);
        total += visited;
    }
    printf("  N = %-6d average visited = %.2f\n", n, (double)total / queries);
    freeTrie(root);
}

int main(void) {
    const char *words[] = {"by", "sea", "sells", "she", "shells", "the"};
    const int values[] = {4, 2, 1, 0, 3, 5};
    const int n = 6;

    TrieNode *root = createTrieNode();
    printf("insert");
    for (int i = 0; i < n; i++) {
        printf(" %s=%d", words[i], values[i]);
        insert(root, words[i], values[i]);
    }
    printf("\n");
    printTrie(root, 1);
    int nodes = countNodes(root);
    printf("nodes = %d (with root), links = %d x %d = %d, non-null = %d\n", nodes, nodes,
           ALPHABET_SIZE, nodes * ALPHABET_SIZE, nodes - 1);

    printf("\nsearch\n");
    const char *queries[] = {"shells", "shell", "she", "shore"};
    for (int i = 0; i < 4; i++) {
        printSearch(root, queries[i]);
    }

    printf("\ninsert shore=7\n");
    insert(root, "shore", 7);
    const char *prefixes[] = {"sh", "se", "t", "x"};
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

    char removed[MAX_WORD];
    deleteWord(root, "shells", removed);
    printf("\ndelete shells: removed");
    for (const char *p = removed; *p; p++) {
        printf(" %c", *p);
    }
    printf("\n");
    const char *after[] = {"shells", "she", "shore"};
    for (int i = 0; i < 3; i++) {
        printSearch(root, after[i]);
    }
    printf("nodes = %d\n", countNodes(root));
    freeTrie(root);

    printf("\nspace (R = %d, w = average key length)\n", ALPHABET_SIZE);
    printf("  keys      N      w  nodes  links   null     RN    RNw\n");
    const char *chain[] = {"s", "she", "sh", "shells", "shel", "shell"};
    const char *spread[] = {"shells", "an", "bat", "the", "yell", "ear"};
    const char *apple[] = {"apple", "application", "apply", "applaud",
                           "applet", "appliance", "applicable"};
    printSpace("by..the", words, n);
    printSpace("chain", chain, 6);
    printSpace("spread", spread, 6);
    printSpace("apple..", apple, 7);

    printf("\nsearch miss, random keys of length 7, 1000 searches\n");
    const int sizes[] = {100, 1000, 10000};
    for (int i = 0; i < 3; i++) {
        searchMiss(sizes[i], 7, 1000);
    }
    return 0;
}
