# 04. LCP 배열 (Longest Common Prefix, Kasai 알고리즘)

접미사 배열에서 **이웃한 두 접미사가 앞에서 몇 글자를 공유하는지** 적은
배열. Kasai 알고리즘은 접미사를 정렬 순서가 아니라 **텍스트 위치 순서**로
처리한다. 그러면 앞 단계에서 센 길이 h를 하나만 줄여 다음 단계의 출발점으로
쓸 수 있어 전체가 O(n)이다. 같은 절차를 의사코드 하나에 C와 Python 두
구현으로 담았다.

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

   i  suffix    rank  j  start h  lcp  compares
   0  banana$      4  1        0    0         1
   1  anana$       3  3        0    3         4
   2  nana$        6  4        2    2         1
   3  ana$         2  5        1    1         1
   4  na$          5  0        0    0         1
   5  a$           1  6        0    0         1
   6  $            0  -        -    -         -

   j  SA[j]  LCP[j]  suffix
   0      6       -  $
   1      5       0  a$
   2      3       1  ana$
   3      1       3  anana$
   4      0       0  banana$
   5      4       0  na$
   6      2       2  nana$
char compares: kasai 9, naive 12
```

두 구현이 같은 결과를 낸다. 위 표가 Kasai의 진행이다. `j`는 정렬에서 바로
앞에 있는 접미사의 시작 위치, `start h`는 비교를 시작한 글자 수다.
`i = 1`에서 `anana$`와 `ana$`가 3글자를 공유했다. 다음 `i = 2`의
`nana$`는 `anana$`에서 첫 글자 `a`를 뗀 것이므로 앞 이웃과 적어도
3 - 1 = 2글자를 공유한다. 그래서 2글자를 건너뛰고 셋째 글자부터 비교해
한 번 만에 끝났다. `i = 3`도 같은 이유로 1글자부터 시작한다.

아래 표가 결과다. 이웃 쌍마다 처음부터 비교하면 글자 비교가 12번,
Kasai는 9번이다. 문자열이 길고 반복이 많을수록 차이가 커진다.

## 바꿔 보기

`text`를 `aaaaaaaa$`로 바꿔 보자. 이웃 쌍마다 처음부터 세면 비교가
1 + 2 + ... + 8 = 36번으로 늘어나 O(n^2)이 되지만, Kasai는 h를 이어 쓰므로
15번에 그친다. Kasai의 비교 횟수는 언제나 2n 이하다.

## 다음

다음 예제(LRS)는 이 LCP 배열의 **최댓값**으로 가장 긴 반복 부분 문자열을 찾는다.
