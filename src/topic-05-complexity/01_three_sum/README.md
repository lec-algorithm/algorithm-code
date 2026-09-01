# 01. 1-sum · 2-sum · 3-sum (연산 세기)

실행 시간을 재는 대신 대표 연산(배열 접근)을 세서 수학적 모델을
만드는 예제. n을 두 배로 하면 접근이 몇 배가 되는지(배가 실험)가
차수를 알려준다. 입력은 주제 04의 minstd 생성기로 만들어 C와
Python이 같은 배열을 본다.

| 파일 | 내용 |
| --- | --- |
| `threeSum.pseudo` | 기준이 되는 절차와 비용 모델 |
| `threeSum.c` | C 구현 |
| `three_sum.py` | Python 구현 |
| `doubling_time.py` | 배가 실험을 실제 시간으로 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-05-complexity/01_three_sum
make -f /work/Makefile threeSum.out && ./threeSum.out
python3 three_sum.py
```

- 결과

```console
N = 100
1-sum: count = 1, accesses = 100
2-sum: count = 19, accesses = 9900
3-sum: count = 642, accesses = 485100

doubling N: accesses ratio
     N        2-sum        3-sum
   200         4.02         8.12
   400         4.01         8.06
```

두 구현이 같은 결과를 낸다. 접근 횟수는 이론과 정확히 일치한다.
1-sum은 n, 2-sum은 n(n-1), 3-sum은 3·C(n,3)이다. n을 두 배로 하면
2-sum은 약 4배(2차), 3-sum은 약 8배(3차)가 된다.

## 시간으로도 재 보기

`doubling_time.py`가 같은 실험을 실제 시간으로 돌린다. 측정값은
기계마다 다르지만 비율은 4와 8 근처로 모인다. Python에서 N = 1600의
3-sum은 수십 초가 걸린다. 그 기다림 자체가 3차의 체감이다.

## 바꿔 보기

3-sum에 `if (a[i] + a[j] > 100) continue;` 같은 가지치기를 넣어도
비율은 여전히 8 근처다. 상수는 줄어도 차수는 그대로라는 것이
보인다. 반대로 2-sum을 "정렬 후 양끝 포인터"로 다시 짜면 차수
자체가 내려간다. 그 이야기는 다음 주제들에서 이어진다.

## 다음

이 주제의 강의 자료에서 이 숫자들이 점근 분석과 표기법으로
이어집니다.
