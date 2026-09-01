"""배가 실험을 실제 시간으로 — n을 두 배로 하면 시간이 몇 배가 되는가.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 doubling_time.py`
접근 횟수와 달리 측정값은 기계마다 다르다. 비율이 4와 8 근처로
모이는 것을 본다.
"""

import time

from three_sum import fill_array, three_sum, two_sum


def measure(fn, a):
    start = time.perf_counter()
    fn(a)
    return time.perf_counter() - start


if __name__ == "__main__":
    print(f"{'N':>6} {'2-sum(s)':>10} {'ratio':>7} {'3-sum(s)':>10} {'ratio':>7}")
    prev2 = prev3 = 0.0
    n = 200
    while n <= 1600:
        a = fill_array(n)
        t2 = measure(two_sum, a)
        t3 = measure(three_sum, a)
        r2 = f"{t2 / prev2:7.1f}" if prev2 else f"{'-':>7}"
        r3 = f"{t3 / prev3:7.1f}" if prev3 else f"{'-':>7}"
        print(f"{n:>6} {t2:>10.4f} {r2} {t3:>10.4f} {r3}")
        prev2, prev3 = t2, t3
        n *= 2
