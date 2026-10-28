"""폭 우선 검색(BFS) — 큐로 가까운 정점부터 층별로 방문한다.

간선 목록으로 받은 그래프를 인접 행렬과 인접 목록으로 바꿔 찍고,
정점 0에서 BFS를 돌리며 꺼낼 때마다 큐의 내용을 보인다.
끝나면 from과 dist 표, 그리고 0에서 4까지의 최단 경로를 찍는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 bfs.py`
"""
from collections import deque


def build(n, edges):
    """무방향 간선 목록을 인접 행렬과 인접 목록으로 바꾼다. 목록은 오름차순."""
    matrix = [[0] * n for _ in range(n)]
    adj = [[] for _ in range(n)]
    for u, v in edges:
        matrix[u][v] = matrix[v][u] = 1
        adj[u].append(v)
        adj[v].append(u)
    for lst in adj:
        lst.sort()  # 편의상 작은 번호부터 본다
    return matrix, adj


def bfs(adj, s):
    """s에서 BFS. 꺼낼 때마다 본 이웃과 새로 방문한 정점, 큐를 한 줄에 찍는다."""
    n = len(adj)
    marked = [False] * n
    frm = [-1] * n
    dist = [-1] * n
    order = []
    marked[s] = True
    dist[s] = 0
    queue = deque([s])
    print(f"visit {s}, queue [{' '.join(map(str, queue))}]")

    while queue:
        v = queue.popleft()
        order.append(v)
        fresh = []
        for w in adj[v]:
            if not marked[w]:
                marked[w] = True
                frm[w] = v
                dist[w] = dist[v] + 1
                queue.append(w)
                fresh.append(w)
        line = f"dequeue {v}: check {' '.join(map(str, adj[v]))}"
        if fresh:
            line += f" -> visit {' '.join(map(str, fresh))}"
        print(f"{line}, queue [{' '.join(map(str, queue))}]")
    return order, frm, dist


def path_to(frm, t):
    """from을 거슬러 올라가 s에서 t까지의 경로를 돌려준다."""
    path = []
    x = t
    while x != -1:
        path.append(x)
        x = frm[x]
    return path[::-1]


if __name__ == "__main__":
    # 정점 7개, 간선 9개
    edges = [(0, 1), (0, 2), (0, 5), (0, 6), (2, 3), (3, 4), (3, 5), (4, 5), (4, 6)]
    V = 7
    matrix, adj = build(V, edges)

    print(f"V = {V}, E = {len(edges)}")
    print()
    print("adjacency matrix")
    print("   " + "".join(f" {v}" for v in range(V)))
    for u in range(V):
        print(f"  {u}" + "".join(f" {x}" for x in matrix[u]))
    print()
    print("adjacency list")
    for v in range(V):
        print(f"  {v}:" + "".join(f" {w}" for w in adj[v]))

    print()
    print("BFS(0)")
    order, frm, dist = bfs(adj, 0)
    print("order:", *order)
    print()
    print("  v  from  dist")
    for v in range(V):
        if frm[v] == -1:
            print(f"  {v}     -  {dist[v]:4d}")
        else:
            print(f"  {v}  {frm[v]:4d}  {dist[v]:4d}")

    path = path_to(frm, 4)
    print()
    print("path 0 -> 4:", *path, f"({dist[4]} edges)")
