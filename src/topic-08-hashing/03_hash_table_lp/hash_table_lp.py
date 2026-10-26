"""선형 탐지 해시 테이블 — 충돌하면 배열의 다음 칸으로 간다.

리스트 없이 배열 하나에 키를 직접 넣는다(개방형 주소 지정). 해시가 가리킨
칸이 차 있으면 빈칸이 나올 때까지 오른쪽으로 한 칸씩 간다. 지울 때는 같은
무리에서 뒤따르던 키들을 꺼냈다가 다시 넣는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 hash_table_lp.py`
"""

# 02 예제와 같은 해시 코드. 원본 강의 그림의 해시값(M = 16)이 이 코드를
# M으로 나눈 나머지와 같다. 표에 없는 글자는 ASCII 코드를 그대로 쓴다.
CODES = {"A": 20, "C": 69, "E": 10, "H": 4, "L": 38, "M": 49,
         "P": 78, "R": 14, "S": 22, "X": 47, "Y": 52}


def hash_code(key):
    return CODES.get(key, ord(key))


def hash_of(key, m):
    return hash_code(key) % m


class ProbingTable:
    def __init__(self, m):
        self.m = m                # 배열 크기
        self.n = 0                # 저장된 키 수
        self.keys = [None] * m    # None이면 빈칸
        self.values = [None] * m


def copy_table(t):
    c = ProbingTable(t.m)
    c.n = t.n
    c.keys = t.keys[:]
    c.values = t.values[:]
    return c


def put(t, key, value):
    """key에 value를 넣는다. (새 키인지, 들여다본 칸 수, 놓인 칸)을 돌려준다."""
    if t.n * 10 > t.m * 7:  # 적재율 N/M이 0.7을 넘으면 두 배로
        resize(t, 2 * t.m)
    i = hash_of(key, t.m)
    probes = 1
    while t.keys[i] is not None:
        if t.keys[i] == key:
            t.values[i] = value
            return False, probes, i
        i = (i + 1) % t.m  # 끝에 닿으면 0번 칸으로
        probes += 1
    t.keys[i] = key
    t.values[i] = value
    t.n += 1
    return True, probes, i


def get(t, key):
    """(값 또는 None, 들여다본 칸 수)를 돌려준다. 빈칸을 만나면 없는 것이다."""
    i = hash_of(key, t.m)
    probes = 1
    while t.keys[i] is not None:
        if t.keys[i] == key:
            return t.values[i], probes
        i = (i + 1) % t.m
        probes += 1
    return None, probes


def resize(t, new_m):
    """t의 키를 0번 칸부터 차례로 크기 new_m인 새 배열에 다시 넣는다."""
    bigger = ProbingTable(new_m)
    for i in range(t.m):
        if t.keys[i] is not None:
            put(bigger, t.keys[i], t.values[i])
    t.m, t.n, t.keys, t.values = bigger.m, bigger.n, bigger.keys, bigger.values


def delete(t, key):
    """(지웠는지, 들여다본 칸 수)를 돌려준다. 다시 넣을 때마다 한 줄씩 찍는다."""
    i = hash_of(key, t.m)
    probes = 1
    while t.keys[i] != key:
        if t.keys[i] is None:
            return False, probes
        i = (i + 1) % t.m
        probes += 1
    t.keys[i] = None
    t.n -= 1

    i = (i + 1) % t.m
    while t.keys[i] is not None:  # 같은 무리에서 뒤따르던 키들
        k, v = t.keys[i], t.values[i]
        t.keys[i] = None
        t.n -= 1
        _, _, slot = put(t, k, v)
        print(f"  reinsert {k}: slot {i} -> {slot}")
        i = (i + 1) % t.m
    if 0 < t.n <= t.m // 8:  # 8분의 1 이하로 비면 절반으로
        resize(t, t.m // 2)
    return True, probes


def total_hit_probes(t):
    """저장된 키를 모두 한 번씩 찾을 때 들여다보는 칸 수의 합"""
    return sum(get(t, k)[1] for k in t.keys if k is not None)


def print_table(t):
    for start in range(0, t.m, 16):
        cols = range(start, min(start + 16, t.m))
        print("slot " + "".join(f"{i:3d}" for i in cols))
        print("key  " + "".join(f"{t.keys[i] or '-':>3}" for i in cols))
        print("value" + "".join(f"{'-' if t.keys[i] is None else t.values[i]:>3}"
                                for i in cols))


def print_get(t, key):
    value, probes = get(t, key)
    result = "not found" if value is None else str(value)
    print(f"get {key}: hash {hash_of(key, t.m)}, probes {probes} -> {result}")


def print_hits(t):
    total = total_hit_probes(t)
    print(f"hits: {total} probes for {t.n} keys, {total / t.n:.2f} per key")


if __name__ == "__main__":
    keys = "SEARCHEXAMPLE"
    t = ProbingTable(16)

    print(f"M = {t.m}, keys", *keys)
    print()

    for i, key in enumerate(keys):  # 값은 등장 순서 0, 1, 2, ...
        is_new, probes, slot = put(t, key, i)
        print(f"put {key}={i:<2d}  hash {hash_of(key, t.m):2d}  probes {probes}  "
              f"slot {slot:2d}  {'new' if is_new else 'update'}")

    print(f"\nN = {t.n}, M = {t.m}, N/M = {t.n / t.m:.4f}")
    print_table(t)

    print()
    print_get(t, "H")
    print_get(t, "Y")
    print_hits(t)

    # 적재율을 낮추면: 같은 키를 크기 32인 배열에 다시 넣는다
    wide = copy_table(t)
    resize(wide, 32)
    print(f"\nresize to M = {wide.m}, N/M = {wide.n / wide.m:.4f}")
    print_table(wide)
    print_hits(wide)

    # 잘못된 삭제: C의 칸만 비운다
    naive = copy_table(t)
    naive.keys[5] = None
    naive.n -= 1
    print("\nnaive delete C: just empty slot 5")
    print_get(naive, "H")

    print(f"\ndelete C: hash {hash_of('C', t.m)}")
    delete(t, "C")
    print_get(t, "H")
    print(f"N = {t.n}, M = {t.m}")
    print_table(t)
