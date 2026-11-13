# 08. union-find로 하는 Kruskal

`03_mst_kruskal`은 덩어리 번호를 `comp` 배열에 적고, 합칠 때마다 배열 전체를
고쳐 쓴다. 여기서는 정점마다 **부모 하나**만 둔다. 부모를 따라 올라가 닿는
**뿌리**가 그 덩어리의 이름이다. 두 끝의 뿌리가 다르면 넣고, 같으면 사이클이다.

- **크기로 합치기**(union by size): 작은 트리를 큰 트리의 뿌리 밑에 단다.
  크기가 같으면 번호가 큰 뿌리를 작은 뿌리 밑에 단다.
- **경로 압축**(path compression): `Find`가 지나온 정점을 뿌리에 바로 붙인다.

뒷부분은 무작위 큰 그래프에서 `comp` 배열이 훑는 칸 수와 union-find가 따라간
링크 수를 센다. 시간이 아니라 세는 값이라 C와 Python의 출력이 같다.

| 파일 | 내용 |
| --- | --- |
| `unionFind.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `unionFind.c` | C 구현 |
| `union_find.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/08_union_find
make -f /work/Makefile unionFind.out && ./unionFind.out
python3 union_find.py
```

- 결과

```console
V = 8, E = 16, union by size + path compression
edge      find u    find v    result
0-7 0.16  0         7         add   7 under 0  parent [0 1 2 3 4 5 6 0]
2-3 0.17  2         3         add   3 under 2  parent [0 1 2 2 4 5 6 0]
1-7 0.19  1         7>0       add   1 under 0  parent [0 0 2 2 4 5 6 0]
0-2 0.26  0         2         add   2 under 0  parent [0 0 0 2 4 5 6 0]
5-7 0.28  5         7>0       add   5 under 0  parent [0 0 0 2 4 0 6 0]
1-3 0.29  1>0       3>2>0     skip  root 0     parent [0 0 0 0 4 0 6 0]
1-5 0.32  1>0       5>0       skip  root 0
2-7 0.34  2>0       7>0       skip  root 0
4-5 0.35  4         5>0       add   4 under 0  parent [0 0 0 0 0 0 6 0]
1-2 0.36  1>0       2>0       skip  root 0
4-7 0.37  4>0       7>0       skip  root 0
0-4 0.38  0         4>0       skip  root 0
6-2 0.40  6         2>0       add   6 under 0  parent [0 0 0 0 0 0 0 0]
7 edges = V - 1, stop
mst: 0-7 2-3 1-7 0-2 5-7 4-5 6-2
total weight = 1.81
links followed = 16 (comp array: 7 merges x 8 cells = 56)

random connected graphs, E = 4V, seed 2026
        V       comp cells     finds     links links/find
    1,000          999,000     7,040     6,447       0.92
   10,000       99,990,000    78,228    72,834       0.93
  100,000    9,999,900,000   788,952   737,258       0.93
```

## 읽는 법

- `find u`, `find v`는 두 끝에서 뿌리까지 올라간 경로다. `7>0`은 7의 부모가 0이고
  0이 뿌리라는 뜻이다.
- 넣은 간선 7개와 총 가중치 1.81은 `03_mst_kruskal`과 같다. 바뀐 것은 덩어리를
  들고 있는 방법뿐이다.
- `1-3` 줄에서 3은 `3>2>0`으로 두 칸을 올라갔다. 경로 압축이 3을 0에 바로 붙여서
  `parent[3]`이 2에서 0으로 바뀌었다. 그 뒤로 3은 한 칸이면 뿌리에 닿는다.
- 링크를 따라간 횟수는 모두 16번이다. `comp` 배열은 합칠 때마다 8칸을 훑어 56칸을 본다.

뒤의 표에서 `comp cells`는 합치기 V-1번이 저마다 V칸을 훑는 수, `links`는 Find가
따라간 부모 링크의 합이다. V가 10배가 되면 `comp` 배열은 100배가 되고, union-find의
Find 한 번은 V와 상관없이 평균 한 칸이 안 된다. 그래서 Kruskal 전체가 정렬
`O(E log E)`에 묶인다.

## 바꿔 보기

- `unite`에서 크기 비교를 빼고 늘 `parent[b] = a`로 달아 보자. 한 줄로 늘어선
  트리가 생기면 `links`가 얼마나 느는지 본다.
- `find`에서 경로 압축 반복문을 지워 보자. 두 기법이 각각 얼마나 기여하는지 갈라 볼 수 있다.

## 다음

다음 예제(`09_mst_timing`)는 Kruskal과 Prim의 시간을 희소 그래프와 완전 그래프에서 잰다.
