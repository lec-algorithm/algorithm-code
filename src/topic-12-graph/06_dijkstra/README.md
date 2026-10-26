# 06. Dijkstra 알고리즘 (Shortest Path)

가중치가 음수가 아닌 방향 그래프에서 출발점 하나로부터 모든 정점까지의
**최단 거리**를 구한다. 아직 확정하지 않은 정점 중 **지금 가장 가까운
것**을 힙에서 꺼내 거리를 확정하고, 그 정점에서 나가는 간선으로 이웃의
거리를 줄여 본다(완화, relaxation). Prim과 같은 틀에서 힙에 넣는 값만
"간선 하나의 가중치"에서 "출발점부터 쌓인 거리"로 바뀐다. 끝에는 음의
가중치 때문에 틀린 답이 나오는 작은 그래프를 돌린다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `dijkstra.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `dijkstra.c` | C 구현. 힙은 04_min_heap과 같은 절차로 직접 만든다 |
| `dijkstra.py` | Python 구현. 힙은 표준 모듈 `heapq`를 쓴다 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/06_dijkstra
make -f /work/Makefile dijkstra.out && ./dijkstra.out
python3 dijkstra.py
```

- 결과

```console
V = 8, E = 16, start 0
pop 0 (dist 0): done
  0->1: dist[1] = 5 via 0
  0->4: dist[4] = 9 via 0
  0->7: dist[7] = 8 via 0
pop 1 (dist 5): done
  1->2: dist[2] = 17 via 1
  1->3: dist[3] = 20 via 1
  1->7: 9 >= 8, keep
pop 7 (dist 8): done
  7->2: dist[2] = 15 via 7
  7->5: dist[5] = 14 via 7
pop 4 (dist 9): done
  4->5: dist[5] = 13 via 4
  4->6: dist[6] = 29 via 4
pop 5 (dist 13): done
  5->2: dist[2] = 14 via 5
  5->6: dist[6] = 26 via 5
pop 5 (dist 14): skip, already done
pop 2 (dist 14): done
  2->3: dist[3] = 17 via 2
  2->6: dist[6] = 25 via 2
pop 2 (dist 15): skip, already done
pop 2 (dist 17): skip, already done
pop 3 (dist 17): done
  3->6: 26 >= 25, keep
pop 3 (dist 20): skip, already done
pop 6 (dist 25): done
pop 6 (dist 26): skip, already done
pop 6 (dist 29): skip, already done

  v  dist  from
  0     0     -
  1     5     0
  2    14     5
  3    17     2
  4     9     0
  5    13     4
  6    25     2
  7     8     0
path 0 -> 6: 0 4 5 2 6 (25)

negative weight: 0->1 4, 0->3 2, 1->2 6, 2->3 -9
pop 0 (dist 0): done
  0->1: dist[1] = 4 via 0
  0->3: dist[3] = 2 via 0
pop 3 (dist 2): done
pop 1 (dist 4): done
  1->2: dist[2] = 10 via 1
pop 2 (dist 10): done
  2->3: 1 < 2, but 3 is already done
dijkstra says dist[3] = 2, but 0->1->2->3 costs 1
```

두 구현이 같은 결과를 낸다. 정점 2의 거리는 17, 15, 14로 세 번 줄었다.
처음 찾은 길(0→1→2)보다 돌아가는 길(0→4→5→2)이 더 짧았다. 0에서 6까지는
0→4→5→2→6, 거리 **25**다. 확정 순서는 0, 1, 7, 4, 5, 2, 3, 6으로 거리가
줄어들지 않는 순서다.

음의 가중치 그래프에서는 정점 3을 거리 2로 먼저 확정해 버린다. 뒤에
0→1→2→3이 1이라는 것을 찾지만 이미 확정했으므로 고치지 않는다.

## 바꿔 보기

`done`을 보지 않고 더 짧으면 언제나 고치도록 바꿔 보자. 음의 가중치
그래프에서 `dist[3]`이 1로 고쳐진다. 그러나 정점 3을 다시 처리하지 않으므로
3 뒤에 정점이 더 있으면 그쪽은 여전히 틀린다. 음의 가중치에는 모든 간선을
`V - 1`번 훑는 Bellman-Ford 알고리즘이 맞다.

## 다음

다음 예제(A*)는 같은 그래프에서 **목표까지 남은 거리의 추정값**을 더해
목표 쪽부터 찾는다.
