"""Prim 알고리즘 — 정점 하나에서 시작해 트리에 가장 가까운 정점을 붙여 간다.

Kruskal과 같은 정점 8개, 간선 16개 그래프를 정점 0에서 시작한다.
최소 힙(우선순위 큐)에서 key가 가장 작은 정점을 꺼내 트리에 붙이고,
그 정점의 이웃 중 더 가벼운 간선을 찾으면 key와 via를 고쳐 힙에 넣는다.

가중치는 원래 표(0.16 등)에 100을 곱한 정수로 다룬다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 prim.py`
"""
import heapq
import itertools

INF = 1000000


def weight_str(w):
    return f"{w // 100}.{w % 100:02d}"


def prim(adj, s):
    n = len(adj)
    key = [INF] * n
    via = [-1] * n
    in_tree = [False] * n
    seq = itertools.count()  # key가 같으면 먼저 넣은 것이 먼저
    key[s] = 0
    pq = [(0, next(seq), s)]
    edges = total = 0

    while pq and edges < n - 1:
        k, _, u = heapq.heappop(pq)
        if in_tree[u]:
            print(f"pop {u} (key {weight_str(k)}): skip, already in tree")
            continue
        in_tree[u] = True
        if u == s:
            print(f"pop {u} (key {weight_str(k)}): start")
        else:
            print(f"pop {u} (key {weight_str(k)}): add {via[u]}-{u}")
            edges += 1
            total += k
        for v, w in adj[u]:
            if not in_tree[v] and w < key[v]:
                key[v] = w
                via[v] = u
                heapq.heappush(pq, (w, next(seq), v))
                print(f"  {v}: key {weight_str(w)} via {u}")
    print(f"{edges} edges = V - 1, stop")
    return key, via, total


if __name__ == "__main__":
    edges = [
        (4, 5, 35), (4, 7, 37), (5, 7, 28), (0, 7, 16), (1, 5, 32), (0, 4, 38),
        (2, 3, 17), (1, 7, 19), (0, 2, 26), (1, 2, 36), (1, 3, 29), (2, 7, 34),
        (6, 2, 40), (3, 6, 52), (6, 0, 58), (6, 4, 93),
    ]
    n = 8
    adj = [[] for _ in range(n)]
    for u, v, w in edges:
        adj[u].append((v, w))
        adj[v].append((u, w))
    for lst in adj:
        lst.sort()  # 이웃은 번호 오름차순

    print(f"V = {n}, E = {len(edges)}, start 0")
    key, via, total = prim(adj, 0)

    print()
    print("  v  key   via")
    for v in range(n):
        print(f"  {v}  {weight_str(key[v])}  {'-' if via[v] == -1 else via[v]}")
    print(f"total weight = {weight_str(total)}")
