# 03. 정규식에서 NFA로 (Regular Expression to NFA)

정규식에서 **비결정적 유한 오토마타**(NFA)를 자동으로 만들고, 텍스트를 읽으며
"지금 있을 수 있는 상태 전부"를 집합으로 들고 다닌다. 상태 i는 정규식의 i번째
글자이고, 글자 상태는 그 글자를 읽어 i+1로 간다. 괄호·`*`·`|`는 입력 없이
움직이는 ε 전이를 만든다. 같은 절차를 의사코드 하나에 C와 Python 두 구현으로
담았다. Python도 `re` 모듈을 쓰지 않는다.

| 파일 | 내용 |
| --- | --- |
| `nfaRegex.pseudo` | 기준이 되는 절차(BuildNFA·Closure·Recognizes). 두 구현은 이것을 옮긴 것이다 |
| `nfaRegex.c` | C 구현 |
| `nfa_regex.py` | Python 구현 |

지원하는 문법은 글자, `.`, `*`, `|`, 괄호다. `|`는 괄호 한 쌍 안에 하나만 둔다.

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-13-regex/03_nfa_regex
make -f /work/Makefile nfaRegex.out && ./nfaRegex.out
python3 nfa_regex.py
```

- 결과

```console
regex = (0|1(01*0)*1)*
nfa   = ((0|1(01*0)*1)*), states 0..16, accept 16

  state  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
  char   (  (  0  |  1  (  0  1  *  0  )  *  1  )  *  )

  eps   0->1 1->2 5->6 7->8 8->7 8->9 5->11 11->5 10->11 11->12 1->4 3->13 1->14 14->1 13->14 14->15 15->16

text = 1001
  start      {0 1 2 4 14 15 16}
  read '1'   {5}
  closure    {5 6 11 12}
  read '0'   {7}
  closure    {7 8 9}
  read '0'   {10}
  closure    {5 6 10 11 12}
  read '1'   {13}
  closure    {1 2 4 13 14 15 16}
  -> accept

  text   value  result
  11         3  accept
  110        6  accept
  1001       9  accept
  1100      12  accept
  10         2  reject
  1011      11  reject
  10000     16  reject

blow-up: (a*)^n b on a^n
  n      M    N  result      work
  2      7    2  no match      16
  4     11    4  no match      46
  6     15    6  no match      92
  8     19    8  no match     154
  10    23   10  no match     232
```

두 구현이 같은 결과를 낸다. `(0|1(01*0)*1)*`는 3의 배수를 이진수로 적은
문자열을 나타낸다. 정규식 전체를 괄호로 한 번 더 감싸므로 상태는 0\~16이고
16이 수락 상태다.

`1001`(9)을 읽는 과정을 보면, 글자를 하나 읽을 때마다 집합이 한 번 바뀐다.
`read` 줄은 글자를 읽고 옮겨 간 상태, `closure` 줄은 거기서 ε 전이로 더 갈
수 있는 상태까지 넓힌 것이다. 마지막 집합에 16이 있으므로 수락한다.

마지막 표는 `01_regular_expression`의 백트래킹이 35만 번 넘게 비교하던
입력이다. 여기서는 집합에 넣은 상태 수의 합(`work`)이 232에 그친다. 글자
하나에 상태를 많아야 M개 보므로 최악 O(MN)이다.

## 바꿔 보기

- `main`의 `regex`를 `((A*B|AC)D)`로, 텍스트를 `AABD`로 바꿔 보자.
- `texts`에 `1111`(15)과 `10010`(18)을 넣어 3의 배수인지 확인해 보자.
- `regex`를 `.*ab*c.*`로 바꾸면 텍스트 어디에든 `ab*c`가 들어 있는지 찾는
  검색이 된다.

## 다음

다음 주제(동적 계획법)에서는 작은 문제의 답을 표에 적어 두고 다시 쓴다.
