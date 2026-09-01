"""퀵 선택 — 다 정렬하지 않고 k번째로 작은 값을 찾는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 quick_selection.py`
"""


def quick_selection(a, k):
    """k번째로 작은 값과 (비교 횟수, 이동 횟수)를 돌려준다 (k는 1부터).

    퀵 정렬과 같은 파티션을 쓰되, 한쪽만 파고든다.
    """
    counts = [0, 0]  # [비교, 이동]
    lo, hi = 0, len(a) - 1
    while True:
        p = _partition(a, lo, hi, counts)
        if p == k - 1:
            return a[p], counts[0], counts[1]
        if p > k - 1:
            hi = p - 1
        else:
            lo = p + 1


def _swap(a, i, j, counts):
    if i != j:
        a[i], a[j] = a[j], a[i]
        counts[1] += 3


def _partition(a, lo, hi, counts):
    """퀵 정렬과 같은 파티션이다. 피벗의 최종 위치를 돌려준다."""
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
    k = 5

    print("before:", *a)
    value, comparisons, moves = quick_selection(a, k)
    print("after :", *a)
    print(f"k = {k} -> value = {value}")
    print(f"comparisons = {comparisons}, moves = {moves}")
