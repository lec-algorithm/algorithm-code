"""피보나치 — 재귀, 메모이제이션, 타뷸레이션.

주제 01의 재귀가 느린 까닭은 같은 항을 몇 번이고 다시 구하기
때문이었다. 답을 적어 두면(메모) 재귀도 O(n)이 된다. 작은 항부터
표를 채워도(타뷸레이션) 같다. 호출 수를 세어 셋을 견준다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 fibonacci.py`
"""

calls = 0  # 함수가 불린 횟수


def fib_recursive(n):
    """정의를 그대로 옮긴 재귀. O(2^n)"""
    global calls
    calls += 1
    if n <= 1:
        return n
    return fib_recursive(n - 1) + fib_recursive(n - 2)


def fib_memo(n, memo):
    """메모이제이션. memo[k]가 -1이면 아직 모른다는 뜻이다. O(n)"""
    global calls
    calls += 1
    if n <= 1:
        return n
    if memo[n] != -1:
        return memo[n]  # 이미 구한 값: 다시 갈라지지 않는다
    memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo)
    return memo[n]


def fib_tabulation(n):
    """타뷸레이션. dp[0]부터 dp[n]까지 빠짐없이 채운다. O(n)

    (값, 표)를 돌려준다.
    """
    dp = [0] * (n + 1)
    if n <= 1:
        return n, dp
    dp[1] = 1
    for i in range(2, n + 1):
        dp[i] = dp[i - 1] + dp[i - 2]
    return dp[n], dp


def show(label, values):
    print(f"  {label:<10} = [{', '.join(map(str, values))}]")


if __name__ == "__main__":
    n = 5
    print(f"F({n})")

    calls = 0
    r = fib_recursive(n)
    print(f"  {'recursive':<10} = {r}, calls = {calls}")

    memo = [-1] * (n + 1)
    calls = 0
    m = fib_memo(n, memo)
    print(f"  {'memo':<10} = {m}, calls = {calls}")
    show("memo", memo)

    t, dp = fib_tabulation(n)
    print(f"  {'tabulation':<10} = {t}")
    show("dp", dp)

    print()
    print(" n  recursive calls  memo calls                 F(n)")
    for k in (10, 20, 30, 90):
        rec = "-"
        if k <= 30:  # 90이면 재귀는 끝나지 않는다
            calls = 0
            fib_recursive(k)
            rec = str(calls)
        memo = [-1] * (k + 1)
        calls = 0
        value = fib_memo(k, memo)
        print(f"{k:3d} {rec:>16} {calls:11d} {value:20d}")
