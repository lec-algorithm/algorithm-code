# 04. 최소 힙 (Min Heap)

**우선순위 큐**(priority queue)는 넣은 순서가 아니라 우선순위 순서로 꺼내는
큐다. 그 바탕이 **최소 힙**이다. 완전 이진 트리를 배열에 담고 "부모 값 ≤
자식 값"을 지킨다. 그러면 루트가 언제나 최솟값이다. 넣을 때는 맨 끝에 두고
위로 올리고(sift up), 꺼낼 때는 마지막 원소를 루트로 옮겨 아래로
내린다(sift down).
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `minHeap.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `minHeap.c` | C 구현 |
| `min_heap.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-12-graph/04_min_heap
make -f /work/Makefile minHeap.out && ./minHeap.out
python3 min_heap.py
```

- 결과

```console
heap [1 2 3 4 5 6 7 8 9]

insert 10 at index 9
  10 (index 9) vs parent 5 (index 4): stay
heap [1 2 3 4 5 6 7 8 9 10]

insert 0 at index 10
  0 (index 10) vs parent 5 (index 4): swap
  0 (index 4) vs parent 2 (index 1): swap
  0 (index 1) vs parent 1 (index 0): swap
  reached the root
heap [0 1 3 4 2 6 7 8 9 10 5]

extract min 0, move last 5 to the root
  5 (index 0) vs children 1 (index 1), 3 (index 2): swap with 1
  5 (index 1) vs children 4 (index 3), 2 (index 4): swap with 2
  5 (index 4) vs children 10 (index 9): stay
heap [1 2 3 4 5 6 7 8 9 10]

extract all: 1 2 3 4 5 6 7 8 9 10
```

두 구현이 같은 결과를 낸다. 10은 부모 5보다 크니 제자리에 머문다. 0은
부모와 세 번 바꿔 루트까지 올라간다. 원소가 11개인 트리의 높이가 3이니
아무리 많이 바꿔도 세 번이다. 꺼낼 때는 루트 자리에 온 5가 더 작은 자식
1, 2와 차례로 자리를 바꾸며 내려간다. 남은 것을 다 꺼내면 오름차순으로
나온다. 이것이 힙 정렬이다.

## 바꿔 보기

비교 부호(`<`·`>=`)를 모두 뒤집어 **최대 힙**으로 바꿔 보자. 루트가
최댓값이 되고, 다 꺼내면 내림차순으로 나온다.

## 다음

다음 예제(Prim)는 이 힙을 우선순위 큐로 써서 MST를 정점 하나에서부터
키운다.
