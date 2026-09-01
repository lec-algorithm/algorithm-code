# 02. 퀵 정렬 (Quick Sort)

피벗 하나를 제자리에 놓고(파티션), 그 양쪽을 각각 정렬하는 정렬.
병합 정렬과 반대로 나눌 때 일하고 결합은 공짜다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `quickSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `quickSort.c` | C 구현 |
| `quick_sort.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-04-quick-sort/02_quick_sort
make -f /work/Makefile quickSort.out && ./quickSort.out
python3 quick_sort.py
```

- 결과

```console
before: 2 8 5 9 1 10 7 6 4 3
after : 1 2 3 4 5 6 7 8 9 10
comparisons = 25, moves = 21
```

두 구현이 같은 결과를 낸다. 같은 배열에서 병합 정렬은 비교 22회에
이동 68회였다. 퀵은 비교를 셋 더 쓰는 대신 이동이 3분의 1이고,
temp 배열도 없다.

## 바꿔 보기

배열을 이미 정렬된 `1 2 … 10`으로 바꾸면 비교가 45회로 뛴다.
첫 원소 피벗이 매번 최솟값이 되어 한쪽만 남기 때문이고, 이것이
퀵 정렬의 최악 O(n^2)이다. 삽입 정렬과 정반대라는 것에 주목하자.
파티션의 피벗을 `01_random`의 난수로 고르게 바꾸면 이 함정이 사라진다.

## 다음

[03_quick_selection](../03_quick_selection/)은 같은 파티션으로
정렬 없이 k번째 값을 찾습니다. 한쪽만 파면 O(n)이 됩니다.
