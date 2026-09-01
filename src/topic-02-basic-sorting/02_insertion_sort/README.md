# 02. 삽입 정렬 (Insertion Sort)

이미 정렬된 앞부분에 새 원소를 끼워 넣는 정렬.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `insertionSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `insertionSort.c` | C 구현 |
| `insertion_sort.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-02-basic-sorting/02_insertion_sort
make -f /work/Makefile insertionSort.out && ./insertionSort.out
python3 insertion_sort.py
```

- 결과

```console
before: 6 8 5 9 10 1 7 2 4 3
after : 1 2 3 4 5 6 7 8 9 10
comparisons = 36, moves = 47
```

두 구현이 같은 결과를 낸다.

## 바꿔 보기

배열을 `1 2 … 10`으로 바꾸면 비교가 9회로 줄어든다. 선택 정렬은 같은
입력에서도 45회였다. 역순 `10 9 … 1`로 바꾸면 45회로 최악이 된다.
입력의 정렬 상태에 따라 최선과 최악이 갈리는 것이 삽입 정렬의 특징이다.

## 다음

삽입 정렬은 원소를 한 칸씩만 민다는 약점이 있습니다.
[03_shell_sort](../03_shell_sort/)는 멀리 떨어진 원소끼리 먼저 정렬해서
이 약점을 줄입니다.
