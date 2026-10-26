/* 이진 탐색 트리 — 삭제.
 *
 * 지울 노드의 자식 수에 따라 세 경우로 나뉜다. 자식이 없으면 떼어 내고,
 * 하나면 그 자식을 부모에 바로 잇고, 둘이면 후속자(오른쪽 서브트리의 가장
 * 작은 값)를 그 자리에 복사한 뒤 원래 후속자를 지운다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-07-bst/02_delete/bstDelete.out
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

TreeNode *findMin(TreeNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/* key를 지운 트리의 루트를 돌려준다. */
TreeNode *deleteNode(TreeNode *root, char key) {
    if (root == NULL) {
        return NULL;
    }
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        /* 경우 1·2: 자식이 0개이거나 하나 */
        if (root->left == NULL) {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }
        /* 경우 3: 자식이 둘. 후속자를 복사하고 원래 후속자를 지운다. */
        TreeNode *succ = findMin(root->right);
        root->key = succ->key;
        root->right = deleteNode(root->right, succ->key);
    }
    return root;
}

static TreeNode *find(TreeNode *root, char key) {
    while (root != NULL && root->key != key) {
        root = key < root->key ? root->left : root->right;
    }
    return root;
}

void freeTree(TreeNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/* 모양을 한 줄로 찍는다. 키(왼쪽,오른쪽), 빈 자식은 -. */
static void printShape(TreeNode *root) {
    if (root == NULL) {
        printf("-");
        return;
    }
    printf("%c", root->key);
    if (root->left != NULL || root->right != NULL) {
        printf("(");
        printShape(root->left);
        printf(",");
        printShape(root->right);
        printf(")");
    }
}

static void printInorder(TreeNode *root) {
    if (root != NULL) {
        printInorder(root->left);
        printf(" %c", root->key);
        printInorder(root->right);
    }
}

static TreeNode *build(const char *keys) {
    TreeNode *root = NULL;
    for (const char *p = keys; *p; p++) {
        root = insert(root, *p);
    }
    return root;
}

/* 매번 새로 만든 트리에서 key 하나를 지운다. 어느 경우인지 먼저 찍는다. */
static void demo(const char *keys, char key) {
    TreeNode *root = build(keys);
    TreeNode *target = find(root, key);
    int children = (target->left != NULL) + (target->right != NULL);

    printf("delete %c: ", key);
    if (children == 0) {
        printf("no child, just remove it\n");
    } else if (children == 1) {
        TreeNode *child = target->left != NULL ? target->left : target->right;
        printf("one child %c, link it to the parent\n", child->key);
    } else {
        printf("two children, successor %c takes its place\n",
               findMin(target->right)->key);
    }
    root = deleteNode(root, key);
    printf("  tree    = ");
    printShape(root);
    printf("\n  inorder =");
    printInorder(root);
    printf("\n");
    freeTree(root);
}

int main(void) {
    const char *trees[] = {"BADCGFE", "JBQLRKNZMP"};
    const char *deletes[] = {"EFD", "QJ"};

    for (int t = 0; t < 2; t++) {
        TreeNode *root = build(trees[t]);
        printf("insert %s\n  tree    = ", trees[t]);
        printShape(root);
        printf("\n  inorder =");
        printInorder(root);
        printf("\n");
        freeTree(root);
        for (size_t i = 0; i < strlen(deletes[t]); i++) {
            demo(trees[t], deletes[t][i]);
        }
        if (t == 0) {
            printf("\n");
        }
    }
    return 0;
}
