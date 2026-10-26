"""이진 탐색 트리 — 삭제.

지울 노드의 자식 수에 따라 세 경우로 나뉜다. 자식이 없으면 떼어 내고,
하나면 그 자식을 부모에 바로 잇고, 둘이면 후속자(오른쪽 서브트리의 가장
작은 값)를 그 자리에 복사한 뒤 원래 후속자를 지운다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 bst_delete.py`
"""


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


def find_min(node):
    while node.left is not None:
        node = node.left
    return node


def delete_node(root, key):
    """key를 지운 트리의 루트를 돌려준다."""
    if root is None:
        return None
    if key < root.key:
        root.left = delete_node(root.left, key)
    elif key > root.key:
        root.right = delete_node(root.right, key)
    else:
        # 경우 1·2: 자식이 0개이거나 하나
        if root.left is None:
            return root.right
        if root.right is None:
            return root.left
        # 경우 3: 자식이 둘. 후속자를 복사하고 원래 후속자를 지운다.
        succ = find_min(root.right)
        root.key = succ.key
        root.right = delete_node(root.right, succ.key)
    return root


def find(root, key):
    while root is not None and root.key != key:
        root = root.left if key < root.key else root.right
    return root


def shape(root):
    """모양을 한 줄로 적는다. 키(왼쪽,오른쪽), 빈 자식은 -."""
    if root is None:
        return "-"
    if root.left is None and root.right is None:
        return root.key
    return f"{root.key}({shape(root.left)},{shape(root.right)})"


def inorder(root, out):
    if root is not None:
        inorder(root.left, out)
        out.append(root.key)
        inorder(root.right, out)
    return out


def build(keys):
    root = None
    for key in keys:
        root = insert(root, key)
    return root


def print_tree(root):
    print(f"  tree    = {shape(root)}")
    print(f"  inorder = {' '.join(inorder(root, []))}")


def demo(keys, key):
    """매번 새로 만든 트리에서 key 하나를 지운다. 어느 경우인지 먼저 찍는다."""
    root = build(keys)
    target = find(root, key)
    children = (target.left is not None) + (target.right is not None)

    if children == 0:
        print(f"delete {key}: no child, just remove it")
    elif children == 1:
        child = target.left if target.left is not None else target.right
        print(f"delete {key}: one child {child.key}, link it to the parent")
    else:
        succ = find_min(target.right)
        print(f"delete {key}: two children, successor {succ.key} takes its place")
    root = delete_node(root, key)
    print_tree(root)


if __name__ == "__main__":
    trees = ["BADCGFE", "JBQLRKNZMP"]
    deletes = ["EFD", "QJ"]

    for t, keys in enumerate(trees):
        print(f"insert {keys}")
        print_tree(build(keys))
        for key in deletes[t]:
            demo(keys, key)
        if t == 0:
            print()
