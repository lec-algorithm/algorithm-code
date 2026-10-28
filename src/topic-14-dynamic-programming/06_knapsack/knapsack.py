"""0/1 배낭 — 물건을 통째로만 담아 가치의 합을 최대로.

주제 06에서 그리디가 160에서 멈춘 문제다. dp[i][c] 표를 채우고,
아래에서 위로 거꾸로 따라가 어떤 물건을 담았는지 복원한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 knapsack.py`
"""


def knapsack(w, v, n, capacity):
    """(최대 가치, dp)를 돌려준다. 물건은 1번부터 센다(w[0], v[0]은 쓰지 않는다).

    dp[i][c]: 물건 1..i만 써서 용량 c에 담을 수 있는 최대 가치
    """
    # 1. 물건이 없으면 가치 0
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]
    # 2. 물건을 하나씩 늘려 가며
    for i in range(1, n + 1):
        for c in range(capacity + 1):
            dp[i][c] = dp[i - 1][c]  # 안 담는 경우
            if w[i] <= c and dp[i - 1][c - w[i]] + v[i] > dp[i][c]:
                dp[i][c] = dp[i - 1][c - w[i]] + v[i]  # 담는 경우
    return dp[n][capacity], dp


if __name__ == "__main__":
    w = [0, 10, 20, 30]
    v = [0, 60, 100, 120]
    n, capacity, step = 3, 50, 10

    print(f"capacity = {capacity}")
    print("items:" + "".join(f" (w={w[i]}, v={v[i]})" for i in range(1, n + 1)))
    print()

    best, dp = knapsack(w, v, n, capacity)

    print(f"dp (columns every {step})")
    print("  i\\c" + "".join(f"{c:5d}" for c in range(0, capacity + 1, step)))
    for i in range(n + 1):
        print(f"  {i:<3d}" + "".join(f"{dp[i][c]:5d}" for c in range(0, capacity + 1, step)))
    print()
    print(f"best = {best}")

    # 3. 경로 복원: 위 칸과 다르면 그 물건을 담은 것
    c = capacity
    for i in range(n, 0, -1):
        if dp[i][c] != dp[i - 1][c]:
            print(f"  take item {i} (w={w[i]}, v={v[i]})")
            c -= w[i]
