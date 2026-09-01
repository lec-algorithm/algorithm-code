"""병합 정렬 — 반으로 나눠 각각 정렬하고, 정렬된 두 절반을 합친다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 merge_sort.py`
"""


def merge_sort(a, lo=None, hi=None, temp=None, counts=None):
    """a[lo..hi]를 제자리에서 오름차순으로 정렬하고
    (비교 횟수, 이동 횟수)를 돌려준다."""
    if lo is None:
        lo, hi = 0, len(a) - 1
        temp = [0] * len(a)
        counts = [0, 0]  # [비교, 이동]
    if lo >= hi:
        return tuple(counts)
    mid = (lo + hi) // 2
    merge_sort(a, lo, mid, temp, counts)
    merge_sort(a, mid + 1, hi, temp, counts)
    _merge(a, lo, mid, hi, temp, counts)
    return tuple(counts)


def _merge(a, lo, mid, hi, temp, counts):
    """정렬된 a[lo..mid]와 a[mid+1..hi]를 하나로 합친다.

    temp로 한 번, 다시 a로 한 번 가므로 원소마다 이동 2회다.
    """
    i, j, k = lo, mid + 1, lo
    while i <= mid and j <= hi:
        counts[0] += 1
        if a[i] <= a[j]:  # <=라서 같은 값이면 왼쪽이 먼저: 안정
            temp[k] = a[i]
            i += 1
        else:
            temp[k] = a[j]
            j += 1
        counts[1] += 1
        k += 1
    while i <= mid:
        temp[k] = a[i]
        i += 1
        k += 1
        counts[1] += 1
    while j <= hi:
        temp[k] = a[j]
        j += 1
        k += 1
        counts[1] += 1
    for k in range(lo, hi + 1):
        a[k] = temp[k]
        counts[1] += 1


if __name__ == "__main__":
    a = [2, 8, 5, 9, 1, 10, 7, 6, 4, 3]

    print("before:", *a)
    comparisons, moves = merge_sort(a)
    print("after :", *a)
    print(f"comparisons = {comparisons}, moves = {moves}")
