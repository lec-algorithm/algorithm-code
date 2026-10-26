"""트리 순회 — 모든 노드를 한 번씩 방문하는 네 가지 순서.

중위·전위·후위는 재귀로, "현재 노드를 왼쪽·오른쪽 서브트리보다 언제
출력하느냐"만 다르다. 레벨 순서는 큐로 위에서부터 한 층씩 읽는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 traversal.py`
"""
from collections import deque


class TreeNode:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None


def insert(root, key):
    if root is None:
        return TreeNode(key)
    if key < root.key:
        root.left = insert(root.left, key)
    elif key > root.key:
        root.right = insert(root.right, key)
    return root


def inorder_traversal(root, out):
    """중위: [왼쪽] [현재] [오른쪽]"""
    if root is not None:
        inorder_traversal(root.left, out)
        out.append(root.key)
        inorder_traversal(root.right, out)
    return out


def preorder_traversal(root, out):
    """전위: [현재] [왼쪽] [오른쪽]"""
    if root is not None:
        out.append(root.key)
        preorder_traversal(root.left, out)
        preorder_traversal(root.right, out)
    return out


def postorder_traversal(root, out):
    """후위: [왼쪽] [오른쪽] [현재]"""
    if root is not None:
        postorder_traversal(root.left, out)
        postorder_traversal(root.right, out)
        out.append(root.key)
    return out


def levelorder_traversal(root, out):
    """레벨 순서: 큐에서 꺼내 출력하고, 자식을 왼쪽부터 큐 뒤에 넣는다."""
    queue = deque([root] if root is not None else [])
    while queue:
        node = queue.popleft()
        out.append(node.key)
        if node.left is not None:
            queue.append(node.left)
        if node.right is not None:
            queue.append(node.right)
    return out


def shape(root):
    """모양을 한 줄로 적는다. 키(왼쪽,오른쪽), 빈 자식은 -."""
    if root is None:
        return "-"
    if root.left is None and root.right is None:
        return root.key
    return f"{root.key}({shape(root.left)},{shape(root.right)})"


if __name__ == "__main__":
    trees = ["DBFACEG", "PFBHGSRYTWZ"]

    for t, keys in enumerate(trees):
        root = None
        for key in keys:
            root = insert(root, key)
        print(f"insert {keys}")
        print(f"  tree       = {shape(root)}")
        print(f"  inorder    = {' '.join(inorder_traversal(root, []))}")
        print(f"  preorder   = {' '.join(preorder_traversal(root, []))}")
        print(f"  postorder  = {' '.join(postorder_traversal(root, []))}")
        print(f"  levelorder = {' '.join(levelorder_traversal(root, []))}")
        if t == 0:
            print()
