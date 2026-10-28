"""트리 출력·해제·복사 — 순회로 트리 전체를 다룬다.

모양 출력과 복사는 전위(부모 먼저), 해제는 후위(자식 먼저), 정렬된
출력은 중위다. 끝으로 순회 결과를 저장했다가 다시 넣으면 어떤 트리가
되는지 본다.

Python은 쓰지 않는 객체를 알아서 거두지만, C의 free와 같은 순서로
링크를 끊어 해제 순서를 보여 준다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 tree_ops.py`
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


def print_tree(root, prefix, is_last):
    """모양 출력. 현재를 먼저 적고 자식을 들여 적는다(전위). 왼쪽이 위다."""
    branch = "└── " if is_last else "├── "
    if root is None:
        print(f"{prefix}{branch}-")
        return
    print(f"{prefix}{branch}{root.key}")

    if root.left is not None or root.right is not None:
        nxt = prefix + ("    " if is_last else "│   ")
        print_tree(root.left, nxt, False)
        print_tree(root.right, nxt, True)


def clear_tree(root):
    """트리 해제. 자식을 모두 해제한 뒤 자기를 해제한다(후위)."""
    if root is not None:
        clear_tree(root.left)
        clear_tree(root.right)
        print(f" {root.key}", end="")
        root.left = root.right = None  # C의 free(root)에 해당
    return None


def copy_tree(root):
    """트리 복사. 자기를 먼저 만들고 자식을 복사해 매단다(전위)."""
    if root is None:
        return None
    copy = TreeNode(root.key)
    print(f" {root.key}", end="")
    copy.left = copy_tree(root.left)
    copy.right = copy_tree(root.right)
    return copy


def same_tree(a, b):
    """두 트리의 모양과 키가 같은가"""
    if a is None or b is None:
        return a is b
    return a.key == b.key and same_tree(a.left, b.left) and same_tree(a.right, b.right)


def height(root):
    if root is None:
        return 0
    return 1 + max(height(root.left), height(root.right))


def inorder_keys(root, out):
    if root is not None:
        inorder_keys(root.left, out)
        out.append(root.key)
        inorder_keys(root.right, out)
    return out


def preorder_keys(root, out):
    if root is not None:
        out.append(root.key)
        preorder_keys(root.left, out)
        preorder_keys(root.right, out)
    return out


def build(keys):
    root = None
    for key in keys:
        root = insert(root, key)
    return root


def rebuild(label, keys, original):
    """저장해 둔 순서대로 다시 넣어 원래 트리와 비교한다."""
    again = build(keys)
    same = "yes" if same_tree(again, original) else "no"
    print(f"{label} height {height(again)}, same shape: {same}")


if __name__ == "__main__":
    keys = "PFBHGSRYTWZ"

    root = build(keys)
    print(f"insert {keys}")
    print()
    print(root.key)
    print_tree(root.left, "", False)
    print_tree(root.right, "", True)
    print()

    inorder = inorder_keys(root, [])
    print(f"sorted (inorder)  : {' '.join(inorder)}")

    print("copy   (preorder) :", end="")
    copy = copy_tree(root)
    print()
    print(f"copy is same      : {'yes' if same_tree(root, copy) else 'no'}")

    print("free   (postorder):", end="")
    copy = clear_tree(copy)
    print()
    print(f"copy after free   : {'NULL' if copy is None else 'not NULL'}")
    print()

    # 순회 결과를 파일에 적었다가 다시 읽어 트리를 만든다고 하자.
    preorder = preorder_keys(root, [])
    rebuild("rebuild from preorder:", preorder, root)
    rebuild("rebuild from inorder :", inorder, root)
