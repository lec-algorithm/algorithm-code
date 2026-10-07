"""허프만 코드 — 가장 드문 두 트리부터 합쳐 접두사 없는 코드를 만든다.

흔한 문자는 짧은 코드, 드문 문자는 긴 코드를 받는다. 만든 코드로
인코딩하고, 트리를 따라 다시 디코딩해 원문이 돌아오는지 본다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 huffman_code.py`
"""


class Node:
    def __init__(self, weight, min_sym, left=None, right=None, label=""):
        self.weight = weight
        self.min_sym = min_sym  # 트리 안의 가장 작은 문자. 가중치가 같을 때 순서
        self.left = left        # 잎이면 None
        self.right = right
        self.sym = min_sym      # 잎의 문자
        self.label = label      # 출력용. 잎들의 문자를 왼쪽부터 이어 붙인 것


def pop_min(forest):
    """forest에서 가장 먼저 꺼낼 트리를 빼서 돌려준다. 가중치, 같으면 가장 작은 문자."""
    best = min(range(len(forest)), key=lambda i: (forest[i].weight, forest[i].min_sym))
    return forest.pop(best)


def huffman(freq):
    """freq로 허프만 트리를 만들고 루트를 돌려준다. 합칠 때마다 한 줄씩 찍는다."""
    forest = [Node(w, c, label=c) for c, w in sorted(freq.items())]

    step = 1
    while len(forest) > 1:
        x = pop_min(forest)
        y = pop_min(forest)
        z = Node(x.weight + y.weight, min(x.min_sym, y.min_sym), x, y, x.label + y.label)
        print(f"merge {step}: {x.label}({x.weight}) + {y.label}({y.weight})"
              f" -> {z.label}({z.weight})")
        forest.append(z)
        step += 1
    return forest[0]


def assign_codes(node, path="", codes=None):
    """루트에서 각 잎까지의 경로를 코드로 적는다. 왼쪽 0, 오른쪽 1."""
    if codes is None:
        codes = {}
    if node.left is None:
        codes[node.sym] = path or "0"
        return codes
    assign_codes(node.left, path + "0", codes)
    assign_codes(node.right, path + "1", codes)
    return codes


def encode(text, codes):
    return "".join(codes[ch] for ch in text)


def decode(bits, root):
    out = []
    node = root
    for b in bits:
        node = node.left if b == "0" else node.right
        if node.left is None:
            out.append(node.sym)
            node = root
    return "".join(out)


if __name__ == "__main__":
    text = "ABRACADABRA!"
    freq = {}
    for ch in text:
        freq[ch] = freq.get(ch, 0) + 1

    print(f"text = {text}")
    print("freq :", *(f"{c}={w}" for c, w in sorted(freq.items())))
    print()

    root = huffman(freq)
    codes = assign_codes(root)

    print()
    print("codes:", *(f"{c}={codes[c]}" for c in sorted(codes)))

    bits = encode(text, codes)
    decoded = decode(bits, root)

    fixed = 0
    while (1 << fixed) < len(freq):
        fixed += 1
    print(f"encoded = {bits}")
    print(f"bits: huffman {len(bits)}, fixed {fixed}-bit {fixed * len(text)},"
          f" ascii {8 * len(text)}")
    print(f"decoded = {decoded}")
