"""깊이 우선 검색(DFS) — 스택으로 한 갈래를 끝까지 파고든 뒤 돌아온다.

1. BFS와 같은 그래프에서 정점 0부터 DFS. 꺼낼 때마다 스택을 찍는다.
2. 응용: 정점 13개 그래프의 연결 요소를 재귀 DFS로 센다.
3. 응용: 과목 7개의 선수 관계(방향 그래프)를 위상 정렬한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 dfs.py`
"""


def build(n, edges, directed=False):
    """간선 목록을 인접 목록으로. 무방향이면 반대쪽에도 넣는다. 목록은 오름차순."""
    adj = [[] for _ in range(n)]
    for u, v in edges:
        adj[u].append(v)
        if not directed:
            adj[v].append(u)
    for lst in adj:
        lst.sort()
    return adj


def stack_str(stack):
    return "stack [" + " ".join(str(v) for v, _ in stack) + "]"


def dfs(adj, s):
    """s에서 스택 DFS. 스택 칸에는 (정점, 넣은 정점)을 둔다."""
    n = len(adj)
    marked = [False] * n
    frm = [-1] * n
    order = []
    stack = [(s, -1)]
    print(f"push {s}, {stack_str(stack)}")

    while stack:
        v, p = stack.pop()
        if marked[v]:
            print(f"pop {v}: already visited, {stack_str(stack)}")
            continue
        marked[v] = True
        frm[v] = p
        order.append(v)
        line = f"pop {v}: visit {v}, check {' '.join(map(str, adj[v]))}"
        pushed = []
        for w in adj[v]:
            if not marked[w]:
                stack.append((w, v))
                pushed.append(w)
        if pushed:
            line += f" -> push {' '.join(map(str, pushed))}"
        print(f"{line}, {stack_str(stack)}")
    return order, frm


def components(adj):
    """재귀 DFS: 한 번 들어가서 닿는 정점에 모두 같은 id를 준다."""
    n = len(adj)
    marked = [False] * n
    ids = [-1] * n

    def visit(v, c):
        marked[v] = True
        ids[v] = c
        for w in adj[v]:
            if not marked[w]:
                visit(w, c)

    count = 0
    for v in range(n):
        if not marked[v]:
            visit(v, count)
            count += 1
    return count, ids


def topological_sort(adj):
    """나가는 간선을 다 본 뒤에 번호를 준다. 먼저 끝나는 정점이 뒤 번호를 받는다."""
    n = len(adj)
    marked = [False] * n
    f = [0] * n
    label = n

    def finish(s):
        nonlocal label
        marked[s] = True
        for v in adj[s]:
            if not marked[v]:
                finish(v)
        f[s] = label
        label -= 1

    for v in range(n):
        if not marked[v]:
            finish(v)
    order = [0] * n
    for v in range(n):
        order[f[v] - 1] = v
    return f, order


if __name__ == "__main__":
    # 1. BFS 예제와 같은 그래프
    edges = [(0, 1), (0, 2), (0, 5), (0, 6), (2, 3), (3, 4), (3, 5), (4, 5), (4, 6)]
    adj = build(7, edges)
    print("DFS(0)")
    order, frm = dfs(adj, 0)
    print("order:", *order)
    print()
    print("  v  from")
    for v in range(len(adj)):
        if frm[v] == -1:
            print(f"  {v}     -")
        else:
            print(f"  {v}  {frm[v]:4d}")

    # 2. 연결 요소: 정점 13개, 간선 13개
    edges13 = [(0, 1), (0, 2), (0, 5), (0, 6), (3, 4), (3, 5), (4, 5),
               (4, 6), (7, 8), (9, 10), (9, 11), (9, 12), (11, 12)]
    adj13 = build(13, edges13)
    count, ids = components(adj13)
    print()
    print("connected components (V = 13, E = 13)")
    for c in range(count):
        print(f"  id {c}:", *[v for v in range(13) if ids[v] == c])
    print(f"count = {count}")

    # 3. 위상 정렬: 과목 7개, 선수 관계 11개 (u -> v: u를 먼저 듣는다)
    names = ["Algorithms", "Complexity Theory", "Artificial Intelligence", "Intro to CS",
             "Cryptography", "Scientific Computing", "Advanced Programming"]
    prereq = [(0, 1), (0, 2), (0, 5), (1, 4), (3, 2), (3, 4),
              (3, 5), (3, 6), (5, 2), (6, 0), (6, 4)]
    dag = build(7, prereq, directed=True)
    f, topo = topological_sort(dag)
    print()
    print("topological sort (V = 7, E = 11, directed)")
    print("  f:", *[f"{v}={f[v]}" for v in range(7)])
    print("  order:", *topo)
    for i, v in enumerate(topo):
        print(f"  {i + 1}. {names[v]}")
