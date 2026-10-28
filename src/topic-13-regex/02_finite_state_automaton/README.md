# 02. 유한 상태 오토마타 (Finite State Automaton)

패턴 `ab*c`를 **상태 다섯 개와 전이**로 인식한다. 입력을 한 글자씩 읽으며
현재 상태를 옮기고, 입력이 끝났을 때 수락 상태(S4)에 있으면 수락한다.
S1에서 S2로는 입력 없이 넘어간다(ε 전이). 같은 절차를 의사코드 하나에 C와
Python 두 구현으로 담았다.

```plaintext
        'a'       ε        'b'            'c'
  S0 ───────> S1 ────> S2 ───────> S3 ───────> S4  (수락)
                        │          ↺ 'b'        ↑
                        └───────── 'c' ─────────┘
```

| 파일 | 내용 |
| --- | --- |
| `fsaSimple.pseudo` | 기준이 되는 절차와 상태·전이 목록. 두 구현은 이것을 옮긴 것이다 |
| `fsaSimple.c` | C 구현 |
| `fsa_simple.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-13-regex/02_finite_state_automaton
make -f /work/Makefile fsaSimple.out && ./fsaSimple.out
python3 fsa_simple.py
```

- 결과

```console
pattern ab*c

input "abc"
  start S0
  'a' -> S1
  eps -> S2
  'b' -> S3
  'c' -> S4
  final S4: accept

input "abbbbc"
  start S0
  'a' -> S1
  eps -> S2
  'b' -> S3
  'b' -> S3
  'b' -> S3
  'b' -> S3
  'c' -> S4
  final S4: accept

input "ac"
  start S0
  'a' -> S1
  eps -> S2
  'c' -> S4
  final S4: accept

input "ab"
  start S0
  'a' -> S1
  eps -> S2
  'b' -> S3
  final S3: reject

input "abd"
  start S0
  'a' -> S1
  eps -> S2
  'b' -> S3
  'd' -> no move from S3: reject

input "abcc"
  start S0
  'a' -> S1
  eps -> S2
  'b' -> S3
  'c' -> S4
  'c' -> no move from S4: reject

summary
  abc      accept
  abbbbc   accept
  ac       accept
  ab       reject
  abd      reject
  abcc     reject
```

두 구현이 같은 결과를 낸다. 입력 글자 하나에 전이가 하나씩이고, 앞의 선택으로
되돌아가는 일이 없다. 거부되는 경우는 둘이다.

- 갈 곳이 없는 글자를 만났을 때: `abd`의 `d`, `abcc`의 두 번째 `c`
- 입력이 끝났는데 수락 상태가 아닐 때: `ab`는 S3에서 끝난다.

## 바꿔 보기

- 패턴을 `ab*c*`로 바꾸려면 어느 상태를 수락 상태로 더하고 어떤 전이를
  더해야 할지 생각해 보고 `accept`를 고쳐 보자.
- `inputs`에 `aabc`를 넣어 보자. S0에서만 `a`를 받으므로 두 번째 `a`에서 거부된다.

## 다음

`03_nfa_regex`에서는 이런 오토마타를 손으로 그리지 않고 정규식에서 자동으로
만든다.
