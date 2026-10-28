"""격자 경로 — 장애물을 피해 (0, 0)에서 (5, 5)까지 가는 경로의 수.

한 번에 x+1 또는 y+1로만 움직인다. 메모이제이션(여기서 끝까지)과
타뷸레이션(처음부터 여기까지) 두 방향으로 센다. 메모 없는 재귀의
호출 수도 함께 찍는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 grid_paths.py`
"""

obstacles = [(2, 2), (3, 3), (4, 4)]  # 장애물 위치

use_memo = False  # False이면 표 없이 순수 재귀
calls = 0  # paths_memo가 불린 횟수
solved = 0  # 메모가 새로 센 칸의 수


def paths_memo(x, y, tx, ty, memo):
    """(x, y)에서 (tx, ty)까지 가는 경로의 수"""
    global calls, solved
    calls += 1
    # 목표 지점 도달
    if x == tx and y == ty:
        return 1
    # 경계를 벗어나거나 장애물
    if x > tx or y > ty or (x, y) in obstacles:
        return 0
    if use_memo and (x, y) in memo:
        return memo[(x, y)]
    count = paths_memo(x + 1, y, tx, ty, memo) + paths_memo(x, y + 1, tx, ty, memo)
    if use_memo:
        memo[(x, y)] = count
        solved += 1
    return count


def paths_tab(tx, ty):
    """(경로의 수, dp)를 돌려준다. dp[i][j] = (0, 0)에서 (i, j)까지 오는 경로의 수"""
    dp = [[0] * (ty + 1) for _ in range(tx + 1)]
    for i in range(tx + 1):
        for j in range(ty + 1):
            if (i, j) in obstacles:
                dp[i][j] = 0
            elif i == 0 and j == 0:
                dp[i][j] = 1
            else:
                up = dp[i - 1][j] if i > 0 else 0
                left = dp[i][j - 1] if j > 0 else 0
                dp[i][j] = up + left
    return dp[tx][ty], dp


if __name__ == "__main__":
    tx, ty = 5, 5
    obs = "".join(f" ({x}, {y})" for x, y in obstacles)
    print(f"grid {tx + 1} x {ty + 1}, (0, 0) -> ({tx}, {ty}), obstacles{obs}")
    print()

    use_memo = False
    calls = 0
    plain = paths_memo(0, 0, tx, ty, None)
    print(f"plain recursion = {plain}, calls = {calls}")

    use_memo = True
    calls = 0
    solved = 0
    m = paths_memo(0, 0, tx, ty, {})
    print(f"memo            = {m}, calls = {calls}, solved {solved} cells")

    t, dp = paths_tab(tx, ty)
    print(f"tab             = {t}")
    print()

    # 표를 찍는다. 장애물은 #
    print("  x\\y" + "".join(f"{j:5d}" for j in range(ty + 1)))
    for i in range(tx + 1):
        row = "".join(f"{'#':>5}" if (i, j) in obstacles else f"{dp[i][j]:5d}" for j in range(ty + 1))
        print(f"{i:4d} {row}")
