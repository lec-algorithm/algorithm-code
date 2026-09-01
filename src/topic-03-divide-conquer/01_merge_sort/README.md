# 01. 병합 정렬 (Merge Sort)

배열을 반으로 나눠 각각 정렬하고, 정렬된 두 절반을 하나로 합치는 정렬.
분할 정복(divide and conquer)의 대표 예다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `mergeSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `mergeSort.c` | C 구현 |
| `merge_sort.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-03-divide-conquer/01_merge_sort
make -f /work/Makefile mergeSort.out && ./mergeSort.out
python3 merge_sort.py
```

- 결과

```console
before: 2 8 5 9 1 10 7 6 4 3
after : 1 2 3 4 5 6 7 8 9 10
comparisons = 22, moves = 68
```

두 구현이 같은 결과를 낸다.

## 바꿔 보기

배열을 이미 정렬된 `1 2 … 10`으로 바꾸면 비교가 19회, 역순이면 15회다.
어느 쪽이든 이동은 68회로 같다. 병합마다 구간 전체가 temp로 갔다가
돌아오기 때문에, 이동 횟수는 입력과 무관하게 정해져 있다.
`merge`의 `<=`를 `<`로 바꾸면 정답은 그대로지만 안정성이 깨진다.

## 다음

[02_counting_sort](../02_counting_sort/)는 원소를 한 번도 비교하지 않고
정렬합니다. 비교 정렬의 하한 Ω(n log n)이 왜 "비교 기반"이라는 조건을
달고 있는지 보여줍니다.
