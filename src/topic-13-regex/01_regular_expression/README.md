# 01. 정규식 매칭 (재귀와 백트래킹)

글자, `.`(아무 글자 하나), `x*`(x를 0번 이상)로 된 정규식을 **재귀로** 맞춰
보는 작은 엔진. `x*`를 만나면 x를 0개, 1개, 2개, ... 먹어 보며 나머지를 다시
시도하고, 실패하면 앞의 선택으로 되돌아간다(백트래킹). 같은 절차를 의사코드
하나에 C와 Python 두 구현으로 담았다. Python도 `re` 모듈을 쓰지 않는다.

| 파일 | 내용 |
| --- | --- |
| `regexMatch.pseudo` | 기준이 되는 절차(Match·MatchStar·Search). 두 구현은 이것을 옮긴 것이다 |
| `regexMatch.c` | C 구현 |
| `regex_match.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-13-regex/01_regular_expression
make -f /work/Makefile regexMatch.out && ./regexMatch.out
python3 regex_match.py
```

- 결과

```console
search("ab*c", "aabbc")
  start 0
    match("ab*c", "aabbc")
      match("b*c", "abbc")
          match("c", "abbc")
  start 1
    match("ab*c", "abbc")
      match("b*c", "bbc")
          match("c", "bbc")
          match("c", "bc")
          match("c", "c")
            match("", "")
  -> found at 1, comparisons = 9

tests
  pattern  text        at  comparisons
  ab*c     ac           0            4
  ab*c     abbbbc       0            8
  ab*c     abd         -1            7
  a.c      xxabcxx      2            6
  .*ab*c   zzabbbcz     0           10

blow-up: match((a*)^n b, a^n)
  n   result        comparisons  max depth
  2   no match               10          5
  4   no match              126          9
  6   no match             1716         13
  8   no match            24310         17
  10  no match           352716         21
```

두 구현이 같은 결과를 낸다. 첫 덩어리는 `match` 호출을 하나씩 찍은 것이고,
들여쓰기가 재귀 깊이다(`matchStar`도 한 단계로 센다). 시작 위치 0에서는
`b*`가 b를 하나도 먹지 못하고 `c`와 `a`가 맞지 않아 실패한다. 위치 1에서는
`b*`가 b를 0개, 1개, 2개 먹어 보다가 세 번째에 `c`가 맞는다.

마지막 표가 이 방식의 약점이다. `a*a*b`처럼 `a*`가 n번 이어지면 a를 어느
`a*`에 몇 개씩 나눠 줄지 모든 경우를 시도한 뒤에야 "맞지 않음"을 안다.
n이 2 늘 때마다 비교 횟수가 10배 넘게 는다.

## 바꿔 보기

- `main`의 첫 `search`를 `search("a.*c", "xabcbcy")`로 바꿔 호출 순서를 보자.
  `.*`가 글자를 하나씩 더 먹으며 같은 시도를 되풀이한다.
- 마지막 표의 `n`을 12까지 늘려 보자. 비교가 500만 번을 넘는다.

## 다음

`02_finite_state_automaton`에서는 같은 일을 상태와 전이로 한다. 되돌아가지 않는다.
