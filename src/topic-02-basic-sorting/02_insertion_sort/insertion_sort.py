"""삽입 정렬 — 이미 정렬된 앞부분에 새 원소를 끼워 넣는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 insertion_sort.py`
"""


def insertion_sort(a):
    """a를 제자리에서 오름차순으로 정렬하고 (비교 횟수, 이동 횟수)를 돌려준다.

    key를 꺼낼 때 1회, 한 칸 밀 때마다 1회, 끼워 넣을 때 1회로 센다.
    """
    comparisons = 0
    moves = 0
    for i in range(1, len(a)):
        key = a[i]
        moves += 1
        j = i - 1
        while j >= 0:
            comparisons += 1
            if a[j] <= key:
                break
            a[j + 1] = a[j]
            moves += 1
            j -= 1
        a[j + 1] = key
        moves += 1
    return comparisons, moves


if __name__ == "__main__":
    a = [6, 8, 5, 9, 10, 1, 7, 2, 4, 3]

    print("before:", *a)
    comparisons, moves = insertion_sort(a)
    print("after :", *a)
    print(f"comparisons = {comparisons}, moves = {moves}")
