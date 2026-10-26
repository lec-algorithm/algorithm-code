"""트라이(trie) — 키를 문자 하나씩 경로로 저장하는 트리.

노드마다 a-z 26개의 링크를 두고, 키의 마지막 문자에 해당하는 노드에 값을
둔다. 삽입, 검색, 접두사로 모두 찾기, 삭제를 차례로 돌리고, 링크가 몇 개나
비어 있는지와 검색 실패가 노드 몇 개에서 끝나는지 센다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 trie.py`
"""
R = 26  # 알파벳 크기 (a-z)


class TrieNode:
    def __init__(self):
        self.children = [None] * R  # a-z 링크. 비어 있으면 None
        self.has_value = False      # 이 노드에서 끝나는 키가 있는가
        self.value = 0              # 그 키의 값


def create_trie_node():
    return TrieNode()


def insert(root, word, value):
    """word를 문자 하나씩 따라 내려가며, 링크가 없으면 새 노드를 만든다."""
    current = root
    for ch in word:
        index = ord(ch) - ord("a")  # 문자를 배열 인덱스로 (a=0, b=1, ...)
        if current.children[index] is None:
            current.children[index] = create_trie_node()
        current = current.children[index]
    current.has_value = True        # 키의 마지막 문자에 해당하는 노드에 값
    current.value = value


def search(root, word, trace=None):
    """word의 값, 없으면 -1. trace를 주면 거친 노드 수(루트 포함)를 담는다."""
    current = root
    visited = 1
    result = -1
    for ch in word:
        index = ord(ch) - ord("a")
        if current.children[index] is None:
            break                   # 링크가 없다: 실패
        current = current.children[index]
        visited += 1
    else:
        if current.has_value:
            result = current.value
        # 노드는 있지만 값이 없으면 실패
    if trace is not None:
        trace.append(visited)
    return result


def get_all_words_helper(node, prefix, results):
    """node 아래의 키를 모두 모은다. a부터 z 순서라 결과가 사전 순이다."""
    if node is None:
        return
    if node.has_value:
        results.append((prefix, node.value))
    for i in range(R):
        if node.children[i] is not None:
            get_all_words_helper(node.children[i], prefix + chr(ord("a") + i), results)


def get_words_with_prefix(root, prefix):
    """prefix로 시작하는 (키, 값)의 목록."""
    current = root
    for ch in prefix:               # 1. 접두사의 마지막 문자 노드로 간다
        index = ord(ch) - ord("a")
        if current.children[index] is None:
            return []
        current = current.children[index]
    results = []
    get_all_words_helper(current, prefix, results)   # 2. 그 아래 키를 모두 모은다
    return results


def has_children(node):
    return any(child is not None for child in node.children)


def delete_helper(node, word, index, removed):
    """node를 부모에서 떼어 내도 되면 True. 떼어 낸 노드의 문자를 removed에 담는다."""
    if index == len(word):          # 키의 마지막 문자 노드에 도착
        if not node.has_value:
            return False            # 없는 키
        node.has_value = False
        node.value = 0
        return not has_children(node)
    i = ord(word[index]) - ord("a")
    child = node.children[i]
    if child is None:
        return False                # 없는 키
    if delete_helper(child, word, index + 1, removed):
        node.children[i] = None     # 값도 자식도 없는 노드를 떼어 낸다
        removed.append(word[index])
        return not node.has_value and not has_children(node)
    return False


def delete_word(root, word, removed):
    delete_helper(root, word, 0, removed)   # 루트는 지우지 않는다


def count_nodes(node):
    """루트를 포함한 노드 수."""
    if node is None:
        return 0
    return 1 + sum(count_nodes(child) for child in node.children)


def print_trie(node, depth):
    """자식을 a부터 z 순서로, 깊이만큼 들여 써서 찍는다. 값이 있으면 = 값."""
    for i in range(R):
        child = node.children[i]
        if child is not None:
            line = "  " * depth + chr(ord("a") + i)
            if child.has_value:
                line += f" = {child.value}"
            print(line)
            print_trie(child, depth + 1)


def print_search(root, word):
    trace = []
    value = search(root, word, trace)
    visited = trace[0]
    path = " ".join(["root"] + list(word[:visited - 1]))
    shown = str(value) if value >= 0 else "none"
    if value >= 0:
        why = ""
    elif visited == len(word) + 1:
        why = " (no value)"
    else:
        why = f" (no link '{word[visited - 1]}')"
    print(f"  {word:<7}-> {shown:<6} visited {visited}: {path}{why}")


def print_space(label, words):
    root = create_trie_node()
    chars = 0
    for i, w in enumerate(words):
        insert(root, w, i)
        chars += len(w)
    nodes = count_nodes(root)
    n = len(words)
    print(f"  {label:<8}{n:>3}{chars / n:>7.2f}{nodes:>7}{nodes * R:>7}"
          f"{nodes * R - (nodes - 1):>7}{R * n:>7}{R * chars:>7}")


# 검색 실패 실험에 쓰는 난수. 두 구현이 같은 수열을 내도록 직접 만든다.
seed = 0


def next_random():
    global seed
    seed = (seed * 1103515245 + 12345) % 2147483648
    return seed >> 16


def random_key(length):
    return "".join(chr(ord("a") + next_random() % R) for _ in range(length))


def search_miss(n, length, queries):
    """무작위 키 n개로 트라이를 만들고, 무작위 키로 검색할 때 거치는 노드 수의 평균."""
    global seed
    seed = 2026
    root = create_trie_node()
    for i in range(n):
        insert(root, random_key(length), i)
    total = 0
    for _ in range(queries):
        trace = []
        search(root, random_key(length), trace)
        total += trace[0]
    print(f"  N = {n:<6} average visited = {total / queries:.2f}")


if __name__ == "__main__":
    words = ["by", "sea", "sells", "she", "shells", "the"]
    values = [4, 2, 1, 0, 3, 5]

    root = create_trie_node()
    print("insert", " ".join(f"{w}={v}" for w, v in zip(words, values)))
    for w, v in zip(words, values):
        insert(root, w, v)
    print_trie(root, 1)
    nodes = count_nodes(root)
    print(f"nodes = {nodes} (with root), links = {nodes} x {R} = {nodes * R},"
          f" non-null = {nodes - 1}")

    print()
    print("search")
    for w in ["shells", "shell", "she", "shore"]:
        print_search(root, w)

    print()
    print("insert shore=7")
    insert(root, "shore", 7)
    for prefix in ["sh", "se", "t", "x"]:
        found = get_words_with_prefix(root, prefix)
        shown = " ".join(f"{w}({v})" for w, v in found) if found else "(none)"
        print(f"getWordsWithPrefix({prefix}): {shown}")

    print()
    removed = []
    delete_word(root, "shells", removed)
    print("delete shells: removed", " ".join(removed))
    for w in ["shells", "she", "shore"]:
        print_search(root, w)
    print(f"nodes = {count_nodes(root)}")

    print()
    print(f"space (R = {R}, w = average key length)")
    print("  keys      N      w  nodes  links   null     RN    RNw")
    print_space("by..the", words)
    print_space("chain", ["s", "she", "sh", "shells", "shel", "shell"])
    print_space("spread", ["shells", "an", "bat", "the", "yell", "ear"])
    print_space("apple..", ["apple", "application", "apply", "applaud",
                            "applet", "appliance", "applicable"])

    print()
    print("search miss, random keys of length 7, 1000 searches")
    for n in [100, 1000, 10000]:
        search_miss(n, 7, 1000)
