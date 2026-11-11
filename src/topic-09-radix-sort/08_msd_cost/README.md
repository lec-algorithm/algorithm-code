# 08. LSD와 MSD의 비용 세기

자리 읽기, 호출, 칸 훑기(호출 x k), 삽입 비교를 센다. 기수 `k`와, 삽입 정렬로 넘기는
구간 크기(cutoff)를 바꿔 가며 LSD와 MSD를 견준다. 문자열 MSD는 읽은 글자를 센다.

| 파일 | 내용 |
| --- | --- |
| `msdCost.pseudo` | 기준이 되는 절차 |
| `msdCost.c` | C 구현 |
| `msd_cost.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-09-radix-sort/08_msd_cost
make -f /work/Makefile msdCost.out && ./msdCost.out
python3 msd_cost.py
```

- 결과 (일부)

```console
1000 random values, 965354 to 4290781594
k = 10
  LSD      reads 10000  calls  10  scans    100  ins     0  total  10100
  MSD      reads  4158  calls 453  scans   4530  ins     0  total   8688  [1 + 5 + 43 + 282 + 103 + 15 + 4]
k = 256
  LSD      reads  4000  calls   4  scans   1024  ins     0  total   5024
  MSD      reads  2001  calls 242  scans  61952  ins     0  total  63953  [1 + 231 + 10]
k = 256
  LSD      reads  4000  calls   4  scans   1024  ins     0  total   5024
  MSD <=16 reads  1000  calls   1  scans    256  ins  1508  total   2764  [1]
```

두 구현이 같은 결과를 낸다. `[ ]` 안은 자리별 호출 수다. `MSD <=16`은 크기 16 이하의 구간을
삽입 정렬로 넘긴 것이다. 합계는 세 횟수를 더한 값으로, 시간이 아니라 규모를 보는 값이다.

## 바꿔 보기

`main`의 입력이나 `cases`의 `k`, `cutoff`를 바꿔 보자. `k = 256`에서 `cutoff`를 32까지 올리면
호출은 줄지만 삽입 비교가 늘어난다.
