"""A* 알고리즘 — f = g + h가 가장 작은 정점부터 열어 목표 쪽을 먼저 찾는다.

Dijkstra와 같은 정점 8개, 간선 16개 그래프에서 0에서 6까지 간다.
h는 정점마다 6까지 남은 거리의 추정값이다. 이 h는 실제 거리를 넘지
않지만(admissible) 간선 두 곳에서 일관성(consistent)이 깨진다.
그래서 닫힌 정점을 다시 열지 않으면 26, 다시 열면 25가 나온다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 a_star.py`
"""
import heapq
import itertools

INF = 1000000


def build(n, edges):
    """방향 간선 목록을 인접 목록으로. 이웃은 번호 오름차순."""
    adj = [[] for _ in range(n)]
    for u, v, w in edges:
        adj[u].append((v, w))
    for lst in adj:
        lst.sort()
    return adj


def a_star(adj, h, s, t, reopen):
    """t에 닿으면 (비용, 경로)를 돌려준다."""
    n = len(adj)
    g = [INF] * n
    came_from = [-1] * n
    closed = [False] * n
    expanded = []
    seq = itertools.count()  # f가 같으면 먼저 넣은 것이 먼저
    g[s] = 0
    open_list = [(h[s], next(seq), s)]

    while open_list:
        f, _, u = heapq.heappop(open_list)
        if closed[u]:
            print(f"pop {u} (f {f}): skip, already closed")
            continue
        head = f"pop {u} (f {f} = {g[u]} + {h[u]})"
        if u == t:
            print(head + ": goal")
            path = []
            x = t
            while x != -1:
                path.append(x)
                x = came_from[x]
            print("expanded:", *expanded)
            return g[t], path[::-1]
        print(head + ": close")
        closed[u] = True
        expanded.append(u)
        for v, w in adj[u]:
            ng = g[u] + w
            if ng >= g[v]:
                if not closed[v]:
                    print(f"  {v}: g {ng} >= {g[v]}, keep")
                continue
            if closed[v] and not reopen:
                print(f"  {v}: g {ng} < {g[v]}, but {v} is closed")
                continue
            if closed[v]:
                print(f"  {v}: g {ng} < {g[v]}, reopen, f {ng + h[v]}")
                closed[v] = False
            else:
                print(f"  {v}: g {ng}, f {ng + h[v]}")
            g[v] = ng
            came_from[v] = u
            heapq.heappush(open_list, (ng + h[v], next(seq), v))
    return INF, []


if __name__ == "__main__":
    edges = [
        (0, 1, 5), (0, 4, 9), (0, 7, 8), (1, 2, 12), (1, 3, 15), (1, 7, 4),
        (2, 3, 3), (2, 6, 11), (3, 6, 9), (4, 5, 4), (4, 6, 20), (4, 7, 5),
        (5, 2, 1), (5, 6, 13), (7, 5, 6), (7, 2, 7),
    ]
    h = [14, 10, 4, 2, 11, 6, 0, 8]
    adj = build(8, edges)

    print("V = 8, E = 16, start 0, goal 6")
    print("h:", *[f"{v}={h[v]}" for v in range(8)])
    bad = [f"{u}->{v} ({h[u]} > {w} + {h[v]})"
           for u in range(8) for v, w in adj[u] if h[u] > w + h[v]]
    print("inconsistent edges:", *bad)

    print()
    print("without reopening closed nodes")
    cost, path = a_star(adj, h, 0, 6, False)
    print("path:", *path, end="")
    print(f", cost {cost}")

    print()
    print("with reopening")
    cost, path = a_star(adj, h, 0, 6, True)
    print("path:", *path, end="")
    print(f", cost {cost}")
