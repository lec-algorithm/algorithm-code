# 07. A* 알고리즘 (A Star)

출발점 하나에서 **목표 하나**까지 가는 최단 경로를 찾는다. Dijkstra처럼
힙에서 하나씩 꺼내되, 기준이 지금까지 온 거리 `g`가 아니라 거기에 목표까지
남은 거리의 추정값 `h`(휴리스틱)를 더한 `f = g + h`다. 그래서 목표 쪽
정점부터 연다. 꺼낸 정점은 **닫힌 목록**(closed)에, 힙에 든 후보는 **열린
목록**(open)에 있다.

이 예제의 `h`는 실제 남은 거리를 넘지 않지만 간선 두 곳에서 일관성이
깨진다. 닫힌 정점을 다시 열지 않으면 26짜리 경로를 답으로 내고, 다시 열면
Dijkstra와 같은 25를 낸다. 두 경우를 차례로 돌린다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `aStar.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `aStar.c` | C 구현. 힙은 04_min_heap과 같은 절차로 직접 만든다 |
| `a_star.py` | Python 구현. 힙은 표준 모듈 `heapq`를 쓴다 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/07_a_star
make -f /work/Makefile aStar.out && ./aStar.out
python3 a_star.py
```

- 결과

```console
V = 8, E = 16, start 0, goal 6
h: 0=14 1=10 2=4 3=2 4=11 5=6 6=0 7=8
inconsistent edges: 4->5 (11 > 4 + 6) 5->2 (6 > 1 + 4)

without reopening closed nodes
pop 0 (f 14 = 0 + 14): close
  1: g 5, f 15
  4: g 9, f 20
  7: g 8, f 16
pop 1 (f 15 = 5 + 10): close
  2: g 17, f 21
  3: g 20, f 22
  7: g 9 >= 8, keep
pop 7 (f 16 = 8 + 8): close
  2: g 15, f 19
  5: g 14, f 20
pop 2 (f 19 = 15 + 4): close
  3: g 18, f 20
  6: g 26, f 26
pop 4 (f 20 = 9 + 11): close
  5: g 13, f 19
  6: g 29 >= 26, keep
pop 5 (f 19 = 13 + 6): close
  2: g 14 < 15, but 2 is closed
  6: g 26 >= 26, keep
pop 5 (f 20): skip, already closed
pop 3 (f 20 = 18 + 2): close
  6: g 27 >= 26, keep
pop 2 (f 21): skip, already closed
pop 3 (f 22): skip, already closed
pop 6 (f 26 = 26 + 0): goal
expanded: 0 1 7 2 4 5 3
path: 0 7 2 6, cost 26

with reopening
pop 0 (f 14 = 0 + 14): close
  1: g 5, f 15
  4: g 9, f 20
  7: g 8, f 16
pop 1 (f 15 = 5 + 10): close
  2: g 17, f 21
  3: g 20, f 22
  7: g 9 >= 8, keep
pop 7 (f 16 = 8 + 8): close
  2: g 15, f 19
  5: g 14, f 20
pop 2 (f 19 = 15 + 4): close
  3: g 18, f 20
  6: g 26, f 26
pop 4 (f 20 = 9 + 11): close
  5: g 13, f 19
  6: g 29 >= 26, keep
pop 5 (f 19 = 13 + 6): close
  2: g 14 < 15, reopen, f 18
  6: g 26 >= 26, keep
pop 2 (f 18 = 14 + 4): close
  3: g 17, f 19
  6: g 25, f 25
pop 3 (f 19 = 17 + 2): close
  6: g 26 >= 25, keep
pop 5 (f 20): skip, already closed
pop 3 (f 20): skip, already closed
pop 2 (f 21): skip, already closed
pop 3 (f 22): skip, already closed
pop 6 (f 25 = 25 + 0): goal
expanded: 0 1 7 2 4 5 2 3
path: 0 4 5 2 6, cost 25
```

두 구현이 같은 결과를 낸다. 갈림길은 정점 5를 닫을 때다. 4를 거쳐 온 5에서
2로 가면 14로, 7을 거쳐 온 15보다 짧다. 그런데 2는 이미 닫혔다. 다시 열지
않으면 2의 `g`가 15로 굳고, 6은 0→7→2→6, **26**으로 끝난다. 다시 열면
2를 한 번 더 꺼내 6을 25로 고치고 0→4→5→2→6, **25**에 닿는다.

원인은 `inconsistent edges` 줄에 있다. `h(4) = 11`인데 4에서 5로 4만큼
가면 `h(5) = 6`이라, 한 걸음 사이에 남은 거리 추정이 4보다 많이 줄었다.

## 바꿔 보기

`h`를 모두 0으로 바꿔 보자. `f = g`가 되어 Dijkstra와 같은 순서로 꺼낸다.
반대로 `h`를 실제 남은 거리(`0=25 1=22 2=11 3=9 4=16 5=12 6=0 7=18`)로
바꾸면 최단 경로 위의 정점만 닫고 목표에 닿는다.

## 다음

주제 12는 여기까지다. 주제 14(동적 계획법)에서 길찾기를 표를 채우는
방식으로 다시 푼다.
