"""거스름돈 — 동적 계획법으로 최소 동전 개수를 구한다.

같은 점화식을 메모이제이션(하향식)과 타뷸레이션(상향식) 두 가지로
푼다. 하향식은 필요한 금액만 풀고, 상향식은 모든 금액을 채운다.
주제 06에서 그리디가 4개를 낸 {50, 40, 10}, 80원도 다시 푼다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 coin_change.py`
"""

INF = float("inf")  # 만들 수 없는 금액

trace = False  # True이면 새로 푸는 금액을 찍는다
solved = 0  # 메모가 새로 푼 금액의 수


def coin_change_memo(amount, coins, memo):
    """하향식. memo[a]가 -1이면 아직 모른다."""
    global solved
    # 1. 기저
    if amount == 0:
        return 0
    if amount < 0:
        return INF
    # 2. 이미 구한 금액이면 바로 돌려준다
    if memo[amount] != -1:
        return memo[amount]
    if trace:
        print(f"  solve {amount}")
    solved += 1
    # 3. 동전마다 하나씩 내 보고 나머지를 재귀로
    best = INF
    for c in coins:
        r = coin_change_memo(amount - c, coins, memo)
        if r != INF and r + 1 < best:
            best = r + 1
    # 4. 적어 두고 돌려준다
    memo[amount] = best
    return best


def coin_change_tab(amount, coins):
    """상향식. (최소 개수, dp, choice)를 돌려준다.

    choice[a]는 a원에서 마지막에 낸 동전이다.
    """
    # 1. 초기화
    dp = [0] + [INF] * amount
    choice = [0] * (amount + 1)
    # 2. 계산
    for a in range(1, amount + 1):
        for c in coins:
            if c <= a and dp[a - c] != INF and dp[a - c] + 1 < dp[a]:
                dp[a] = dp[a - c] + 1
                choice[a] = c
    # 3. 반환
    return dp[amount], dp, choice


def cells(values):
    return ", ".join("INF" if v == INF else str(v) for v in values)


def run(coins, amount, show):
    global trace, solved
    print(f"coins = {{{', '.join(map(str, coins))}}}, amount = {amount}")

    memo = [-1] * (amount + 1)
    trace = show
    solved = 0
    r = coin_change_memo(amount, coins, memo)
    print(f"memo -> {cells([r])} coins, solved {solved} amounts")
    if show:
        print(f"  memo = [{cells(memo)}]")

    t, dp, choice = coin_change_tab(amount, coins)
    if t == INF:
        print(f"tab  -> INF, filled {amount} amounts")
    else:
        # 4. 경로 복원: choice를 거꾸로 따라간다
        used = []
        a = amount
        while a > 0:
            used.append(str(choice[a]))
            a -= choice[a]
        print(f"tab  -> {t} coins: {' + '.join(used)}, filled {amount} amounts")
    if show:
        print(f"  dp   = [{cells(dp)}]")


if __name__ == "__main__":
    run([2, 3, 5], 6, True)
    print()
    run([50, 40, 10], 80, False)
