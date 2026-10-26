# 03. 선형 탐지 (Linear Probing)

연결 리스트 없이 배열 하나에 키를 직접 넣는다(개방형 주소 지정). 해시가
가리킨 칸이 차 있으면 **빈칸이 나올 때까지 오른쪽으로 한 칸씩** 간다. 검색도
같은 길을 따라가다 빈칸을 만나면 멈춘다. 지울 때는 같은 무리에서 뒤따르던
키들을 꺼냈다가 다시 넣는다. 같은 절차를 의사코드 하나에 C와 Python 두
구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `hashTableLP.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `hashTableLP.c` | C 구현 |
| `hash_table_lp.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-08-hashing/03_hash_table_lp
make -f /work/Makefile hashTableLP.out && ./hashTableLP.out
python3 hash_table_lp.py
```

- 결과

```console
M = 16, keys S E A R C H E X A M P L E

put S=0   hash  6  probes 1  slot  6  new
put E=1   hash 10  probes 1  slot 10  new
put A=2   hash  4  probes 1  slot  4  new
put R=3   hash 14  probes 1  slot 14  new
put C=4   hash  5  probes 1  slot  5  new
put H=5   hash  4  probes 4  slot  7  new
put E=6   hash 10  probes 1  slot 10  update
put X=7   hash 15  probes 1  slot 15  new
put A=8   hash  4  probes 1  slot  4  update
put M=9   hash  1  probes 1  slot  1  new
put P=10  hash 14  probes 3  slot  0  new
put L=11  hash  6  probes 3  slot  8  new
put E=12  hash 10  probes 1  slot 10  update

N = 10, M = 16, N/M = 0.6250
slot   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
key    P  M  -  -  A  C  S  H  L  -  E  -  -  -  R  X
value 10  9  -  -  8  4  0  5 11  - 12  -  -  -  3  7

get H: hash 4, probes 4 -> 5
get Y: hash 4, probes 6 -> not found
hits: 17 probes for 10 keys, 1.70 per key

resize to M = 32, N/M = 0.3125
slot   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
key    -  -  -  -  H  C  L  -  -  -  E  -  -  -  P  R
value  -  -  -  -  5  4 11  -  -  - 12  -  -  - 10  3
slot  16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
key    X  M  -  -  A  -  S  -  -  -  -  -  -  -  -  -
value  7  9  -  -  8  -  0  -  -  -  -  -  -  -  -  -
hits: 12 probes for 10 keys, 1.20 per key

naive delete C: just empty slot 5
get H: hash 4, probes 2 -> not found

delete C: hash 5
  reinsert S: slot 6 -> 6
  reinsert H: slot 7 -> 5
  reinsert L: slot 8 -> 7
get H: hash 4, probes 2 -> 5
N = 9, M = 16
slot   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
key    P  M  -  -  A  H  S  L  -  -  E  -  -  -  R  X
value 10  9  -  -  8  5  0 11  -  - 12  -  -  -  3  7
```

두 구현이 같은 결과를 낸다. 키와 값은 02 예제와 같은 `SEARCHEXAMPLE`이고,
배열 크기는 16이다.

- `probes`는 들여다본 칸 수다. 마지막에 만난 빈칸까지 센다. `H`는 해시가 4인데
  4, 5, 6번이 차 있어 7번 칸에 들어갔다. 4번에서 8번까지 다섯 칸이 붙어 있는
  덩어리가 **키 무리**(cluster)다.
- 없는 키 `Y`도 해시가 4라서, 무리를 끝까지 지나 9번 빈칸을 보고서야 없다는
  것을 안다. 6칸이다.
- 같은 키 10개를 크기 32에 다시 넣으면 적재율이 0.625에서 0.3125로 내려가고,
  모든 키를 한 번씩 찾는 데 드는 칸 수가 17에서 12로 준다.
- `C`가 있던 5번 칸만 비우면 `H`를 찾는 길이 5번에서 끊겨 `not found`가 된다.
  그래서 `delete`는 뒤따르던 `S`, `H`, `L`을 꺼냈다가 다시 넣는다. `H`는 비어
  있는 5번으로 당겨 들어온다.

`put`은 적재율이 0.7을 넘으면 배열을 두 배로 늘리고, `delete`는 8분의 1
이하로 비면 절반으로 줄인다. 이 실행에서는 둘 다 일어나지 않는다.

## 바꿔 보기

`createTable(16)`을 `createTable(11)`로 바꿔 보자. 칸이 빠듯해 무리가 금방
이어지고, `X`는 칸 4개를 들여다본 뒤에야 자리를 얻는다. 키 8개가 들어가 적재율이
0.7을 넘으면 `P`를 넣기 직전에 배열이 22칸으로 늘어난다. `P`부터 `slot` 번호가
10을 넘는 것이 그 표시다. 거꾸로 `delete`에서 다시 넣는
반복문을 지우고 `H`를 찾아보면, 잘못된 삭제가 무엇을 깨뜨리는지 보인다.

## 다음

해시 테이블은 순서를 버리고 속도를 얻는다. 키의 순서가 필요한 일(가장 작은
키, 범위 검색)은 주제 07의 이진 탐색 트리가 맡는다.
