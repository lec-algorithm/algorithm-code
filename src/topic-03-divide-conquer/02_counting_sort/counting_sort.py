"""카운팅 정렬 — 값을 비교하지 않고, 값마다 몇 개 있는지 세서 자리를 계산한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 counting_sort.py`
"""

MAX_KEY = 5  # 값의 범위: 0 이상 MAX_KEY 이하


def counting_sort(a):
    """정렬된 새 리스트와 (비교 횟수, 이동 횟수)를 돌려준다.

    원소끼리 비교하는 일이 없으므로 비교 횟수는 항상 0이다.
    """
    count = [0] * (MAX_KEY + 1)
    for value in a:                       # 1) 세기
        count[value] += 1
    for v in range(1, MAX_KEY + 1):       # 2) 누적
        count[v] += count[v - 1]
    b = [0] * len(a)
    moves = 0
    for value in reversed(a):             # 3) 뒤에서부터 배치: 안정
        count[value] -= 1
        b[count[value]] = value
        moves += 1
    return b, 0, moves


if __name__ == "__main__":
    a = [1, 0, 1, 5, 4, 3, 1, 4, 5, 2, 1]

    print("before:", *a)
    b, comparisons, moves = counting_sort(a)
    print("after :", *b)
    print(f"comparisons = {comparisons}, moves = {moves}")
