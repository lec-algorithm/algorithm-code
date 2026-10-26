# 03. 접미사 배열 (Suffix Array)

문자열의 **모든 접미사를 사전순으로 정렬**하고, 그 순서대로 접미사의 시작
위치만 적은 배열. 접미사 문자열을 그대로 정렬하는 단순한 방법과, 앞 2글자,
4글자, 8글자의 순위를 두 배씩 늘려 가는 **더블링(doubling)** 두 가지로
만들어 결과가 같은지 본다. 같은 절차를 의사코드 하나에 C와 Python 두
구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `suffixArray.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `suffixArray.c` | C 구현 |
| `suffix_array.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-09-radix-sort/03_suffix_array
make -f /work/Makefile suffixArray.out && ./suffixArray.out
python3 suffix_array.py
```

- 결과

```console
text = banana$ (n = 7)
  0 banana$
  1 anana$
  2 nana$
  3 ana$
  4 na$
  5 a$
  6 $

naive    SA = 6 5 3 1 0 4 2

k = 2
   j  SA[j]  key         rank  suffix
   0      6  ( 36,  -1)     0  $
   1      5  ( 97,  36)     1  a$
   2      1  ( 97, 110)     2  anana$
   3      3  ( 97, 110)     2  ana$
   4      0  ( 98,  97)     3  banana$
   5      2  (110,  97)     4  nana$
   6      4  (110,  97)     4  na$
  distinct ranks = 5 of 7
k = 4
   j  SA[j]  key         rank  suffix
   0      6  (  0,  -1)     0  $
   1      5  (  1,  -1)     1  a$
   2      3  (  2,   1)     2  ana$
   3      1  (  2,   2)     3  anana$
   4      0  (  3,   4)     4  banana$
   5      4  (  4,   0)     5  na$
   6      2  (  4,   4)     6  nana$
  distinct ranks = 7 of 7

doubling SA = 6 5 3 1 0 4 2
same = yes
```

두 구현이 같은 결과를 낸다. `$`는 ASCII 36이라 어느 소문자(97 이상)보다
작다. 그래서 `$`로 끝나는 접미사는 자기를 접두사로 갖는 더 긴 접미사보다
앞에 온다.

`k = 2` 표의 key는 (첫 글자, 둘째 글자)의 문자 코드다. `anana$`와 `ana$`는
둘 다 `an`으로 시작해 key가 같으므로 같은 순위 2를 받는다. `k = 4`에서는
key가 (앞 2글자의 순위, 2칸 뒤에서 시작하는 2글자의 순위)다. `ana$`는
(2, 1), `anana$`는 (2, 2)라서 이제 갈린다. 순위 7개가 모두 다르면 끝난다.
두 라운드 만에 끝났다.

## 바꿔 보기

`text`를 `mississippi$`로 바꿔 보자. `k = 8`까지 가서 라운드가 하나 늘어난다.
`aaaaaaa$`처럼 같은 문자만 반복되어도 라운드 수는 log2 n을 넘지 않는다
(n = 8이면 3라운드). 비교하는 길이가 라운드마다 두 배가 되기 때문이다.

## 다음

다음 예제(LCP 배열)는 정렬된 접미사 중 **이웃한 둘이 앞에서 몇 글자를
공유하는지** 센다.
