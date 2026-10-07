# 03. 분할 배낭 (Fractional Knapsack)

용량이 정해진 배낭에 가치를 최대로 담는 문제. 물건을 쪼갤 수 있다.
**단위 무게당 가치가 높은 물건부터** 담는다. 같은 절차를 의사코드
하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `fractionalKnapsack.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `fractionalKnapsack.c` | C 구현 |
| `fractional_knapsack.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-06-greedy/03_fractional_knapsack
make -f /work/Makefile fractionalKnapsack.out && ./fractionalKnapsack.out
python3 fractional_knapsack.py
```

- 결과

```console
capacity = 15
  take (w=10, v=60, v/w=6.0) x 1.00
  take (w=20, v=100, v/w=5.0) x 0.25
  total value = 85.00

capacity = 50
  take (w=10, v=60, v/w=6.0) x 1.00
  take (w=20, v=100, v/w=5.0) x 1.00
  take (w=30, v=120, v/w=4.0) x 0.67
  total value = 240.00

0/1 greedy, capacity = 50
  total value = 160
```

두 구현이 같은 결과를 낸다. 용량 15에서는 가장 알찬 물건(무게 10)을
통째로 담고, 남은 5만큼 다음 물건을 4분의 1 쪼개 담았다.

마지막 줄은 **쪼갤 수 없게** 바꾼 경우다. 같은 순서로 담으면 160인데,
무게 20과 30짜리를 담으면 220이다. 쪼갤 수 없으면 그리디가 최적을
놓친다. 그 문제(0/1 배낭)는 주제 14의 동적 계획법이 푼다.

## 바꿔 보기

기준을 "가치가 큰 물건부터"로 바꿔 보자. 용량 15에서 무게 30짜리를
절반 쪼개 담게 되어 60이 나온다. 단위 무게당 가치로 줄 세운 85보다
적다. 기준 하나가 답을 바꾼다.

## 다음

다음 예제(허프만 코드)는 그리디로 데이터를 압축한다.
