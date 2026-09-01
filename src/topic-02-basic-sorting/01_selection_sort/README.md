# 01. 선택 정렬 (Selection Sort)

남은 원소 중 최솟값을 골라 정렬된 앞부분의 끝에 붙이는 정렬.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `selectionSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `selectionSort.c` | C 구현 |
| `selection_sort.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-02-basic-sorting/01_selection_sort
make -f /work/Makefile selectionSort.out && ./selectionSort.out
python3 selection_sort.py
```

- 결과

```console
before: 6 8 5 9 10 1 7 2 4 3
after : 1 2 3 4 5 6 7 8 9 10
comparisons = 45, moves = 15
```

두 구현이 같은 결과를 낸다.

## 바꿔 보기

배열을 이미 정렬된 `1 2 … 10`으로 바꿔도 비교는 그대로 45회다.
어떤 입력이든 비교 횟수가 n(n-1)/2로 같다는 것이 선택 정렬의 특징이다.
대신 교환이 한 번도 일어나지 않아 이동은 0회가 된다.

## 다음

[02_insertion_sort](../02_insertion_sort/)는 정렬된 입력이면 비교를 9회로
줄입니다. 입력에 따라 비교 횟수가 달라지는 첫 정렬입니다.
