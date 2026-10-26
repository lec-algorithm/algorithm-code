"""Kruskal 알고리즘 — 가벼운 간선부터, 사이클을 만들지 않으면 넣는다.

정점 8개, 간선 16개 그래프의 최소 스패닝 트리를 구한다. 간선을 가중치
순으로 정렬하고, 두 끝이 다른 덩어리(연결 요소)에 있으면 넣고 덩어리를
합친다. 같은 덩어리면 사이클이 생기므로 버린다.

가중치는 원래 표(0.16 등)에 100을 곱한 정수로 다룬다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 kruskal.py`
"""


def weight_str(w):
    return f"{w // 100}.{w % 100:02d}"


def merge(comp, a, b):
    """덩어리 a와 b를 합친다. 번호는 둘 중 작은 쪽(덩어리의 가장 작은 정점)."""
    small = min(a, b)
    for x in range(len(comp)):
        if comp[x] == a or comp[x] == b:
            comp[x] = small


def kruskal(n, edges):
    """edges를 정렬해 Kruskal을 돌리고, 고른 간선 목록을 돌려준다."""
    comp = list(range(n))
    mst = []
    for u, v, w in sorted(edges, key=lambda e: e[2]):
        head = f"  {u}-{v} {weight_str(w)}"
        if comp[u] != comp[v]:
            mst.append((u, v, w))
            merge(comp, comp[u], comp[v])
            print(f"{head}  add   comp [{' '.join(map(str, comp))}]")
            if len(mst) == n - 1:
                print(f"{len(mst)} edges = V - 1, stop")
                break
        else:
            print(f"{head}  skip  ({u} and {v} both in {comp[u]}, cycle)")
    return mst


if __name__ == "__main__":
    # 입력 순서는 정렬되어 있지 않다.
    edges = [
        (4, 5, 35), (4, 7, 37), (5, 7, 28), (0, 7, 16), (1, 5, 32), (0, 4, 38),
        (2, 3, 17), (1, 7, 19), (0, 2, 26), (1, 2, 36), (1, 3, 29), (2, 7, 34),
        (6, 2, 40), (3, 6, 52), (6, 0, 58), (6, 4, 93),
    ]
    n = 8
    print(f"V = {n}, E = {len(edges)}")
    print("edges by weight")
    mst = kruskal(n, edges)
    print("mst:", *[f"{u}-{v}" for u, v, _ in mst])
    print(f"total weight = {weight_str(sum(w for _, _, w in mst))}")
