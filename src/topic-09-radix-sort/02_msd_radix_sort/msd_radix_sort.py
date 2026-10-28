"""MSD 기수 정렬 — 가장 높은 자리부터 버킷으로 나누고, 버킷마다 다음 자리로 재귀한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 msd_radix_sort.py`
"""

RADIX = 10
WIDTH = 3      # 출력할 때 맞출 자릿수

examined = 0   # 자리를 읽은 횟수: 카운팅 정렬이 원소 하나를 셀 때마다 1


def fmt(values):
    return " ".join(f"{v:0{WIDTH}d}" for v in values)


def counting_sort_by_digit(a, lo, hi, exp):
    """a[lo..hi]를 exp 자리로 안정 카운팅 정렬하고, 숫자마다 몇 개였는지 돌려준다."""
    global examined
    count = [0] * RADIX
    for i in range(lo, hi + 1):                 # 1) 세기
        count[(a[i] // exp) % RADIX] += 1
        examined += 1
    size = count[:]
    for d in range(1, RADIX):                   # 2) 누적
        count[d] += count[d - 1]
    output = [0] * (hi - lo + 1)
    for i in range(hi, lo - 1, -1):             # 3) 뒤에서부터 배치: 안정
        d = (a[i] // exp) % RADIX
        count[d] -= 1
        output[count[d]] = a[i]
    a[lo:hi + 1] = output
    return size


def msd_recursive(a, lo, hi, exp, depth):
    """a[lo..hi]를 exp 자리부터 정렬한다. depth는 들여쓰기용 재귀 깊이다."""
    if lo >= hi or exp == 0:
        return

    size = counting_sort_by_digit(a, lo, hi, exp)

    pad = "  " * depth
    print(f"{pad}exp {exp:<3d} [{lo}..{hi}]: {fmt(a[lo:hi + 1])}")
    print(f"{pad}  buckets:", *(f"{d}={size[d]}" for d in range(RADIX) if size[d] > 0))

    start = lo
    for d in range(RADIX):
        if size[d] > 1:
            msd_recursive(a, start, start + size[d] - 1, exp // RADIX, depth + 1)
        start += size[d]


def msd_radix_sort(a):
    max_value = max(a)
    exp = 1
    while max_value // exp >= RADIX:
        exp *= RADIX
    msd_recursive(a, 0, len(a) - 1, exp, 0)


if __name__ == "__main__":
    a = [170, 45, 75, 90, 2, 802, 24, 66]
    n = len(a)

    print(f"input : {fmt(a)}")
    msd_radix_sort(a)
    print(f"sorted: {fmt(a)}")
    print(f"digits examined = {examined} (LSD: {n} x {WIDTH} = {n * WIDTH})")
