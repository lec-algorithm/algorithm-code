"""퀵 정렬 — 피벗을 제자리에 놓고, 양쪽을 각각 정렬한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 quick_sort.py`
"""


def quick_sort(a, lo=None, hi=None, counts=None):
    """a[lo..hi]를 제자리에서 오름차순으로 정렬하고
    (비교 횟수, 이동 횟수)를 돌려준다."""
    if lo is None:
        lo, hi = 0, len(a) - 1
        counts = [0, 0]  # [비교, 이동]
    if lo >= hi:
        return tuple(counts)
    p = _partition(a, lo, hi, counts)
    quick_sort(a, lo, p - 1, counts)
    quick_sort(a, p + 1, hi, counts)
    return tuple(counts)


def _swap(a, i, j, counts):
    if i != j:
        a[i], a[j] = a[j], a[i]
        counts[1] += 3


def _partition(a, lo, hi, counts):
    """a[lo]를 피벗으로 삼아 작은 것은 왼쪽, 나머지는 오른쪽으로 보내고
    피벗을 그 경계에 놓는다. 피벗의 최종 위치를 돌려준다."""
    pivot = a[lo]
    i = lo
    for j in range(lo + 1, hi + 1):
        counts[0] += 1
        if a[j] < pivot:
            i += 1
            _swap(a, i, j, counts)
    _swap(a, lo, i, counts)
    return i


if __name__ == "__main__":
    a = [2, 8, 5, 9, 1, 10, 7, 6, 4, 3]

    print("before:", *a)
    comparisons, moves = quick_sort(a)
    print("after :", *a)
    print(f"comparisons = {comparisons}, moves = {moves}")
