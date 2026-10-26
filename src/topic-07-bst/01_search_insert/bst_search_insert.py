"""이진 탐색 트리 — 검색과 삽입.

찾는 값이 노드보다 작으면 왼쪽, 크면 오른쪽으로 내려간다. 삽입은 검색이
실패한 자리(비어 있는 자식)에 새 노드를 매단다. 같은 키라도 넣는 순서에
따라 트리의 높이가 달라지는 것까지 본다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 bst_search_insert.py`
"""


class TreeNode:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None


def insert(root, key):
    """key를 넣은 트리의 루트를 돌려준다. 같은 키가 이미 있으면 그대로 둔다."""
    if root is None:
        return TreeNode(key)
    if key < root.key:
        root.left = insert(root.left, key)
    elif key > root.key:
        root.right = insert(root.right, key)
    return root


def search(root, key, path):
    """key를 가진 노드를 돌려준다. 없으면 None.

    지나간 노드의 키를 path에 담는다. 한 노드에서 비교 한 번이다.
    """
    node = root
    path.clear()
    while node is not None:
        path.append(node.key)
        if key == node.key:
            return node
        node = node.left if key < node.key else node.right
    return None


def height(root):
    """높이 = 레벨의 수. 루트만 있으면 1, 비어 있으면 0."""
    if root is None:
        return 0
    return 1 + max(height(root.left), height(root.right))


def find_min(node):
    """가장 작은 키는 왼쪽 끝에 있다."""
    while node.left is not None:
        node = node.left
    return node


def find_max(node):
    """가장 큰 키는 오른쪽 끝에 있다."""
    while node.right is not None:
        node = node.right
    return node


def inorder(root, out):
    if root is not None:
        inorder(root.left, out)
        out.append(root.key)
        inorder(root.right, out)
    return out


def keys_text(keys):
    return " ".join(str(k) for k in keys)


def build_verbose(keys):
    """keys를 차례로 넣으며, 넣기 전에 검색이 지나간 길을 찍는다."""
    root = None
    path = []
    comparisons = 0
    for key in keys:
        search(root, key, path)
        comparisons += len(path)
        if not path:
            print(f"  insert {key:2d}: root")
        else:
            parent = path[-1]
            side = "left" if key < parent else "right"
            print(f"  insert {key:2d}: {keys_text(path)} -> {side} of {parent}")
        root = insert(root, key)
    return root, comparisons


def build(keys):
    """keys를 차례로 넣는다. 넣을 때마다 한 비교 횟수를 더해 돌려준다."""
    root = None
    path = []
    comparisons = 0
    for key in keys:
        search(root, key, path)
        comparisons += len(path)
        root = insert(root, key)
    return root, comparisons


def search_verbose(root, key):
    path = []
    found = search(root, key, path)
    result = "found" if found else "not found"
    print(f"  search {key}: {keys_text(path)} -> {result} ({len(path)} comparisons)")


if __name__ == "__main__":
    # 1. 삽입: 넣는 값마다 검색이 실패한 자리에 매달린다.
    keys = [5, 9, 7, 3, 8, 12, 6, 4, 20]
    print(f"insert {keys_text(keys)}")
    root, comparisons = build_verbose(keys)
    print(f"inorder = {keys_text(inorder(root, []))}")
    print(f"height = {height(root)}, comparisons = {comparisons}")
    print()

    # 2. 검색: 비교 한 번마다 한쪽 서브트리를 통째로 버린다.
    keys2 = [7, 4, 14, 1, 6, 16, 2, 5, 15]
    root, _ = build(keys2)
    print(f"tree {keys_text(keys2)}")
    search_verbose(root, 6)
    search_verbose(root, 13)
    print(f"  min = {find_min(root).key}, max = {find_max(root).key}")
    print()

    # 3. 같은 키, 다른 모양: 검색 비용은 높이를 따른다.
    shapes = ["SRXMHECA", "HCRAEMSX"]
    print("same keys, two shapes")
    for shape in shapes:
        root, _ = build(shape)
        print(f"  tree {keys_text(shape)}: height {height(root)}")
        print("  ", end="")
        search_verbose(root, "E")
    print()

    # 4. 넣는 순서가 모양을 정한다. 정렬된 순서면 한쪽으로 기운다.
    orders = ["DBFACEG", "BADCGFE", "ABCDEFG"]
    print("insert order -> shape")
    for order in orders:
        root, comparisons = build(order)
        print(f"  {keys_text(order)}: height {height(root)}, comparisons {comparisons}")
