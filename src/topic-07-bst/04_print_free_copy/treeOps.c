/* 트리 출력·해제·복사 — 순회로 트리 전체를 다룬다.
 *
 * 모양 출력과 복사는 전위(부모 먼저), 해제는 후위(자식 먼저), 정렬된
 * 출력은 중위다. 끝으로 순회 결과를 저장했다가 다시 넣으면 어떤 트리가
 * 되는지 본다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-07-bst/04_print_free_copy/treeOps.out
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 64

typedef struct TreeNode {
    char key;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *createNode(char key) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode *insert(TreeNode *root, char key) {
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

/* 모양 출력. 현재를 먼저 적고 자식을 들여 적는다(전위). 왼쪽이 위다. */
void printTree(TreeNode *root, const char *prefix, int isLast) {
    printf("%s%s", prefix, isLast ? "└── " : "├── ");
    if (root == NULL) {
        printf("-\n");
        return;
    }
    printf("%c\n", root->key);

    if (root->left != NULL || root->right != NULL) {
        char next[256];
        snprintf(next, sizeof(next), "%s%s", prefix, isLast ? "    " : "│   ");
        printTree(root->left, next, 0);
        printTree(root->right, next, 1);
    }
}

/* 트리 해제. 자식을 모두 해제한 뒤 자기를 해제한다(후위). */
TreeNode *clearTree(TreeNode *root) {
    if (root != NULL) {
        clearTree(root->left);
        clearTree(root->right);
        printf(" %c", root->key);
        free(root);
    }
    return NULL;
}

/* 트리 복사. 자기를 먼저 만들고 자식을 복사해 매단다(전위). */
TreeNode *copyTree(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }
    TreeNode *copy = createNode(root->key);
    printf(" %c", root->key);
    copy->left = copyTree(root->left);
    copy->right = copyTree(root->right);
    return copy;
}

/* 출력 없이 해제한다. 순서는 clearTree와 같다. */
static void freeTree(TreeNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/* 두 트리의 모양과 키가 같은가 */
int sameTree(TreeNode *a, TreeNode *b) {
    if (a == NULL || b == NULL) {
        return a == b;
    }
    return a->key == b->key && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

int height(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    int l = height(root->left);
    int r = height(root->right);
    return 1 + (l > r ? l : r);
}

/* 순회 결과를 out에 문자열로 담는다. */
static void inorderKeys(TreeNode *root, char out[], int *n) {
    if (root != NULL) {
        inorderKeys(root->left, out, n);
        out[(*n)++] = root->key;
        inorderKeys(root->right, out, n);
    }
}

static void preorderKeys(TreeNode *root, char out[], int *n) {
    if (root != NULL) {
        out[(*n)++] = root->key;
        preorderKeys(root->left, out, n);
        preorderKeys(root->right, out, n);
    }
}

static void printKeys(const char *label, const char *keys) {
    printf("%s", label);
    for (const char *p = keys; *p; p++) {
        printf(" %c", *p);
    }
    printf("\n");
}

static TreeNode *build(const char *keys) {
    TreeNode *root = NULL;
    for (const char *p = keys; *p; p++) {
        root = insert(root, *p);
    }
    return root;
}

/* 저장해 둔 순서대로 다시 넣어 원래 트리와 비교한다. */
static void rebuild(const char *label, const char *keys, TreeNode *original) {
    TreeNode *again = build(keys);
    printf("%s height %d, same shape: %s\n", label, height(again),
           sameTree(again, original) ? "yes" : "no");
    freeTree(again);
}

int main(void) {
    const char *keys = "PFBHGSRYTWZ";
    char inorder[MAX_NODES] = {0};
    char preorder[MAX_NODES] = {0};
    int n;

    TreeNode *root = build(keys);
    printf("insert %s\n\n", keys);
    printf("%c\n", root->key);
    printTree(root->left, "", 0);
    printTree(root->right, "", 1);
    printf("\n");

    n = 0;
    inorderKeys(root, inorder, &n);
    printKeys("sorted (inorder)  :", inorder);

    printf("copy   (preorder) :");
    TreeNode *copy = copyTree(root);
    printf("\ncopy is same      : %s\n", sameTree(root, copy) ? "yes" : "no");

    printf("free   (postorder):");
    copy = clearTree(copy);
    printf("\ncopy after free   : %s\n\n", copy == NULL ? "NULL" : "not NULL");

    /* 순회 결과를 파일에 적었다가 다시 읽어 트리를 만든다고 하자. */
    n = 0;
    preorderKeys(root, preorder, &n);
    rebuild("rebuild from preorder:", preorder, root);
    rebuild("rebuild from inorder :", inorder, root);

    freeTree(root);
    return 0;
}
