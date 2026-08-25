"""순차 검색 — 앞에서부터 하나씩 비교한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 sequential_search.py`
"""


def sequential_search(a, key):
    """key를 찾으면 (인덱스, 비교 횟수)를, 없으면 (-1, 비교 횟수)를 돌려준다."""
    comparisons = 0
    for i, value in enumerate(a):
        comparisons += 1
        if value == key:
            return i, comparisons
    return -1, comparisons


if __name__ == "__main__":
    a = [6, 13, 14, 25, 33, 43, 51, 53, 64, 72, 84, 93, 95, 96, 97]
    key = 51

    index, comparisons = sequential_search(a, key)

    print(f"n = {len(a)}, key = {key}")
    print(f"index = {index}, comparisons = {comparisons}")
