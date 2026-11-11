"""LSD와 MSD의 비용 세기 — 자리 읽기, 호출, 칸 훑기(호출 x k), 삽입 비교.

기수 k와, 삽입 정렬로 넘기는 구간 크기(cutoff)를 바꾸며 센다. 문자열 MSD는 읽은 글자를 센다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 msd_cost.py`
"""

MASK = 0xFFFFFFFF


class Cost:
    def __init__(self, reads=0, calls=0, scans=0):
        self.reads = reads
        self.calls = calls
        self.scans = scans
        self.ins = 0
        self.by_level = []   # 자리별 호출 수

    def total(self):
        return self.reads + self.scans + self.ins


def random_values(n):
    """시드가 고정된 무작위 값. 0 이상 2^32 미만이고 C 구현과 같은 값이 나온다."""
    s = 0x2f6e2b1
    out = []
    for _ in range(n):
        s = (s + 0x6d2b79f5) & MASK
        x = ((s ^ (s >> 15)) * (1 | s)) & MASK
        x = ((x + ((x ^ (x >> 7)) * (61 | x))) & MASK) ^ x
        out.append(x ^ (x >> 14))
    return out


def width_of(max_value, k):
    d = 1
    e = k
    while max_value >= e:
        e *= k
        d += 1
    return d


def insertion_sort(a, lo, hi, c):
    for i in range(lo + 1, hi + 1):
        v = a[i]
        j = i - 1
        while j >= lo:
            c.ins += 1
            if a[j] <= v:
                break
            a[j + 1] = a[j]
            j -= 1
        a[j + 1] = v


def msd_recursive(a, lo, hi, exp, k, cutoff, level, c):
    if lo >= hi or exp == 0:
        return
    if hi - lo + 1 <= cutoff:
        insertion_sort(a, lo, hi, c)
        return

    c.calls += 1
    if level == len(c.by_level):
        c.by_level.append(0)
    c.by_level[level] += 1
    c.scans += k

    size = [0] * k
    for i in range(lo, hi + 1):
        size[(a[i] // exp) % k] += 1
        c.reads += 1
    pos = size[:]
    for d in range(1, k):
        pos[d] += pos[d - 1]
    output = [0] * (hi - lo + 1)
    for i in range(hi, lo - 1, -1):
        d = (a[i] // exp) % k
        pos[d] -= 1
        output[pos[d]] = a[i]
    a[lo:hi + 1] = output

    start = lo
    for d in range(k):
        if size[d] > 1:
            msd_recursive(a, start, start + size[d] - 1, exp // k, k, cutoff, level + 1, c)
        start += size[d]


def row(name, c):
    return (f"  {name:<8} reads {c.reads:5d}  calls {c.calls:3d}  scans {c.scans:6d}"
            f"  ins {c.ins:5d}  total {c.total():6d}")


def print_digits(a, k):
    """값들을 기수 k의 자리로 보여 준다. 가장 큰 값의 자릿수에 맞춘다."""
    width = width_of(max(a), k)
    parts = []
    for v in a:
        digits = [(v // k ** p) % k for p in range(width - 1, -1, -1)]
        parts.append("(" + ", ".join(map(str, digits)) + ")")
    print(f"    k = {k:<3d} {width} digit{' ' if width == 1 else 's'}  " + " ".join(parts))


def compare(values, k, cutoff):
    width = width_of(max(values), k)
    lsd = Cost(reads=len(values) * width, calls=width, scans=width * k)
    msd = Cost()
    a = values[:]
    msd_recursive(a, 0, len(a) - 1, k ** (width - 1), k, cutoff, 0, msd)

    print(row("LSD", lsd))
    name = f"MSD <={cutoff}" if cutoff > 1 else "MSD"
    print(row(name, msd) + "  [" + " + ".join(map(str, msd.by_level)) + "]")


def msd_words(words, lo, hi, pos, tally):
    """문자열 MSD. 끝난 단어는 어떤 글자보다 앞에 두고, 둘 이상인 글자 버킷만 다음 글자로 간다."""
    tally["calls"] += 1
    keys = []
    for i in range(lo, hi + 1):
        if pos < len(words[i]):
            keys.append(ord(words[i][pos]) + 1)
            tally["read"] += 1
        else:
            keys.append(0)
    order = sorted(range(hi - lo + 1), key=lambda i: keys[i])
    words[lo:hi + 1] = [words[lo + i] for i in order]

    start = lo
    for d in sorted(set(keys)):
        size = keys.count(d)
        if d > 0 and size > 1:
            msd_words(words, start, start + size - 1, pos + 1, tally)
        start += size


def count_words(words):
    tally = {"read": 0, "calls": 0}
    msd_words(words[:], 0, len(words) - 1, 0, tally)
    total = sum(len(w) for w in words)
    print(f"  {' '.join(words)}: read {tally['read']} of {total}, calls {tally['calls']}")


if __name__ == "__main__":
    inputs = [
        [170, 45, 75, 90, 2, 802, 24, 66],
        [111, 112, 113, 114],
        [100, 200, 300, 400],
        [100, 300, 600, 900],
        [555, 555, 555, 555],
    ]

    print("digits")
    for a in inputs:
        print("  " + " ".join(map(str, a)))
        for k in (10, 256):
            print_digits(a, k)

    print("\ncost")
    for a in inputs:
        for k in (10, 256):
            print(" ".join(map(str, a)) + f", k = {k}")
            compare(a, k, 0)

    values = random_values(1000)
    print(f"\n1000 random values, {min(values)} to {max(values)}")
    for k, cutoff in ((10, 0), (10, 9), (10, 32), (256, 0), (256, 9), (256, 16)):
        print(f"k = {k}")
        compare(values, k, cutoff)

    print("\nstrings")
    count_words(["she", "shells", "sea", "by", "the", "shore"])
    count_words(["ant", "bee", "cat", "dog", "elk", "fox"])
    count_words(["sea", "sea", "sea", "sea"])
    count_words(["car", "card", "care", "cat"])
