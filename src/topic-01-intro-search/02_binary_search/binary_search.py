"""이진 검색 — 정렬된 배열에서 범위를 절반씩 줄여 간다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 binary_search.py`
"""


def binary_search(a, key):
    """key를 찾으면 (인덱스, 비교 횟수)를, 없으면 (-1, 비교 횟수)를 돌려준다.

    a는 반드시 오름차순으로 정렬되어 있어야 한다.
    """
    lo, hi = 0, len(a) - 1
    comparisons = 0

    while lo <= hi:
        mid = lo + (hi - lo) // 2
        comparisons += 1

        if a[mid] == key:
            return mid, comparisons
        elif a[mid] < key:
            lo = mid + 1  # 오른쪽 절반만 남긴다
        else:
            hi = mid - 1  # 왼쪽 절반만 남긴다

    return -1, comparisons


if __name__ == "__main__":
    a = [6, 13, 14, 25, 33, 43, 51, 53, 64, 72, 84, 93, 95, 96, 101]
    key = 51

    index, comparisons = binary_search(a, key)

    print(f"n = {len(a)}, key = {key}")
    print(f"index = {index}, comparisons = {comparisons}")
