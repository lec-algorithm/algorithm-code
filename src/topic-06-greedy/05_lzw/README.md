# 05. LZW 압축 (Lempel-Ziv-Welch)

읽으면서 **반복되는 문자열**에 새 코드를 붙여 가는 압축. 인코더는 문자열
표를 만들며 코드를 내보내고, 디코더는 코드만 보고 **같은 표를 스스로
다시 만든다.** 표는 보내지 않는다. 같은 절차를 의사코드 하나에 C와 Python
두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `lzw.pseudo` | 기준이 되는 절차(인코딩·디코딩). 두 구현은 이것을 옮긴 것이다 |
| `lzw.c` | C 구현 |
| `lzw.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-06-greedy/05_lzw
make -f /work/Makefile lzw.out && ./lzw.out
python3 lzw.py
```

- 결과

```console
text = BABAABAAA

encode
  P     C   P+C   action
  B     A   BA    out 66 (B), add 256 = BA
  A     B   AB    out 65 (A), add 257 = AB
  B     A   BA    in table, P = BA
  BA    A   BAA   out 256 (BA), add 258 = BAA
  A     B   AB    in table, P = AB
  AB    A   ABA   out 257 (AB), add 259 = ABA
  A     A   AA    out 65 (A), add 260 = AA
  A     A   AA    in table, P = AA
  AA              out 260 (AA)
codes: 66 65 256 257 65 260

decode
  OLD   NEW   S     C   add
              B
  66    65    A     A   256 = BA
  65    256   BA    B   257 = AB
  256   257   AB    A   258 = BAA
  257   65    A     A   259 = ABA
  65    260   AA    A   260 = AA  (NEW not in table yet)
decoded = BABAABAAA

bits: ascii 9 x 8 = 72, lzw 6 x 9 = 54
```

두 구현이 같은 결과를 낸다. 9글자가 코드 6개가 되었다. 코드 256\~260은
인코더가 읽으면서 만든 것이고, 디코더의 add 열을 보면 **같은 표가 같은
순서로** 다시 만들어진다.

마지막 줄을 보자. 디코더가 코드 260을 받았을 때 표에는 아직 259까지만
있다. 인코더가 260을 만들자마자 바로 썼기 때문이다. 그런 문자열은
"OLD의 번역 + 그 첫 글자"뿐이라서 디코더가 직접 만들어 낸다.

## 바꿔 보기

`text`를 `ABABABABABABABAB`처럼 반복이 많은 문자열로 바꿔 보자. 표의
문자열이 점점 길어지면서 코드 하나가 여러 글자를 대신한다. 반대로 반복이
없는 문자열에서는 코드가 글자 수만큼 나와 오히려 비트가 늘어난다.

## 다음

다음 주제(이진 탐색 트리)에서는 트리를 검색에 쓴다.
