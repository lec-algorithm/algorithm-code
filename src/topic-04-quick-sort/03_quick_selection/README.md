# 03. 퀵 선택 (Quick Selection)

배열을 다 정렬하지 않고 k번째로 작은 값을 찾는 알고리즘.
퀵 정렬과 같은 파티션을 쓰되, 답이 있는 한쪽만 파고든다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `quickSelection.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `quickSelection.c` | C 구현 |
| `quick_selection.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-04-quick-sort/03_quick_selection
make -f /work/Makefile quickSelection.out && ./quickSelection.out
python3 quick_selection.py
```

- 결과

```console
before: 2 8 5 9 1 10 7 6 4 3
after : 1 2 3 4 5 10 7 6 9 8
k = 5 -> value = 5
comparisons = 16, moves = 15
```

두 구현이 같은 결과를 낸다. **after 줄을 보자.** 앞쪽 다섯 개만
자리를 잡았고 뒤는 뒤섞여 있다. 다 정렬하지 않고도 답이 나왔다.
같은 배열을 퀵 정렬로 다 정렬하면 비교 25회, 선택은 16회다.

## 바꿔 보기

`k`를 1로 바꾸면 파티션 한 번(비교 9회)으로 끝난다. 10으로 바꾸면
20회다. 어느 쪽이든 다 정렬하는 것보다 싸다.
특정 "값"을 찾는 데는 쓰지 말자. 그건 검색(주제 01)의 일이고,
퀵 선택은 "몇 번째"를 찾는 도구다.

## 다음

다음 주제(알고리즘 성능 분석)에서 지금까지의 O 표기를 수학으로
단단하게 만듭니다.
