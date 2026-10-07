/* 허프만 코드 — 가장 드문 두 트리부터 합쳐 접두사 없는 코드를 만든다.
 *
 * 흔한 문자는 짧은 코드, 드문 문자는 긴 코드를 받는다. 만든 코드로
 * 인코딩하고, 트리를 따라 다시 디코딩해 원문이 돌아오는지 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-06-greedy/04_huffman_code/huffmanCode.out
 */
#include <stdio.h>
#include <string.h>

#define SYMBOLS 256
#define MAX_NODES (2 * SYMBOLS)
#define MAX_BITS 1024

typedef struct {
    int weight;
    unsigned char minSym; /* 트리 안의 가장 작은 문자. 가중치가 같을 때 순서 */
    int left;             /* 잎이면 -1 */
    int right;
    unsigned char sym;    /* 잎의 문자 */
    char label[SYMBOLS];  /* 출력용. 잎들의 문자를 왼쪽부터 이어 붙인 것 */
} Node;

static Node nodes[MAX_NODES];
static int nodeCount = 0;
static char codes[SYMBOLS][SYMBOLS];

static int newNode(int weight, unsigned char minSym, int left, int right) {
    Node *node = &nodes[nodeCount];
    node->weight = weight;
    node->minSym = minSym;
    node->left = left;
    node->right = right;
    node->sym = minSym;
    node->label[0] = '\0';
    return nodeCount++;
}

/* a가 b보다 먼저 꺼낼 트리인가. 가중치, 같으면 가장 작은 문자. */
static int before(int a, int b) {
    if (nodes[a].weight != nodes[b].weight) {
        return nodes[a].weight < nodes[b].weight;
    }
    return nodes[a].minSym < nodes[b].minSym;
}

/* forest에서 가장 먼저 꺼낼 트리를 빼서 돌려준다. */
static int popMin(int forest[], int *size) {
    int best = 0;
    for (int i = 1; i < *size; i++) {
        if (before(forest[i], forest[best])) {
            best = i;
        }
    }
    int node = forest[best];
    forest[best] = forest[--(*size)];
    return node;
}

/* freq로 허프만 트리를 만들고 루트를 돌려준다. 합칠 때마다 한 줄씩 찍는다. */
int huffman(const int freq[]) {
    int forest[SYMBOLS];
    int size = 0;
    nodeCount = 0;
    for (int c = 0; c < SYMBOLS; c++) {
        if (freq[c] > 0) {
            int leaf = newNode(freq[c], (unsigned char)c, -1, -1);
            nodes[leaf].label[0] = (char)c;
            nodes[leaf].label[1] = '\0';
            forest[size++] = leaf;
        }
    }

    for (int step = 1; size > 1; step++) {
        int x = popMin(forest, &size);
        int y = popMin(forest, &size);
        unsigned char minSym = nodes[x].minSym < nodes[y].minSym
                                   ? nodes[x].minSym : nodes[y].minSym;
        int z = newNode(nodes[x].weight + nodes[y].weight, minSym, x, y);
        char label[SYMBOLS];
        snprintf(label, SYMBOLS, "%s%s", nodes[x].label, nodes[y].label);
        strcpy(nodes[z].label, label);
        printf("merge %d: %s(%d) + %s(%d) -> %s(%d)\n", step,
               nodes[x].label, nodes[x].weight, nodes[y].label, nodes[y].weight,
               nodes[z].label, nodes[z].weight);
        forest[size++] = z;
    }
    return forest[0];
}

/* 루트에서 각 잎까지의 경로를 코드로 적는다. 왼쪽 0, 오른쪽 1. */
static void assignCodes(int node, char path[], int depth) {
    if (nodes[node].left == -1) {
        path[depth] = '\0';
        strcpy(codes[nodes[node].sym], depth > 0 ? path : "0");
        return;
    }
    path[depth] = '0';
    assignCodes(nodes[node].left, path, depth + 1);
    path[depth] = '1';
    assignCodes(nodes[node].right, path, depth + 1);
}

void encode(const char *text, char bits[]) {
    bits[0] = '\0';
    for (const char *p = text; *p; p++) {
        strcat(bits, codes[(unsigned char)*p]);
    }
}

void decode(const char *bits, int root, char out[]) {
    int node = root;
    int n = 0;
    for (const char *p = bits; *p; p++) {
        node = *p == '0' ? nodes[node].left : nodes[node].right;
        if (nodes[node].left == -1) {
            out[n++] = (char)nodes[node].sym;
            node = root;
        }
    }
    out[n] = '\0';
}

int main(void) {
    const char *text = "ABRACADABRA!";
    int freq[SYMBOLS] = {0};
    char path[SYMBOLS];
    char bits[MAX_BITS];
    char decoded[MAX_BITS];
    int kinds = 0;

    for (const char *p = text; *p; p++) {
        freq[(unsigned char)*p]++;
    }

    printf("text = %s\n", text);
    printf("freq :");
    for (int c = 0; c < SYMBOLS; c++) {
        if (freq[c] > 0) {
            printf(" %c=%d", c, freq[c]);
            kinds++;
        }
    }
    printf("\n\n");

    int root = huffman(freq);
    assignCodes(root, path, 0);

    printf("\ncodes:");
    for (int c = 0; c < SYMBOLS; c++) {
        if (freq[c] > 0) {
            printf(" %c=%s", c, codes[c]);
        }
    }
    printf("\n");

    encode(text, bits);
    decode(bits, root, decoded);

    int fixed = 0;
    while ((1 << fixed) < kinds) {
        fixed++;
    }
    int length = (int)strlen(text);
    printf("encoded = %s\n", bits);
    printf("bits: huffman %d, fixed %d-bit %d, ascii %d\n",
           (int)strlen(bits), fixed, fixed * length, 8 * length);
    printf("decoded = %s\n", decoded);
    return 0;
}
