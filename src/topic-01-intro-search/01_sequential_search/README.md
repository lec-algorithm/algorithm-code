# 01. 순차 검색 (Sequential Search)

정렬 여부와 무관하게 앞에서부터 하나씩 비교하는 가장 단순한 검색.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `sequentialSearch.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `sequentialSearch.c` | C 구현 |
| `sequential_search.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-01-intro-search/01_sequential_search
make -f /work/Makefile sequentialSearch.out && ./sequentialSearch.out
python3 sequential_search.py
```

- 결과

```console
n = 15, key = 51
index = 6, comparisons = 7
```

두 구현이 같은 결과를 낸다.

## 바꿔 보기

`key`를 바꾸면 비교 횟수가 어떻게 달라지는지 본다. 배열의 첫 값(`6`)이면 1회,
마지막 값(`101`)이면 15회, 없는 값이면 15회다.

## 다음

같은 배열을 [02_binary_search](../02_binary_search/)로 찾으면 비교가 4회입니다.
대신 배열이 정렬되어 있어야 합니다.
