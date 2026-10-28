"""Dijkstra 알고리즘 — 지금 가장 가까운 정점의 거리를 확정해 나간다.

1. 정점 8개, 간선 16개 방향 그래프에서 정점 0으로부터의 최단 거리를
   구한다. 힙에서 꺼낼 때와 거리를 고칠 때마다 한 줄씩 찍는다.
2. 음의 가중치가 있는 정점 4개짜리 그래프에서 틀린 답이 나오는 것을 본다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 dijkstra.py`
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


def dijkstra(adj, s):
    n = len(adj)
    dist = [INF] * n
    frm = [-1] * n
    done = [False] * n
    seq = itertools.count()  # 거리가 같으면 먼저 넣은 것이 먼저
    dist[s] = 0
    pq = [(0, next(seq), s)]

    while pq:
        d, _, u = heapq.heappop(pq)
        if done[u]:
            print(f"pop {u} (dist {d}): skip, already done")
            continue
        done[u] = True
        print(f"pop {u} (dist {d}): done")
        for v, w in adj[u]:
            nd = dist[u] + w
            if done[v]:
                if nd < dist[v]:
                    print(f"  {u}->{v}: {nd} < {dist[v]}, but {v} is already done")
            elif nd < dist[v]:
                dist[v] = nd
                frm[v] = u
                heapq.heappush(pq, (nd, next(seq), v))
                print(f"  {u}->{v}: dist[{v}] = {nd} via {u}")
            else:
                print(f"  {u}->{v}: {nd} >= {dist[v]}, keep")
    return dist, frm


def path_to(frm, t):
    path = []
    x = t
    while x != -1:
        path.append(x)
        x = frm[x]
    return path[::-1]


if __name__ == "__main__":
    edges = [
        (0, 1, 5), (0, 4, 9), (0, 7, 8), (1, 2, 12), (1, 3, 15), (1, 7, 4),
        (2, 3, 3), (2, 6, 11), (3, 6, 9), (4, 5, 4), (4, 6, 20), (4, 7, 5),
        (5, 2, 1), (5, 6, 13), (7, 5, 6), (7, 2, 7),
    ]
    adj = build(8, edges)
    print("V = 8, E = 16, start 0")
    dist, frm = dijkstra(adj, 0)

    print()
    print("  v  dist  from")
    for v in range(8):
        if frm[v] == -1:
            print(f"  {v}  {dist[v]:4d}     -")
        else:
            print(f"  {v}  {dist[v]:4d}  {frm[v]:4d}")
    print("path 0 -> 6:", *path_to(frm, 6), f"({dist[6]})")

    # 음의 가중치: 2->3이 -9
    neg_edges = [(0, 1, 4), (0, 3, 2), (1, 2, 6), (2, 3, -9)]
    neg = build(4, neg_edges)
    print()
    print("negative weight: 0->1 4, 0->3 2, 1->2 6, 2->3 -9")
    dist, frm = dijkstra(neg, 0)
    weight = {(u, v): w for u, v, w in neg_edges}
    real = weight[(0, 1)] + weight[(1, 2)] + weight[(2, 3)]
    print(f"dijkstra says dist[3] = {dist[3]}, but 0->1->2->3 costs {real}")
