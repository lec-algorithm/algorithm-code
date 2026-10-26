/* 트리 순회 — 모든 노드를 한 번씩 방문하는 네 가지 순서.
 *
 * 중위·전위·후위는 재귀로, "현재 노드를 왼쪽·오른쪽 서브트리보다 언제
 * 출력하느냐"만 다르다. 레벨 순서는 큐로 위에서부터 한 층씩 읽는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-07-bst/03_traversal/traversal.out
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

/* 중위: [왼쪽] [현재] [오른쪽] */
void inorderTraversal(TreeNode *root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf(" %c", root->key);
        inorderTraversal(root->right);
    }
}

/* 전위: [현재] [왼쪽] [오른쪽] */
void preorderTraversal(TreeNode *root) {
    if (root != NULL) {
        printf(" %c", root->key);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

/* 후위: [왼쪽] [오른쪽] [현재] */
void postorderTraversal(TreeNode *root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf(" %c", root->key);
    }
}

/* 레벨 순서: 큐에서 꺼내 출력하고, 자식을 왼쪽부터 큐 뒤에 넣는다. */
void levelorderTraversal(TreeNode *root) {
    TreeNode *queue[MAX_NODES];
    int head = 0;
    int tail = 0;
    if (root != NULL) {
        queue[tail++] = root;
    }
    while (head < tail) {
        TreeNode *node = queue[head++];
        printf(" %c", node->key);
        if (node->left != NULL) {
            queue[tail++] = node->left;
        }
        if (node->right != NULL) {
            queue[tail++] = node->right;
        }
    }
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

int main(void) {
    const char *trees[] = {"DBFACEG", "PFBHGSRYTWZ"};

    for (int t = 0; t < 2; t++) {
        TreeNode *root = NULL;
        for (const char *p = trees[t]; *p; p++) {
            root = insert(root, *p);
        }
        printf("insert %s\n", trees[t]);
        printf("  tree       = ");
        printShape(root);
        printf("\n  inorder    =");
        inorderTraversal(root);
        printf("\n  preorder   =");
        preorderTraversal(root);
        printf("\n  postorder  =");
        postorderTraversal(root);
        printf("\n  levelorder =");
        levelorderTraversal(root);
        printf("\n");
        if (t == 0) {
            printf("\n");
        }
        freeTree(root);
    }
    return 0;
}
