# 02. 깊이 우선 검색 (Depth-First Search)

BFS와 같은 그래프를 이번에는 **스택**(LIFO)으로 훑는다. 가장 나중에 넣은
정점을 먼저 꺼내므로 한 갈래를 끝까지 파고든 뒤에야 돌아온다. 이어서
DFS의 응용 둘을 돌린다. 하나는 **연결 요소** 세기, 다른 하나는 과목
선수 관계의 **위상 정렬**이다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `dfs.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `dfs.c` | C 구현 |
| `dfs.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/02_dfs
make -f /work/Makefile dfs.out && ./dfs.out
python3 dfs.py
```

- 결과

```console
DFS(0)
push 0, stack [0]
pop 0: visit 0, check 1 2 5 6 -> push 1 2 5 6, stack [1 2 5 6]
pop 6: visit 6, check 0 4 -> push 4, stack [1 2 5 4]
pop 4: visit 4, check 3 5 6 -> push 3 5, stack [1 2 5 3 5]
pop 5: visit 5, check 0 3 4 -> push 3, stack [1 2 5 3 3]
pop 3: visit 3, check 2 4 5 -> push 2, stack [1 2 5 3 2]
pop 2: visit 2, check 0 3, stack [1 2 5 3]
pop 3: already visited, stack [1 2 5]
pop 5: already visited, stack [1 2]
pop 2: already visited, stack [1]
pop 1: visit 1, check 0, stack []
order: 0 6 4 5 3 2 1

  v  from
  0     -
  1     0
  2     3
  3     5
  4     6
  5     4
  6     0

connected components (V = 13, E = 13)
  id 0: 0 1 2 3 4 5 6
  id 1: 7 8
  id 2: 9 10 11 12
count = 3

topological sort (V = 7, E = 11, directed)
  f: 0=3 1=6 2=5 3=1 4=7 5=4 6=2
  order: 3 6 0 5 2 1 4
  1. Intro to CS
  2. Advanced Programming
  3. Algorithms
  4. Scientific Computing
  5. Artificial Intelligence
  6. Complexity Theory
  7. Cryptography
```

두 구현이 같은 결과를 낸다. 이웃을 작은 번호부터 넣으니 가장 큰 번호가
맨 위에 오고, 0 다음에 6, 4, 5, 3으로 파고든다. 같은 정점이 스택에 두 번
들어갈 수 있으므로 꺼낼 때 이미 방문했는지 보고 버린다(`already visited`).
연결 요소는 세 덩어리, 위상 정렬은 `Intro to CS`부터 시작한다.

## 바꿔 보기

`dfs`에서 이웃을 **큰 번호부터** 넣도록 바꿔 보자. 그러면 작은 번호가 맨
위에 와서 방문 순서가 `0 1 2 3 4 5 6`이 된다. 재귀로 작은 번호부터 들어가는
DFS와 같은 순서다.

## 다음

다음 예제(Kruskal)부터는 간선에 **가중치**가 붙는다.
