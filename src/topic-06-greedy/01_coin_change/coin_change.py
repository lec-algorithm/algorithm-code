"""동전 거스름돈 — 언제나 가장 큰 동전부터 낸다 (그리디).

우리 동전(500·100·50·10)에서는 최적이다. 40원짜리 동전이 끼어
있으면 같은 절차가 최적을 놓친다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 coin_change.py`
"""


def coin_change(coins, amount):
    """쓴 동전의 총 개수와 단위별 개수를 돌려준다.

    coins는 큰 것부터 정렬되어 있다.
    """
    used = []
    count = 0
    for coin in coins:
        k = amount // coin
        used.append(k)
        count += k
        amount -= k * coin
    return count, used


def run(coins, amount):
    count, used = coin_change(coins, amount)
    print(f"coins = {{{', '.join(map(str, coins))}}}, amount = {amount}")
    parts = "".join(f" {coin}x{k}" for coin, k in zip(coins, used) if k > 0)
    print(f"greedy:{parts} -> {count} coins")


if __name__ == "__main__":
    run([500, 100, 50, 10], 1260)
    print()
    run([50, 40, 10], 80)
