/* 이진 탐색 트리 — 검색과 삽입.
 *
 * 찾는 값이 노드보다 작으면 왼쪽, 크면 오른쪽으로 내려간다. 삽입은 검색이
 * 실패한 자리(비어 있는 자식)에 새 노드를 매단다. 같은 키라도 넣는 순서에
 * 따라 트리의 높이가 달라지는 것까지 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-07-bst/01_search_insert/bstSearchInsert.out
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 32

typedef struct TreeNode {
    int key;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/* 새 노드. 자식은 둘 다 비어 있다. */
TreeNode *createNode(int key) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* key를 넣은 트리의 루트를 돌려준다. 같은 키가 이미 있으면 그대로 둔다. */
TreeNode *insert(TreeNode *root, int key) {
    if (root == NULL) {
        return createNode(key);
    }
    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    }
    return root;
}

/* key를 가진 노드를 돌려준다. 없으면 NULL.
 * 지나간 노드의 키를 path에 담는다. 한 노드에서 비교 한 번이다. */
TreeNode *search(TreeNode *root, int key, int path[], int *length) {
    TreeNode *node = root;
    *length = 0;
    while (node != NULL) {
        path[(*length)++] = node->key;
        if (key == node->key) {
            return node;
        }
        node = key < node->key ? node->left : node->right;
    }
    return NULL;
}

/* 높이 = 레벨의 수. 루트만 있으면 1, 비어 있으면 0. */
int height(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    int l = height(root->left);
    int r = height(root->right);
    return 1 + (l > r ? l : r);
}

/* 가장 작은 키는 왼쪽 끝, 가장 큰 키는 오른쪽 끝에 있다. */
TreeNode *findMin(TreeNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

TreeNode *findMax(TreeNode *node) {
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

void freeTree(TreeNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/* 키를 숫자로, letters가 1이면 글자로 찍는다. */
static void printKey(int key, int letters) {
    if (letters) {
        printf("%c", key);
    } else {
        printf("%d", key);
    }
}

static void printKeys(const int keys[], int n, int letters) {
    for (int i = 0; i < n; i++) {
        printf(i == 0 ? "" : " ");
        printKey(keys[i], letters);
    }
}

static void printInorder(TreeNode *root, int *first) {
    if (root != NULL) {
        printInorder(root->left, first);
        printf(*first ? "%d" : " %d", root->key);
        *first = 0;
        printInorder(root->right, first);
    }
}

/* keys를 차례로 넣으며, 넣기 전에 검색이 지나간 길을 찍는다. */
static TreeNode *buildVerbose(const int keys[], int n, int *comparisons) {
    TreeNode *root = NULL;
    int path[MAX_KEYS];
    int length;
    *comparisons = 0;
    for (int i = 0; i < n; i++) {
        search(root, keys[i], path, &length);
        *comparisons += length;
        printf("  insert %2d:", keys[i]);
        if (length == 0) {
            printf(" root\n");
        } else {
            printf(" ");
            printKeys(path, length, 0);
            int parent = path[length - 1];
            printf(" -> %s of %d\n", keys[i] < parent ? "left" : "right", parent);
        }
        root = insert(root, keys[i]);
    }
    return root;
}

/* keys를 차례로 넣는다. 넣을 때마다 한 비교 횟수를 더해 돌려준다. */
static TreeNode *build(const int keys[], int n, int *comparisons) {
    TreeNode *root = NULL;
    int path[MAX_KEYS];
    int length;
    *comparisons = 0;
    for (int i = 0; i < n; i++) {
        search(root, keys[i], path, &length);
        *comparisons += length;
        root = insert(root, keys[i]);
    }
    return root;
}

static void searchVerbose(TreeNode *root, int key, int letters) {
    int path[MAX_KEYS];
    int length;
    TreeNode *found = search(root, key, path, &length);
    printf("  search ");
    printKey(key, letters);
    printf(": ");
    printKeys(path, length, letters);
    printf(" -> %s (%d comparisons)\n", found ? "found" : "not found", length);
}

int main(void) {
    int comparisons;

    /* 1. 삽입: 넣는 값마다 검색이 실패한 자리에 매달린다. */
    int keys[] = {5, 9, 7, 3, 8, 12, 6, 4, 20};
    int n = sizeof(keys) / sizeof(keys[0]);
    printf("insert ");
    printKeys(keys, n, 0);
    printf("\n");
    TreeNode *root = buildVerbose(keys, n, &comparisons);
    int first = 1;
    printf("inorder = ");
    printInorder(root, &first);
    printf("\nheight = %d, comparisons = %d\n\n", height(root), comparisons);
    freeTree(root);

    /* 2. 검색: 비교 한 번마다 한쪽 서브트리를 통째로 버린다. */
    int keys2[] = {7, 4, 14, 1, 6, 16, 2, 5, 15};
    int n2 = sizeof(keys2) / sizeof(keys2[0]);
    root = build(keys2, n2, &comparisons);
    printf("tree ");
    printKeys(keys2, n2, 0);
    printf("\n");
    searchVerbose(root, 6, 0);
    searchVerbose(root, 13, 0);
    printf("  min = %d, max = %d\n\n", findMin(root)->key, findMax(root)->key);
    freeTree(root);

    /* 3. 같은 키, 다른 모양: 검색 비용은 높이를 따른다. */
    int shapes[][8] = {
        {'S', 'R', 'X', 'M', 'H', 'E', 'C', 'A'},
        {'H', 'C', 'R', 'A', 'E', 'M', 'S', 'X'},
    };
    printf("same keys, two shapes\n");
    for (int i = 0; i < 2; i++) {
        root = build(shapes[i], 8, &comparisons);
        printf("  tree ");
        printKeys(shapes[i], 8, 1);
        printf(": height %d\n", height(root));
        printf("  ");
        searchVerbose(root, 'E', 1);
        freeTree(root);
    }
    printf("\n");

    /* 4. 넣는 순서가 모양을 정한다. 정렬된 순서면 한쪽으로 기운다. */
    int orders[][7] = {
        {'D', 'B', 'F', 'A', 'C', 'E', 'G'},
        {'B', 'A', 'D', 'C', 'G', 'F', 'E'},
        {'A', 'B', 'C', 'D', 'E', 'F', 'G'},
    };
    printf("insert order -> shape\n");
    for (int i = 0; i < 3; i++) {
        root = build(orders[i], 7, &comparisons);
        printf("  ");
        printKeys(orders[i], 7, 1);
        printf(": height %d, comparisons %d\n", height(root), comparisons);
        freeTree(root);
    }
    return 0;
}
