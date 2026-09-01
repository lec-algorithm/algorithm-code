# 02. 카운팅 정렬 (Counting Sort)

값을 비교하지 않고, 값마다 몇 개 있는지 세서 각 원소의 자리를 계산하는
정렬. 키가 0 이상 k 이하의 정수일 때만 쓸 수 있다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `countingSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `countingSort.c` | C 구현 |
| `counting_sort.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-03-divide-conquer/02_counting_sort
make -f /work/Makefile countingSort.out && ./countingSort.out
python3 counting_sort.py
```

- 결과

```console
before: 1 0 1 5 4 3 1 4 5 2 1
after : 0 1 1 1 1 2 3 4 4 5 5
comparisons = 0, moves = 11
```

두 구현이 같은 결과를 낸다. **비교가 0회**라는 것이 이 예제의 요점이다.

## 바꿔 보기

3단계의 `for i <- n-1 downto 0`을 앞에서부터(`0 to n-1`)로 바꿔 보자.
정렬 결과는 같지만 같은 값끼리의 순서가 뒤집힌다. 값에 이름표를 붙여
찍어 보면 안정성이 깨진 것이 보인다. `MAX_KEY`를 1000000으로 올리면
원소 11개를 정렬하는 데 카운팅 배열 백만 칸이 필요해진다. O(n + k)의
k가 무엇을 뜻하는지 보인다.

## 다음

다음 주제(랜덤과 퀵 정렬)에서 비교 정렬로 돌아갑니다. 평균 O(n log n)을
상수까지 빠르게 만드는 길입니다.
