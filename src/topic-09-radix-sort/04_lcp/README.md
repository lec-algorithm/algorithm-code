# 04. LCP 배열 (Longest Common Prefix)

접미사 배열에서 **이웃한 두 접미사가 앞에서 몇 글자를 공유하는지** 적은
배열. 이웃 쌍마다 처음부터 비교해 만든다. 참고로 같은 배열을 O(n)에 만드는
Kasai 알고리즘도 함께 돌려 결과와 글자 비교 횟수를 견준다. 같은 절차를
의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `lcp.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `lcp.c` | C 구현 |
| `lcp.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-09-radix-sort/04_lcp
make -f /work/Makefile lcp.out && ./lcp.out
python3 lcp.py
```

- 결과

```console
text = banana$
SA   = 6 5 3 1 0 4 2

   j  SA[j]  LCP[j]  suffix
   0      6       -  $
   1      5       0  a$
   2      3       1  ana$
   3      1       3  anana$
   4      0       0  banana$
   5      4       0  na$
   6      2       2  nana$
char compares = 12

for reference: Kasai
   i  suffix    rank  j  start h  lcp  compares
   0  banana$      4  1        0    0         1
   1  anana$       3  3        0    3         4
   2  nana$        6  4        2    2         1
   3  ana$         2  5        1    1         1
   4  na$          5  0        0    0         1
   5  a$           1  6        0    0         1
   6  $            0  -        -    -         -
same LCP = yes, char compares = 9
```

두 구현이 같은 결과를 낸다. 위 표가 LCP 배열이고, 이웃 쌍마다 처음부터 비교해
글자 비교가 12번이다. 아래는 참고용 Kasai의 진행이다. 같은 배열을 9번 비교로 만든다.

## 바꿔 보기

`text`를 `aaaaaaaa$`로 바꿔 보자. 공통 접두사가 길어 처음부터 비교하면
1 + 2 + ... + 8 = 36번으로 늘어난다. 최악 O(n^2)이다. Kasai는 15번이다.

## 다음

다음 예제(LRS)는 이 LCP 배열의 **최댓값**으로 가장 긴 반복 부분 문자열을 찾는다.
