"""삽입 정렬 vs 셸 정렬 — 배열이 커질 때 걸리는 시간을 잰다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 performance_comparison.py`
같은 무작위 배열을 두 정렬에 주고 시간을 비교한다. 측정값은 기계마다 다르다.
"""

import random
import time

from shell_sort import shell_sort


def insertion_sort(a):
    """비교용 삽입 정렬. 02_insertion_sort의 구현과 같다."""
    for i in range(1, len(a)):
        key = a[i]
        j = i - 1
        while j >= 0 and a[j] > key:
            a[j + 1] = a[j]
            j -= 1
        a[j + 1] = key


def measure(sort, a):
    """정렬 한 번에 걸린 초를 돌려준다."""
    start = time.perf_counter()
    sort(a)
    return time.perf_counter() - start


if __name__ == "__main__":
    random.seed(2026)  # 매번 같은 배열이 나오도록 고정

    print(f"{'n':>7} {'삽입 정렬(s)':>12} {'셸 정렬(s)':>12}")
    for n in (20, 40, 1000, 10000):
        a = [random.randrange(n * 10) for _ in range(n)]
        t_ins = measure(insertion_sort, list(a))
        t_shell = measure(shell_sort, list(a))
        print(f"{n:>7} {t_ins:>14.6f} {t_shell:>13.6f}")
