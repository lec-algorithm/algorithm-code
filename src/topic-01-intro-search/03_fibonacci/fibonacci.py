"""피보나치 — 같은 수열을 재귀와 반복으로.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 fibonacci.py`
"""

import time

# 호출 횟수를 세어 재귀가 얼마나 많이 갈라지는지 본다.
calls = 0


def fibonacci_recursive(n):
    """정의를 그대로 옮긴 재귀. O(2^n)"""
    global calls
    calls += 1
    if n <= 1:
        return n
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2)


def fibonacci_iterative(n):
    """앞의 두 항만 들고 나아가는 반복. O(n)"""
    if n <= 1:
        return n
    prev, curr = 0, 1
    for _ in range(2, n + 1):
        prev, curr = curr, prev + curr
    return curr


def time_it(fn, n):
    """fn(n)을 재고 (결과, 걸린 시간 ms)를 돌려준다."""
    t0 = time.perf_counter()
    value = fn(n)
    return value, (time.perf_counter() - t0) * 1000


if __name__ == "__main__":
    print(" n   재귀 호출 수        재귀(ms)   반복(ms)   F(n)")
    print("--- --------------- ---------- ---------- ----------")

    # n=40이면 재귀가 12초쯤 걸린다. 여기서 멈추는 것도 관찰의 일부다.
    for n in (10, 20, 30, 35):
        calls = 0
        rec, t_rec = time_it(fibonacci_recursive, n)
        itr, t_itr = time_it(fibonacci_iterative, n)

        print(f"{n:3d} {calls:15,d} {t_rec:10.2f} {t_itr:10.4f} {itr:10d}")

        if rec != itr:  # 두 구현은 같은 답을 내야 한다
            print(f"  !! 값이 다르다: {rec} vs {itr}")

    print()
    print("n이 10 늘 때마다 재귀 호출 수가 대략 100배가 된다.")
    print("반복은 n에 비례해 늘 뿐이다.")
