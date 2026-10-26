# 02. 개별 체이닝 (Separate Chaining)

크기 M인 배열의 칸마다 연결 리스트를 두고, 해시값이 같은 키들을 한
리스트에 매단다. 해시로 칸을 찾고(1단계), 그 칸의 리스트를 순차 탐색한다
(2단계). 새 키는 리스트 맨 앞에 붙이고, 이미 있는 키는 값만 바꾼다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `hashTableSC.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `hashTableSC.c` | C 구현 |
| `hash_table_sc.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-08-hashing/02_hash_table_sc
make -f /work/Makefile hashTableSC.out && ./hashTableSC.out
python3 hash_table_sc.py
```

- 결과

```console
M = 5, keys S E A R C H E X A M P L E

put S=0   hash 2  compares 0  new     [2] S0
put E=1   hash 0  compares 0  new     [0] E1
put A=2   hash 0  compares 1  new     [0] A2 -> E1
put R=3   hash 4  compares 0  new     [4] R3
put C=4   hash 4  compares 1  new     [4] C4 -> R3
put H=5   hash 4  compares 2  new     [4] H5 -> C4 -> R3
put E=6   hash 0  compares 2  update  [0] A2 -> E6
put X=7   hash 2  compares 1  new     [2] X7 -> S0
put A=8   hash 0  compares 1  update  [0] A8 -> E6
put M=9   hash 4  compares 3  new     [4] M9 -> H5 -> C4 -> R3
put P=10  hash 3  compares 0  new     [3] P10
put L=11  hash 3  compares 1  new     [3] L11 -> P10
put E=12  hash 0  compares 2  update  [0] A8 -> E12

N = 10, M = 5, N/M = 2.0
[0] A8 -> E12
[1] -
[2] X7 -> S0
[3] L11 -> P10
[4] M9 -> H5 -> C4 -> R3

get E: hash 0, compares 2 -> 12
get Y: hash 2, compares 2 -> not found

delete E: hash 0, compares 2 -> deleted
N = 9
[0] A8
[1] -
[2] X7 -> S0
[3] L11 -> P10
[4] M9 -> H5 -> C4 -> R3
```

두 구현이 같은 결과를 낸다. 키는 `SEARCHEXAMPLE`의 열세 글자이고, 값은
등장 순서(0부터)다.

- `E`는 세 번, `A`는 두 번 나온다. 두 번째부터는 리스트에서 같은 키를 찾아
  값만 바꾸므로(`update`) 저장된 키는 10개다.
- `compares`는 그 칸의 리스트에서 키를 몇 번 비교했는지다. 새 키는 리스트를
  끝까지 훑어 없음을 확인한 뒤 맨 앞에 붙는다. 4번 칸에 `M`을 넣을 때는
  이미 셋이 있어 3번 비교했다.
- 10개를 5칸에 넣었으니 리스트 길이의 평균은 `N/M = 2.0`이다. 없는 키 `Y`를
  찾을 때도 2번 칸의 리스트 둘만 보고 끝난다.

키마다 붙은 정수 코드(`CODES`)는 이 예제에서 글자마다 정해 둔 값이다. 실제로는 01 예제의 문자열 해시 함수가 이 자리를 맡는다.

## 바꿔 보기

`createTable(5)`의 5를 1로 바꿔 보자. 모든 키가 한 리스트에 매달려 해시
테이블이 연결 리스트 하나가 된다. 반대로 크게(예: 97) 잡으면 비교 횟수는
거의 1이지만 빈 칸이 대부분이다. M은 시간과 메모리 사이의 저울추다.

## 다음

다음 예제(선형 탐지)는 리스트 없이, 충돌하면 배열의 다음 칸으로 간다.
