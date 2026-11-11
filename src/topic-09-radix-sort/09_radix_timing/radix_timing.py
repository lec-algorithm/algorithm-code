"""기수 정렬 시간 재기 — LSD, MSD, 작은 구간을 삽입 정렬로 넘기는 MSD (모두 R = 256).

값은 컴퓨터마다 다르므로 비율을 본다. 표의 값은 밀리초다. Python은 C보다 훨씬 느리므로
100,000개까지만 잰다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 radix_timing.py`
"""

import time

RADIX = 256
DIGITS = 4
CUTOFF = 16
MIN_SECONDS = 0.2   # 이만큼 걸릴 때까지 되풀이해 평균을 낸다
MASK = 0xFFFFFFFF


def random_values(n):
    """시드가 고정된 무작위 값. 0 이상 2^32 미만이다."""
    s = 0x9e3779b9
    out = []
    for _ in range(n):
        s = (s + 0x6d2b79f5) & MASK
        x = ((s ^ (s >> 15)) * (1 | s)) & MASK
        x = ((x + ((x ^ (x >> 7)) * (61 | x))) & MASK) ^ x
        out.append(x ^ (x >> 14))
    return out


def lsd_sort(a):
    src = a
    dst = [0] * len(a)
    for shift in range(0, 8 * DIGITS, 8):
        count = [0] * RADIX
        for v in src:
            count[(v >> shift) & 255] += 1
        for d in range(1, RADIX):
            count[d] += count[d - 1]
        for v in reversed(src):
            d = (v >> shift) & 255
            count[d] -= 1
            dst[count[d]] = v
        src, dst = dst, src
    # 패스가 짝수 번이라 결과가 a로 돌아와 있다.


def msd_recursive(a, buffer, lo, hi, shift, cutoff):
    if lo >= hi or shift < 0:
        return
    if hi - lo + 1 <= cutoff:
        for i in range(lo + 1, hi + 1):
            v = a[i]
            j = i - 1
            while j >= lo and a[j] > v:
                a[j + 1] = a[j]
                j -= 1
            a[j + 1] = v
        return

    count = [0] * RADIX
    for i in range(lo, hi + 1):
        count[(a[i] >> shift) & 255] += 1
    pos = [0] * RADIX
    start = lo
    for d in range(RADIX):
        pos[d] = start
        start += count[d]
    for i in range(lo, hi + 1):
        d = (a[i] >> shift) & 255
        buffer[pos[d]] = a[i]
        pos[d] += 1
    a[lo:hi + 1] = buffer[lo:hi + 1]

    start = lo
    for d in range(RADIX):
        if count[d] > 1:
            msd_recursive(a, buffer, start, start + count[d] - 1, shift - 8, cutoff)
        start += count[d]


def msd_sort(a):
    msd_recursive(a, [0] * len(a), 0, len(a) - 1, 8 * (DIGITS - 1), 0)


def msd_cutoff_sort(a):
    msd_recursive(a, [0] * len(a), 0, len(a) - 1, 8 * (DIGITS - 1), CUTOFF)


def measure(sort, values):
    """한 번 정렬하는 데 걸린 평균 시간(ms). 짧으면 여러 번 돌린다."""
    spent = 0.0
    runs = 0
    while True:
        work = values[:]
        t0 = time.perf_counter()
        sort(work)
        spent += time.perf_counter() - t0
        runs += 1
        if work != sorted(values):
            raise SystemExit("not sorted")
        if spent >= MIN_SECONDS:
            return spent / runs * 1000


if __name__ == "__main__":
    print(f"{'n':>9}  {'LSD':>10}  {'MSD':>10}  {'MSD <=16':>12}")
    for n in (1000, 10000, 100000):
        values = random_values(n)
        print(f"{n:9d}  {measure(lsd_sort, values):10.3f}  {measure(msd_sort, values):10.3f}"
              f"  {measure(msd_cutoff_sort, values):12.3f}")
