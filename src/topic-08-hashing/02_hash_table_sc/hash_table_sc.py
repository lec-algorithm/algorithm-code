"""개별 체이닝 해시 테이블 — 배열의 칸마다 연결 리스트를 둔다.

해시로 칸을 찾고(1단계), 그 칸의 리스트를 순차 탐색한다(2단계).
새 키는 리스트 맨 앞에 붙이고, 이미 있는 키는 값만 바꾼다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 hash_table_sc.py`
"""

# 이 예제의 키는 대문자 한 글자다. 글자마다 정수 해시 코드를 정해 두었다.
# 표에 없는 글자는 ASCII 코드를 그대로 쓴다.
CODES = {"A": 20, "C": 69, "E": 10, "H": 4, "L": 38, "M": 49,
         "P": 78, "R": 14, "S": 22, "X": 47, "Y": 52}


def hash_code(key):
    return CODES.get(key, ord(key))


def hash_of(key, m):
    return hash_code(key) % m


class Node:
    def __init__(self, key, value, next_node):
        self.key = key
        self.value = value
        self.next = next_node


class ChainingTable:
    def __init__(self, m):
        self.m = m                 # 배열 크기
        self.n = 0                 # 저장된 키 수
        self.heads = [None] * m    # heads[i]: i번 칸 리스트의 맨 앞 노드


def put(t, key, value):
    """key에 value를 넣는다. (새 키인지, 비교 횟수)를 돌려준다."""
    i = hash_of(key, t.m)
    compares = 0
    x = t.heads[i]
    while x is not None:
        compares += 1
        if x.key == key:
            x.value = value
            return False, compares
        x = x.next
    t.heads[i] = Node(key, value, t.heads[i])  # 맨 앞에 붙인다
    t.n += 1
    return True, compares


def get(t, key):
    """(값 또는 None, 비교 횟수)를 돌려준다."""
    i = hash_of(key, t.m)
    compares = 0
    x = t.heads[i]
    while x is not None:
        compares += 1
        if x.key == key:
            return x.value, compares
        x = x.next
    return None, compares


def delete(t, key):
    """(지웠는지, 비교 횟수)를 돌려준다."""
    i = hash_of(key, t.m)
    compares = 0
    prev = None
    x = t.heads[i]
    while x is not None:
        compares += 1
        if x.key == key:
            if prev is None:
                t.heads[i] = x.next   # 맨 앞 노드
            else:
                prev.next = x.next    # 중간이나 끝 노드
            t.n -= 1
            return True, compares
        prev = x
        x = x.next
    return False, compares


def chain_text(t, i):
    parts = []
    x = t.heads[i]
    while x is not None:
        parts.append(f"{x.key}{x.value}")
        x = x.next
    return f"[{i}] " + (" -> ".join(parts) if parts else "-")


def print_table(t):
    for i in range(t.m):
        print(chain_text(t, i))


if __name__ == "__main__":
    keys = "SEARCHEXAMPLE"
    t = ChainingTable(5)

    print(f"M = {t.m}, keys", *keys)
    print()

    for i, key in enumerate(keys):  # 값은 등장 순서 0, 1, 2, ...
        is_new, compares = put(t, key, i)
        action = "new" if is_new else "update"
        h = hash_of(key, t.m)
        print(f"put {key}={i:<2d}  hash {h}  compares {compares}  {action:<6s}  "
              + chain_text(t, h))

    print(f"\nN = {t.n}, M = {t.m}, N/M = {t.n / t.m:.1f}")
    print_table(t)

    print()
    for key in ["E", "Y"]:
        value, compares = get(t, key)
        result = "not found" if value is None else str(value)
        print(f"get {key}: hash {hash_of(key, t.m)}, compares {compares} -> {result}")

    deleted, compares = delete(t, "E")
    print(f"\ndelete E: hash {hash_of('E', t.m)}, compares {compares} -> "
          + ("deleted" if deleted else "not found"))
    print(f"N = {t.n}")
    print_table(t)
