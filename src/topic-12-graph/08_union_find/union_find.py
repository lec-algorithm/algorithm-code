"""union-find로 하는 Kruskal — 덩어리를 트리로 들고, 뿌리가 같으면 같은 덩어리다.

03_mst_kruskal과 같은 정점 8개, 간선 16개 그래프를 쓴다. 덩어리 번호를 배열
전체에 고쳐 쓰는 대신, 정점마다 부모(parent) 하나만 둔다. 부모를 따라 올라가
닿는 뿌리가 그 덩어리의 이름이다.
  - 크기로 합치기(union by size): 작은 트리를 큰 트리의 뿌리 밑에 단다.
  - 경로 압축(path compression): Find가 지나온 정점을 뿌리에 바로 붙인다.

뒷부분은 무작위 큰 그래프에서 comp 배열이 훑는 칸 수와 union-find가 따라간
링크 수를 센다. 시간이 아니라 세는 값이라 C와 Python의 출력이 같다.

가중치는 원래 표(0.16 등)에 100을 곱한 정수로 다룬다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 union_find.py`
"""


class UnionFind:
    def __init__(self, n):
        self.parent = list(range(n))
        self.size = [1] * n
        self.links = 0  # Find가 따라간 부모 링크 수
        self.path = ""  # 마지막 Find가 지나온 경로, "3>2>0"

    def find(self, x, trace=False):
        parent = self.parent
        root = x
        steps = [x] if trace else None
        while parent[root] != root:
            root = parent[root]
            self.links += 1
            if trace:
                steps.append(root)
        while parent[x] != root:  # 경로 압축
            parent[x], x = root, parent[x]
        if trace:
            self.path = ">".join(map(str, steps))
        return root

    def unite(self, a, b):
        """a, b는 서로 다른 뿌리. 붙인 쪽(아래로 간 뿌리)을 돌려준다."""
        size = self.size
        if size[a] < size[b] or (size[a] == size[b] and a > b):
            a, b = b, a
        self.parent[b] = a
        size[a] += size[b]
        return b


def weight_str(w):
    return f"{w // 100}.{w % 100:02d}"


def kruskal_trace(n, edges):
    """작은 그래프: 한 줄에 간선 하나씩, 두 Find의 경로와 결과를 찍는다."""
    uf = UnionFind(n)
    mst = []
    print("edge      find u    find v    result")
    for u, v, w in sorted(edges, key=lambda e: (e[2], e[0], e[1])):
        before = uf.parent[:]
        a = uf.find(u, trace=True)
        pu = uf.path
        b = uf.find(v, trace=True)
        line = f"{u}-{v} {weight_str(w)}  {pu:<9} {uf.path:<9} "
        if a != b:
            mst.append((u, v, w))
            low = uf.unite(a, b)
            result = f"add   {low} under {uf.parent[low]}"
        else:
            result = f"skip  root {a}"
        if uf.parent != before:  # 부모가 바뀐 줄만 parent를 찍는다
            line += f"{result:<15}  parent [{' '.join(map(str, uf.parent))}]"
        else:
            line += result
        print(line)
        if len(mst) == n - 1:
            print(f"{len(mst)} edges = V - 1, stop")
            break
    return mst, uf.links


class Rng:
    """x <- (1664525 x + 1013904223) mod 2^32, rand(k) = x mod k. C와 같은 수열"""

    def __init__(self, seed):
        self.x = seed

    def rand(self, k):
        self.x = (1664525 * self.x + 1013904223) & 0xFFFFFFFF
        return self.x % k


def count_large(n, rng):
    m = 4 * n
    edges = []
    for v in range(1, n):  # 앞의 정점에 이어 연결을 보장한다
        u = rng.rand(v)
        edges.append((u, v, rng.rand(1000000)))
    while len(edges) < m:
        u, v = rng.rand(n), rng.rand(n)
        if u == v:
            continue
        edges.append((u, v, rng.rand(1000000)))
    edges.sort(key=lambda e: (e[2], e[0], e[1]))
    uf = UnionFind(n)
    finds = 0
    k = 0
    for u, v, _ in edges:
        if k == n - 1:
            break
        a, b = uf.find(u), uf.find(v)
        finds += 2
        if a != b:
            uf.unite(a, b)
            k += 1
    # V, comp 배열이 훑는 칸(합치기 V-1번 x V칸), Find 수, 따라간 링크, Find 한 번에 평균 링크
    print(f"{n:>9,} {(n - 1) * n:>16,} {finds:>9,} {uf.links:>9,} {uf.links / finds:>10.2f}")


if __name__ == "__main__":
    # 입력 순서는 정렬되어 있지 않다.
    edges = [
        (4, 5, 35), (4, 7, 37), (5, 7, 28), (0, 7, 16), (1, 5, 32), (0, 4, 38),
        (2, 3, 17), (1, 7, 19), (0, 2, 26), (1, 2, 36), (1, 3, 29), (2, 7, 34),
        (6, 2, 40), (3, 6, 52), (6, 0, 58), (6, 4, 93),
    ]
    n = 8
    print(f"V = {n}, E = {len(edges)}, union by size + path compression")
    mst, links = kruskal_trace(n, edges)
    print("mst:", *[f"{u}-{v}" for u, v, _ in mst])
    print(f"total weight = {weight_str(sum(w for _, _, w in mst))}")
    k = len(mst)
    print(f"links followed = {links} (comp array: {k} merges x {n} cells = {k * n})")

    print()
    print("random connected graphs, E = 4V, seed 2026")
    print(f"{'V':>9} {'comp cells':>16} {'finds':>9} {'links':>9} {'links/find':>10}")
    rng = Rng(2026)
    for size in (1000, 10000, 100000):
        count_large(size, rng)
