"""최단 경로 — 간선을 정확히 k개 써서 u에서 v로 가는 가장 싼 길.

순수 재귀와 메모이제이션으로 같은 답을 구하고, 메모 표에 적힌
칸을 찍는다. 가장 싼 첫 걸음(next)을 함께 적어 두었다가 길을
복원한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 shortest_path.py`
"""

INF = float("inf")  # 갈 수 없음

# graph[u][x]: u -> x 간선의 비용. 0이면 간선이 없다.
graph = [
    [0, 1, 2, 0, 0, 0],  # 0 -> 1 (1), 0 -> 2 (2)
    [0, 0, 0, 6, 0, 0],  # 1 -> 3 (6)
    [0, 2, 0, 4, 3, 0],  # 2 -> 1 (2), 2 -> 3 (4), 2 -> 4 (3)
    [0, 0, 0, 0, 0, 3],  # 3 -> 5 (3)
    [0, 0, 0, 1, 0, 5],  # 4 -> 3 (1), 4 -> 5 (5)
    [0, 0, 0, 0, 0, 0],  # 5: 나가는 간선 없음
]
V = len(graph)

use_memo = False  # False이면 표 없이 순수 재귀
calls = 0  # shortest_path가 불린 횟수
memo = {}  # (k, u, v) -> 비용
nxt = {}  # (k, u, v) -> 가장 싼 첫 걸음


def cost(c):
    return "INF" if c == INF else str(c)


def shortest_path(u, v, k):
    global calls
    calls += 1
    # 1. 기저: 간선을 다 썼다
    if k == 0:
        return 0 if u == v else INF
    # 2. 이미 구한 (k, u, v)면 바로
    if use_memo and (k, u, v) in memo:
        print(f"  hit memo[{k}][{u}][{v}] = {cost(memo[(k, u, v)])}")
        return memo[(k, u, v)]
    # 3. 첫 간선 u -> x를 하나씩 골라 본다
    best = INF
    for x in range(V):
        if graph[u][x] == 0:
            continue
        r = shortest_path(x, v, k - 1)
        if r != INF and graph[u][x] + r < best:
            best = graph[u][x] + r
            nxt[(k, u, v)] = x
    # 4. 적어 두고 돌려준다
    if use_memo:
        memo[(k, u, v)] = best
    return best


if __name__ == "__main__":
    src, dst, k = 0, 3, 3

    edges = [f"{u}->{x} ({graph[u][x]})" for u in range(V) for x in range(V) if graph[u][x] != 0]
    print("edges: " + ", ".join(edges))
    print(f"{src} -> {dst} with exactly {k} edges")
    print()

    use_memo = False
    calls = 0
    plain = shortest_path(src, dst, k)
    print(f"plain recursion = {cost(plain)}, calls = {calls}")

    use_memo = True
    calls = 0
    m = shortest_path(src, dst, k)
    print(f"memo            = {cost(m)}, calls = {calls}")

    # 메모 표에 적힌 칸 (v = dst)
    for a in range(1, k + 1):
        cells = [f"{u}->{dst} = {cost(memo[(a, u, dst)])}" for u in range(V) if (a, u, dst) in memo]
        print(f"  k={a}: " + ", ".join(cells))

    # 5. 경로 복원: 적어 둔 첫 걸음을 따라간다
    if m == INF:
        print("no path")
    else:
        path = [src]
        u = src
        for a in range(k, 0, -1):
            u = nxt[(a, u, dst)]
            path.append(u)
        weights = " + ".join(str(graph[path[i]][path[i + 1]]) for i in range(k))
        print(f"path: {' -> '.join(map(str, path))} ({weights} = {m})")
