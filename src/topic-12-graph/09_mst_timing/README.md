# 09. 최소 스패닝 트리 시간 재기

Kruskal 두 가지와 Prim 두 가지의 시간을 세 종류의 그래프에서 잰다. 값은 밀리초다.
값은 컴퓨터마다 다르므로 비율을 본다. 네 방법이 낸 총 가중치가 같은지도 확인한다.

| 방법 | 절차 | 시간 |
| --- | --- | --- |
| Kruskal comp | 정렬 + `comp` 배열 합치기 (`03_mst_kruskal`) | `O(E log E + V^2)` |
| Kruskal UF | 정렬 + union-find (`08_union_find`) | `O(E log E)` |
| Prim heap | 최소 힙 (`05_mst_prim`) | `O(E log V)` |
| Prim array | 힙 없이 key 배열 V칸을 훑어 고른다 | `O(V^2 + E)` |

| 그래프 | 만드는 법 |
| --- | --- |
| 희소 | E = 4V. 정점마다 앞의 정점 하나에 이어 연결을 보장하고 나머지는 무작위 |
| 완전 | 모든 쌍을 잇는다. E = V(V-1)/2, 가중치는 무작위 |
| 완전, 힙에 가장 나쁜 경우 | u < v인 간선의 가중치가 (V - u) x V + v. 정점을 붙일 때마다 남은 모든 key가 준다 |

| 파일 | 내용 |
| --- | --- |
| `mstTiming.pseudo` | 재는 절차와 Prim array |
| `mstTiming.c` | C 구현. 희소는 V = 32,000, 완전은 V = 2,000까지 |
| `mst_timing.py` | Python 구현. 크기를 줄였다 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/09_mst_timing
make -f /work/Makefile mstTiming.out && ./mstTiming.out
python3 mst_timing.py
```

- 결과 (C, 한 예)

```console
sparse, E = 4V (ms)
      V         E  Kruskal comp  Kruskal UF  Prim heap  Prim array
   2000      8000         2.968       0.435      0.142       2.287
   8000     32000        34.831       2.106      0.955      35.513
  32000    128000       517.287      10.024      5.184     555.430

complete, E = V(V-1)/2 (ms)
      V         E  Kruskal comp  Kruskal UF  Prim heap  Prim array
    500    124750         9.306       9.148      0.256       0.491
   1000    499500        41.711      41.022      0.965       2.106
   2000   1999000       183.393     180.328      3.413       9.270

complete, every edge lowers a key (ms)
      V         E  Kruskal comp  Kruskal UF  Prim heap  Prim array
    500    124750         2.984       3.002      5.445       0.233
   1000    499500        13.145      13.253     25.066       0.833
   2000   1999000        57.362      56.097    109.677       3.222
```

시간을 재는 예제라 두 구현의 출력 숫자는 같지 않다. 입력은 시드가 고정된 같은 값이다.

## 읽는 법

- **희소 그래프**: V가 4배가 될 때 Kruskal comp와 Prim array는 15배쯤 는다. 둘 다
  `V^2`에 묶이기 때문이다. Kruskal comp는 합치기마다 V칸을, Prim array는 정점마다
  V칸을 훑는다. union-find와 힙으로 바꾼 두 방법은 V = 32,000에서 50배 이상 빠르다.
- **무작위 완전 그래프**: Prim heap이 가장 빠르다. 가중치가 무작위면 이미 붙은 정점이
  남은 정점의 key를 줄이는 일이 드물어, 힙에 넣는 횟수가 E보다 훨씬 적다.
  Kruskal은 간선 E개를 모두 정렬해야 해서 가장 느리다.
- **힙에 가장 나쁜 완전 그래프**: 간선마다 key가 줄어 힙에 E번 넣는다. 이제 Prim
  array가 V = 2,000에서 30배 넘게 빠르다. `O(V^2)`는 입력과 상관없이 지켜지는 상한이고, 완전
  그래프에서는 간선을 한 번씩 보는 비용과 같은 크기다.
- 그래서 밀집 그래프에는 Prim array가 안전한 선택이다. 희소 그래프에는 Kruskal UF나
  Prim heap을 쓴다. 간선이 이미 목록으로 주어지거나, 트리 대신 덩어리 여러 개(숲)가
  필요하면 Kruskal이 자연스럽다.

## 바꿔 보기

- 희소 그래프의 E를 `4 * n`에서 `n * 20`으로 늘려 보자. 어디쯤에서 Prim array가 다시 따라잡는가.
- `primHeap`에서 `inTree` 검사를 빼면 이미 붙은 정점을 다시 꺼내 총 가중치가 틀린다.
  확인 코드가 `different total weight`로 멈추는 것을 본다.
