# 03. Kruskal 알고리즘 (Minimum Spanning Tree)

정점 8개, 간선 16개 가중치 그래프의 **최소 스패닝 트리**(MST)를 구한다.
간선을 가중치 순으로 정렬하고 가벼운 것부터 본다. 두 끝이 **다른
덩어리**에 있으면 넣고 덩어리를 합치며, 같은 덩어리면 사이클이 생기므로
버린다. 덩어리 번호는 `comp` 배열이 들고 있다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `kruskal.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `kruskal.c` | C 구현 |
| `kruskal.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/03_mst_kruskal
make -f /work/Makefile kruskal.out && ./kruskal.out
python3 kruskal.py
```

- 결과

```console
V = 8, E = 16
edges by weight
  0-7 0.16  add   comp [0 1 2 3 4 5 6 0]
  2-3 0.17  add   comp [0 1 2 2 4 5 6 0]
  1-7 0.19  add   comp [0 0 2 2 4 5 6 0]
  0-2 0.26  add   comp [0 0 0 0 4 5 6 0]
  5-7 0.28  add   comp [0 0 0 0 4 0 6 0]
  1-3 0.29  skip  (1 and 3 both in 0, cycle)
  1-5 0.32  skip  (1 and 5 both in 0, cycle)
  2-7 0.34  skip  (2 and 7 both in 0, cycle)
  4-5 0.35  add   comp [0 0 0 0 0 0 6 0]
  1-2 0.36  skip  (1 and 2 both in 0, cycle)
  4-7 0.37  skip  (4 and 7 both in 0, cycle)
  0-4 0.38  skip  (0 and 4 both in 0, cycle)
  6-2 0.40  add   comp [0 0 0 0 0 0 0 0]
7 edges = V - 1, stop
mst: 0-7 2-3 1-7 0-2 5-7 4-5 6-2
total weight = 1.81
```

두 구현이 같은 결과를 낸다. 가중치는 원래 표의 값에 100을 곱한 정수로
다루고 찍을 때만 소수로 바꾼다. 간선 13개를 보고 7개를 넣은 뒤 멈췄다.
버린 간선 여섯 개는 모두 이미 한 덩어리인 두 정점을 이으려던 것이다.
`comp`가 모두 0이 되는 순간 트리가 완성된다.

## 바꿔 보기

`merge`는 매번 배열 전체를 훑는다. 덩어리마다 대표를 가리키는
**union-find**(서로소 집합)로 바꿔 보자. 판정과 합치기가 거의 상수 시간이
되어 전체 비용이 정렬 `O(E log E)`에 묶인다.

## 다음

다음 예제(최소 힙)는 Prim과 Dijkstra가 쓰는 **우선순위 큐**를 만든다.
