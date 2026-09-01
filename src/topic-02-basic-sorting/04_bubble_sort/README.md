# 04. 버블 정렬 (Bubble Sort)

이웃한 두 원소를 비교해서 큰 것을 뒤로 보내는 정렬. 한 회전이 끝나면
가장 큰 원소가 맨 뒤에 도착한다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `bubbleSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `bubbleSort.c` | C 구현 |
| `bubble_sort.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-02-basic-sorting/04_bubble_sort
make -f /work/Makefile bubbleSort.out && ./bubbleSort.out
python3 bubble_sort.py
```

- 결과

```console
before: 6 8 5 9 10 1 7 2 4 3
after : 1 2 3 4 5 6 7 8 9 10
comparisons = 45, moves = 87
```

두 구현이 같은 결과를 낸다.

## 바꿔 보기

같은 배열에서 선택 정렬은 이동이 15회였는데 버블 정렬은 87회다.
비교는 45회로 같다. 이웃끼리만 교환하니 원소가 한 칸씩밖에 못 가서다.
한 회전에서 교환이 없으면 바로 멈추는 flag를 넣어 보자. 이미 정렬된
입력이면 비교가 45회에서 9회로 줄어든다.

## 다음

네 정렬 모두 최악이 O(n^2)입니다. 다음 주제(분할 정복과 머지 정렬)에서
O(n log n) 정렬을 만듭니다.
