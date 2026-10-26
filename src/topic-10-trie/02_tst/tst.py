"""TST(ternary search tree) — 노드마다 링크 3개로 줄인 트라이.

노드는 문자 하나를 갖고, 그보다 작은 문자는 왼쪽, 큰 문자는 오른쪽, 같으면
가운데로 내려가 다음 문자를 본다. 삽입(한 단어는 한 걸음씩), 검색, 접두사로
모두 찾기를 돌리고, 같은 키를 담는 데 링크가 몇 개 드는지 센다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 tst.py`
"""


class TSTNode:
    def __init__(self, data):
        self.data = data         # 이 노드가 나타내는 문자
        self.has_value = False   # 이 노드에서 끝나는 키가 있는가
        self.value = 0           # 그 키의 값
        self.left = None         # data보다 작은 문자 (같은 자리)
        self.middle = None       # 다음 자리의 문자
        self.right = None        # data보다 큰 문자 (같은 자리)


def create_tst_node(data):
    return TSTNode(data)


def insert(root, word, value, index, trace=False):
    """갱신된 서브트리의 루트를 돌려준다. trace면 비교 한 번마다 한 줄 찍는다."""
    c = word[index]
    if root is None:
        root = create_tst_node(c)       # 아무것도 없으면 새 노드
        if trace:
            last = " = %d" % value if index == len(word) - 1 else ""
            print(f"  {c}       new node{last}")
        if index < len(word) - 1:
            root.middle = insert(root.middle, word, value, index + 1, trace)
        else:
            root.has_value = True
            root.value = value
        return root
    if c < root.data:                   # 작으면 왼쪽, 같은 자리
        if trace:
            print(f"  {c} < {root.data}   left")
        root.left = insert(root.left, word, value, index, trace)
    elif c > root.data:                 # 크면 오른쪽, 같은 자리
        if trace:
            print(f"  {c} > {root.data}   right")
        root.right = insert(root.right, word, value, index, trace)
    elif index < len(word) - 1:         # 같으면 가운데, 다음 자리
        if trace:
            print(f"  {c} = {root.data}   middle")
        root.middle = insert(root.middle, word, value, index + 1, trace)
    else:                               # 키의 마지막 문자: 값을 둔다
        if trace:
            print(f"  {c} = {root.data}   value = {value}")
        root.has_value = True
        root.value = value
    return root


def get_node(root, word, trace=None):
    """word의 마지막 문자에 해당하는 노드, 없으면 None. trace에 거친 노드 수를 담는다."""
    node = root
    index = 0
    visited = 0
    found = None
    while node is not None:
        visited += 1
        c = word[index]
        if c < node.data:
            node = node.left
        elif c > node.data:
            node = node.right
        elif index == len(word) - 1:
            found = node
            break
        else:
            node = node.middle
            index += 1
    if trace is not None:
        trace.append(visited)
    return found


def search(root, word, trace=None):
    """word의 값, 없으면 -1."""
    node = get_node(root, word, trace)
    if node is not None and node.has_value:
        return node.value
    return -1


def get_all_words_helper(node, prefix, results):
    """왼쪽, 자기, 가운데, 오른쪽 순서로 돌면 사전 순이 된다."""
    if node is None:
        return
    get_all_words_helper(node.left, prefix, results)
    if node.has_value:
        results.append((prefix + node.data, node.value))
    get_all_words_helper(node.middle, prefix + node.data, results)
    get_all_words_helper(node.right, prefix, results)


def get_words_with_prefix(root, prefix):
    results = []
    prefix_node = get_node(root, prefix)            # 1. 접두사의 마지막 문자 노드
    if prefix_node is None:
        return results
    if prefix_node.has_value:                       # 접두사 자체가 키인 경우
        results.append((prefix, prefix_node.value))
    get_all_words_helper(prefix_node.middle, prefix, results)   # 2. 가운데 아래만
    return results


def count_nodes(node):
    if node is None:
        return 0
    return 1 + count_nodes(node.left) + count_nodes(node.middle) + count_nodes(node.right)


def height(node):
    """루트에서 가장 먼 노드까지 거치는 노드 수."""
    if node is None:
        return 0
    return 1 + max(height(node.left), height(node.middle), height(node.right))


def print_tst(node, depth, tag):
    """자식을 왼쪽(L), 가운데(M), 오른쪽(R) 순서로 들여 써서 찍는다."""
    if node is None:
        return
    line = "  " * depth + tag + node.data
    if node.has_value:
        line += f" = {node.value}"
    print(line)
    print_tst(node.left, depth + 1, "L ")
    print_tst(node.middle, depth + 1, "M ")
    print_tst(node.right, depth + 1, "R ")


def build(words, values):
    root = None
    for w, v in zip(words, values):
        root = insert(root, w, v, 0)
    return root


def print_space(label, words):
    root = build(words, range(len(words)))
    nodes = count_nodes(root)
    print(f"  {label:<8}{len(words):>3}{nodes:>7}{nodes * 3:>7}"
          f"{nodes * 3 - (nodes - 1):>7}{height(root):>8}")


if __name__ == "__main__":
    words = ["apple", "application", "apply", "applaud", "applet", "appliance", "applicable"]
    values = [10, 20, 30, 40, 50, 60, 70]

    print("insert", " ".join(f"{w}={v}" for w, v in zip(words, values)))
    root = None
    for w, v in zip(words, values):
        if w == "appliance":
            print(f"insert {w}={v}")
            root = insert(root, w, v, 0, trace=True)
        else:
            root = insert(root, w, v, 0)

    print()
    print_tst(root, 0, "")

    print()
    print("search")
    for w in ["applaud", "apply", "application", "app", "apples"]:
        trace = []
        value = search(root, w, trace)
        shown = str(value) if value >= 0 else "none"
        print(f"  {w:<12}-> {shown:<6} visited {trace[0]}")

    print()
    for prefix in ["app", "appli", "apple", "b"]:
        found = get_words_with_prefix(root, prefix)
        shown = " ".join(f"{w}({v})" for w, v in found) if found else "(none)"
        print(f"getWordsWithPrefix({prefix}): {shown}")

    print()
    print("space (3 links per node)")
    print("  keys      N  nodes  links   null  height")
    print_space("apple..", words)
    print_space("sorted", sorted(words))
    print_space("by..the", ["by", "sea", "sells", "she", "shells", "the"])
