# 01. 폭 우선 검색 (Breadth-First Search)

간선 목록으로 받은 그래프를 **인접 행렬**과 **인접 목록**으로 바꿔 찍고,
정점 0에서 BFS를 돌린다. 큐(FIFO)에서 꺼낼 때마다 본 이웃과 큐의 내용을
한 줄씩 찍는다. 끝나면 정점마다 어디서 왔는지(`from`)와 간선 몇 개
거리인지(`dist`)가 남고, 그것을 거슬러 올라가면 최단 경로가 나온다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `bfs.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `bfs.c` | C 구현 |
| `bfs.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/01_bfs
make -f /work/Makefile bfs.out && ./bfs.out
python3 bfs.py
```

- 결과

```console
V = 7, E = 9

adjacency matrix
    0 1 2 3 4 5 6
  0 0 1 1 0 0 1 1
  1 1 0 0 0 0 0 0
  2 1 0 0 1 0 0 0
  3 0 0 1 0 1 1 0
  4 0 0 0 1 0 1 1
  5 1 0 0 1 1 0 0
  6 1 0 0 0 1 0 0

adjacency list
  0: 1 2 5 6
  1: 0
  2: 0 3
  3: 2 4 5
  4: 3 5 6
  5: 0 3 4
  6: 0 4

BFS(0)
visit 0, queue [0]
dequeue 0: check 1 2 5 6 -> visit 1 2 5 6, queue [1 2 5 6]
dequeue 1: check 0, queue [2 5 6]
dequeue 2: check 0 3 -> visit 3, queue [5 6 3]
dequeue 5: check 0 3 4 -> visit 4, queue [6 3 4]
dequeue 6: check 0 4, queue [3 4]
dequeue 3: check 2 4 5, queue [4]
dequeue 4: check 3 5 6, queue []
order: 0 1 2 5 6 3 4

  v  from  dist
  0     -     0
  1     0     1
  2     0     1
  3     2     2
  4     5     2
  5     0     1
  6     0     1

path 0 -> 4: 0 5 4 (2 edges)
```

두 구현이 같은 결과를 낸다. 같은 그래프가 행렬로는 49칸, 목록으로는
이웃 18개(간선 9개를 양 끝에서 한 번씩)가 된다. 방문 순서는 거리 1인
1, 2, 5, 6을 모두 꺼낸 뒤에야 거리 2인 3, 4로 넘어간다. `dist` 열이
그대로 0에서 각 정점까지의 최단 경로 길이(간선 수)다.

## 바꿔 보기

`adj`를 오름차순으로 정렬하는 줄을 지우고 간선 순서를 바꿔 보자. 방문
순서와 `from`은 달라질 수 있지만 `dist` 열은 바뀌지 않는다. 같은 층
안의 순서만 달라지기 때문이다.

## 다음

다음 예제(DFS)는 큐 대신 **스택**을 써서 같은 그래프를 깊이 먼저 훑는다.
