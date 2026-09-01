"""버블 정렬 — 이웃끼리 비교해서 큰 것을 뒤로 보낸다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 bubble_sort.py`
"""


def bubble_sort(a):
    """a를 제자리에서 오름차순으로 정렬하고 (비교 횟수, 이동 횟수)를 돌려준다.

    교환 한 번은 temp를 거치므로 이동 3회로 센다.
    """
    comparisons = 0
    moves = 0
    n = len(a)
    for i in range(n - 1):
        for j in range(n - 1 - i):
            comparisons += 1
            if a[j] > a[j + 1]:
                a[j], a[j + 1] = a[j + 1], a[j]
                moves += 3
    return comparisons, moves


if __name__ == "__main__":
    a = [6, 8, 5, 9, 10, 1, 7, 2, 4, 3]

    print("before:", *a)
    comparisons, moves = bubble_sort(a)
    print("after :", *a)
    print(f"comparisons = {comparisons}, moves = {moves}")
