"""1-sum, 2-sum, 3-sum — 연산을 세서 실행 시간의 수학적 모델을 만든다.

대표 연산으로 배열 접근을 센다. n을 두 배로 하면 접근이 몇 배가
되는지(배가 실험)가 시간 복잡도의 차수를 알려준다. 입력은 주제 04의
minstd 생성기로 만들어 C와 Python이 같은 배열을 본다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 three_sum.py`
"""

state = 1


def next_random():
    global state
    state = state * 16807 % 2147483647
    return state


def fill_array(n):
    """-100 이상 100 이하의 정수 n개를 만든다."""
    global state
    state = 1
    return [next_random() % 201 - 100 for _ in range(n)]


def one_sum(a):
    """0인 원소의 개수와 배열 접근 횟수를 돌려준다."""
    count = accesses = 0
    for value in a:
        accesses += 1
        if value == 0:
            count += 1
    return count, accesses


def two_sum(a):
    """합이 0이 되는 쌍의 개수와 배열 접근 횟수를 돌려준다."""
    count = accesses = 0
    n = len(a)
    for i in range(n):
        for j in range(i + 1, n):
            accesses += 2
            if a[i] + a[j] == 0:
                count += 1
    return count, accesses


def three_sum(a):
    """합이 0이 되는 셋의 개수와 배열 접근 횟수를 돌려준다."""
    count = accesses = 0
    n = len(a)
    for i in range(n):
        for j in range(i + 1, n):
            for k in range(j + 1, n):
                accesses += 3
                if a[i] + a[j] + a[k] == 0:
                    count += 1
    return count, accesses


if __name__ == "__main__":
    a = fill_array(100)
    print("N = 100")
    for name, fn in (("1-sum", one_sum), ("2-sum", two_sum), ("3-sum", three_sum)):
        count, accesses = fn(a)
        print(f"{name}: count = {count}, accesses = {accesses}")

    print("\ndoubling N: accesses ratio")
    print(f"{'N':>6} {'2-sum':>12} {'3-sum':>12}")
    prev2 = prev3 = 0
    n = 100
    while n <= 400:
        a = fill_array(n)
        _, acc2 = two_sum(a)
        _, acc3 = three_sum(a)
        if prev2 > 0:
            print(f"{n:>6} {acc2 / prev2:>12.2f} {acc3 / prev3:>12.2f}")
        prev2, prev3 = acc2, acc3
        n *= 2
