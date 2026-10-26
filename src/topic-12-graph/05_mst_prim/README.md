# 05. Prim 알고리즘 (Minimum Spanning Tree)

Kruskal과 같은 그래프의 MST를 이번에는 **정점 0에서 시작해 키워 간다**.
정점마다 `key`(트리에 붙는 가장 가벼운 간선의 가중치)와 `via`(그 간선의
트리 쪽 끝)를 두고, 최소 힙에서 `key`가 가장 작은 정점을 꺼내 트리에
붙인다. 붙인 정점의 이웃 중 더 가벼운 간선을 찾으면 `key`와 `via`를 고쳐
힙에 새로 넣는다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `prim.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `prim.c` | C 구현. 힙은 04_min_heap과 같은 절차로 직접 만든다 |
| `prim.py` | Python 구현. 힙은 표준 모듈 `heapq`를 쓴다 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/05_mst_prim
make -f /work/Makefile prim.out && ./prim.out
python3 prim.py
```

- 결과

```console
V = 8, E = 16, start 0
pop 0 (key 0.00): start
  2: key 0.26 via 0
  4: key 0.38 via 0
  6: key 0.58 via 0
  7: key 0.16 via 0
pop 7 (key 0.16): add 0-7
  1: key 0.19 via 7
  4: key 0.37 via 7
  5: key 0.28 via 7
pop 1 (key 0.19): add 7-1
  3: key 0.29 via 1
pop 2 (key 0.26): add 0-2
  3: key 0.17 via 2
  6: key 0.40 via 2
pop 3 (key 0.17): add 2-3
pop 5 (key 0.28): add 7-5
  4: key 0.35 via 5
pop 3 (key 0.29): skip, already in tree
pop 4 (key 0.35): add 5-4
pop 4 (key 0.37): skip, already in tree
pop 4 (key 0.38): skip, already in tree
pop 6 (key 0.40): add 2-6
7 edges = V - 1, stop

  v  key   via
  0  0.00  -
  1  0.19  7
  2  0.26  0
  3  0.17  2
  4  0.35  5
  5  0.28  7
  6  0.40  2
  7  0.16  0
total weight = 1.81
```

두 구현이 같은 결과를 낸다. 트리에 붙는 순서는 0, 7, 1, 2, 3, 5, 4, 6이고
총 가중치는 Kruskal과 같은 **1.81**이다. 정점 4의 `key`는 0.38에서 0.37,
다시 0.35로 두 번 줄었다. 힙의 항목을 고치는 대신 새로 넣었으므로, 낡은
항목(0.37, 0.38)은 나중에 꺼내져 `skip`으로 버려진다.

## 바꿔 보기

`prim(adj, 0)`의 시작 정점을 다른 번호로 바꿔 보자. 붙는 순서는 달라지지만
간선 일곱 개와 총 가중치 1.81은 그대로다. 가중치가 모두 다르면 MST는 하나뿐이다.

## 다음

다음 예제(Dijkstra)는 같은 틀에서 `key` 대신 **출발점에서의 거리**를 쓴다.
