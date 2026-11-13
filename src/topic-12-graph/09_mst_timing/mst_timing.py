"""최소 스패닝 트리 시간 재기 — Kruskal 두 가지, Prim 두 가지를 희소 그래프와 완전 그래프에서.

  Kruskal comp   정렬 + comp 배열 합치기(03_mst_kruskal)   O(E log E + V^2)
  Kruskal UF     정렬 + union-find(08_union_find)           O(E log E)
  Prim heap      최소 힙(05_mst_prim, heapq)                O(E log V)
  Prim array     key 배열 V칸을 훑어 고른다                 O(V^2 + E)

그래프는 셋이다. 희소(E = 4V), 무작위 가중치 완전 그래프, 그리고 간선마다 key가
줄어 힙에 가장 나쁜 완전 그래프. C보다 느리므로 크기를 줄였다.

값은 밀리초다. 컴퓨터마다 다르므로 비율을 본다. 네 방법이 낸 총 가중치가
같은지 확인한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 mst_timing.py`
"""
import heapq
import time

MIN_SECONDS = 0.2
INF = float("inf")


class Rng:
    """x <- (1664525 x + 1013904223) mod 2^32, rand(k) = x mod k. C와 같은 수열"""

    def __init__(self, seed):
        self.x = seed

    def rand(self, k):
        self.x = (1664525 * self.x + 1013904223) & 0xFFFFFFFF
        return self.x % k


def make_sparse(n, rng):
    edges = []
    for v in range(1, n):  # 앞의 정점에 이어 연결을 보장한다
        u = rng.rand(v)
        edges.append((u, v, rng.rand(1000000)))
    while len(edges) < 4 * n:
        u, v = rng.rand(n), rng.rand(n)
        if u != v:
            edges.append((u, v, rng.rand(1000000)))
    return edges


def make_complete(n, rng):
    return [(u, v, rng.rand(1000000)) for u in range(n) for v in range(u + 1, n)]


def make_worst_complete(n):
    """u < v인 간선의 가중치를 (V - u) x V + v로 둔다. Prim은 0, 1, 2, … 순으로 붙이고,
    u를 붙일 때마다 남은 모든 정점의 key가 줄어 간선마다 한 번씩 힙에 넣는다."""
    return [(u, v, (n - u) * n + v) for u in range(n) for v in range(u + 1, n)]


def adjacency(n, edges):
    adj = [[] for _ in range(n)]
    for u, v, w in edges:
        adj[u].append((v, w))
        adj[v].append((u, w))
    return adj


def kruskal_comp(n, edges, adj):
    comp = list(range(n))
    total = k = 0
    for u, v, w in sorted(edges, key=lambda e: (e[2], e[0], e[1])):
        a, b = comp[u], comp[v]
        if a == b:
            continue
        total += w
        k += 1
        small = min(a, b)
        for x in range(n):  # 합치기: V칸을 다 훑는다
            if comp[x] == a or comp[x] == b:
                comp[x] = small
        if k == n - 1:
            break
    return total


def kruskal_uf(n, edges, adj):
    parent = list(range(n))
    size = [1] * n

    def find(x):
        root = x
        while parent[root] != root:
            root = parent[root]
        while parent[x] != root:
            parent[x], x = root, parent[x]
        return root

    total = k = 0
    for u, v, w in sorted(edges, key=lambda e: (e[2], e[0], e[1])):
        a, b = find(u), find(v)
        if a == b:
            continue
        total += w
        k += 1
        if size[a] < size[b]:
            a, b = b, a
        parent[b] = a
        size[a] += size[b]
        if k == n - 1:
            break
    return total


def prim_heap(n, edges, adj):
    key = [INF] * n
    in_tree = [False] * n
    key[0] = 0
    heap = [(0, 0)]
    total = 0
    while heap:
        k, u = heapq.heappop(heap)
        if in_tree[u]:  # 더 가벼운 간선으로 이미 붙었다
            continue
        in_tree[u] = True
        total += k
        for v, w in adj[u]:
            if not in_tree[v] and w < key[v]:
                key[v] = w
                heapq.heappush(heap, (w, v))
    return total


def prim_array(n, edges, adj):
    key = [INF] * n
    in_tree = [False] * n
    key[0] = 0
    total = 0
    for _ in range(n):
        u = -1
        for v in range(n):  # V칸을 훑어 가장 작은 key
            if not in_tree[v] and (u < 0 or key[v] < key[u]):
                u = v
        in_tree[u] = True
        total += key[u]
        for v, w in adj[u]:
            if not in_tree[v] and w < key[v]:
                key[v] = w
    return total


def measure(mst, n, edges, adj, expected):
    """한 번 구하는 데 걸린 평균 시간(ms). 짧으면 여러 번 돌린다. 첫 번은 재지 않고 데운다."""
    mst(n, edges, adj)
    spent = 0.0
    runs = 0
    while True:
        t0 = time.perf_counter()
        total = mst(n, edges, adj)
        spent += time.perf_counter() - t0
        runs += 1
        if expected[0] is None:
            expected[0] = total
        if total != expected[0]:
            raise SystemExit("different total weight")
        if spent >= MIN_SECONDS:
            return spent / runs * 1000


def row(n, edges):
    adj = adjacency(n, edges)
    expected = [None]
    ms = [measure(f, n, edges, adj, expected) for f in (kruskal_comp, kruskal_uf, prim_heap, prim_array)]
    print(f"{n:7d} {len(edges):9d}  {ms[0]:12.3f}  {ms[1]:10.3f}  {ms[2]:9.3f}  {ms[3]:10.3f}")


HEAD = f"{'V':>7} {'E':>9}  {'Kruskal comp':>12}  {'Kruskal UF':>10}  {'Prim heap':>9}  {'Prim array':>10}"

if __name__ == "__main__":
    rng = Rng(2026)
    print("sparse, E = 4V (ms)")
    print(HEAD)
    for n in (500, 1000, 2000):
        row(n, make_sparse(n, rng))
    print()
    print("complete, E = V(V-1)/2 (ms)")
    print(HEAD)
    for n in (250, 500, 1000):
        row(n, make_complete(n, rng))
    print()
    print("complete, every edge lowers a key (ms)")
    print(HEAD)
    for n in (250, 500, 1000):
        row(n, make_worst_complete(n))
