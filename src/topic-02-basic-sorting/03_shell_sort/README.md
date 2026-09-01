# 03. 셸 정렬 (Shell Sort)

gap 간격으로 떨어진 원소들끼리 먼저 삽입 정렬해 두고, gap을 줄여 가며
반복하는 정렬. 마지막 gap은 1, 즉 보통의 삽입 정렬이다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `shellSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `shellSort.c` | C 구현 |
| `shell_sort.py` | Python 구현 |
| `performance_comparison.py` | 삽입 정렬과의 시간 비교 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-02-basic-sorting/03_shell_sort
make -f /work/Makefile shellSort.out && ./shellSort.out
python3 shell_sort.py
```

- 결과

```console
before: 6 8 5 9 10 1 7 2 4 3
after : 1 2 3 4 5 6 7 8 9 10
comparisons = 29, moves = 57
```

두 구현이 같은 결과를 낸다.

## 성능 비교

원소 10개로는 차이가 보이지 않는다. `performance_comparison.py`가
배열을 키워 가며 삽입 정렬과 시간을 비교한다. 측정값은 기계마다 다르다.

## 바꿔 보기

gap 수열을 바꾸면 성능이 달라진다. 지금은 절반씩 줄이지만(10개면 5, 2, 1),
Knuth 수열(1, 4, 13, 40, …)을 쓰면 최악이 O(n^1.5)로 좋아진다.
`shell_sort.py`의 gap 계산을 바꿔서 비교 횟수가 어떻게 변하는지 본다.

## 다음

[04_bubble_sort](../04_bubble_sort/)는 이웃끼리만 비교하는 가장 단순한
정렬입니다. 왜 잘 쓰이지 않는지 숫자로 확인합니다.
