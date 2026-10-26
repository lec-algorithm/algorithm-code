# 05. 가장 긴 반복 부분 문자열 (Longest Repeated Substring)

한 문자열 안에 **두 번 이상 나오는 가장 긴 부분 문자열**을 찾는다.
반복되는 부분 문자열은 두 접미사의 공통 접두사이고, 접미사를 정렬하면
공통 접두사가 긴 접미사끼리 이웃하게 모인다. 그래서 **LCP 배열의 최댓값**이
답이다. 모든 위치 쌍을 비교하는 무차별 대입과 결과를 견준다. 같은 절차를
의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `lrs.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `lrs.c` | C 구현 |
| `lrs.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-09-radix-sort/05_lrs
make -f /work/Makefile lrs.out && ./lrs.out
python3 lrs.py
```

- 결과

```console
text = banana$ (n = 7)
   j  SA[j]  LCP[j]  suffix
   0      6       -  $
   1      5       0  a$
   2      3       1  ana$
   3      1       3  anana$
   4      0       0  banana$
   5      4       0  na$
   6      2       2  nana$
suffix sort: max LCP = 3 at j = 3 -> "ana" (positions 3, 1), 6 adjacent pairs
brute force: "ana", 21 pairs, 28 char compares

text = aacaagtttacaagc$ (n = 16)
   j  SA[j]  LCP[j]  suffix
   0     15       -  $
   1      0       0  aacaagtttacaagc$
   2     11       2  aagc$
   3      3       3  aagtttacaagc$
   4      9       1  acaagc$
   5      1       5  acaagtttacaagc$
   6     12       1  agc$
   7      4       2  agtttacaagc$
   8     14       0  c$
   9     10       1  caagc$
  10      2       4  caagtttacaagc$
  11     13       0  gc$
  12      5       1  gtttacaagc$
  13      8       0  tacaagc$
  14      7       1  ttacaagc$
  15      6       2  tttacaagc$
suffix sort: max LCP = 5 at j = 5 -> "acaag" (positions 9, 1), 15 adjacent pairs
brute force: "acaag", 120 pairs, 161 char compares
```

두 구현이 같은 결과를 낸다. `banana$`에서는 `ana`가 위치 1과 3에서 두 번
나온다(겹쳐도 된다). 두 번째 문자열은 DNA 염기 서열 같은 15글자로,
`acaag`가 위치 1과 9에서 반복된다. 정렬하고 나면 두 접미사 `acaagc$`와
`acaagtttacaagc$`가 바로 이웃(j = 4, 5)에 와 있다. 접미사 정렬은 이웃 쌍
15개만 보면 되지만, 무차별 대입은 위치 쌍 120개를 모두 본다. 길이가 n이면
쌍은 n(n-1)/2개로 늘어난다.

## 바꿔 보기

`report("mississippi$")`를 더해 보자. 답은 `issi`다(위치 1과 4, 겹침).
`abcdefg$`처럼 반복이 없는 문자열이면 LCP가 모두 0이라 빈 문자열이 나온다.

## 다음

다음 예제(LCS)는 문자열 **두 개**에 공통으로 나오는 가장 긴 부분 문자열을
같은 방법으로 찾는다.
