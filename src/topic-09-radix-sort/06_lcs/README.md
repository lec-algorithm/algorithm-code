# 06. 가장 긴 공통 부분 문자열 (Longest Common Substring)

**두 문자열에 함께 나오는** 가장 긴 부분 문자열을 찾는다. 두 문자열을
구분자 `#`로 이어 붙여 문자열 하나로 만들고, 그 접미사 배열과 LCP 배열을
쓴다. LRS와 다른 점은 하나다. 이웃한 두 접미사가 **서로 다른 문자열에서
시작할 때만** LCP를 후보로 삼는다. 같은 절차를 의사코드 하나에 C와 Python
두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `lcs.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `lcs.c` | C 구현 |
| `lcs.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-09-radix-sort/06_lcs
make -f /work/Makefile lcs.out && ./lcs.out
python3 lcs.py
```

- 결과

```console
s1 = mississippi, s2 = kiss
text = mississippi#kiss$ (n = 17, s1 = 0..10, s2 = 12..15)
   j  SA[j]  from  LCP[j]  suffix
   0     11     -       -  #kiss$
   1     16     -       0  $
   2     10     1       0  i#kiss$
   3      7     1       1  ippi#kiss$
   4     13     2       1  iss$  *
   5      4     1       3  issippi#kiss$  *
   6      1     1       4  ississippi#kiss$
   7     12     2       0  kiss$  *
   8      0     1       0  mississippi#kiss$  *
   9      9     1       0  pi#kiss$
  10      8     1       1  ppi#kiss$
  11     15     2       0  s$  *
  12      6     1       1  sippi#kiss$  *
  13      3     1       2  sissippi#kiss$
  14     14     2       1  ss$  *
  15      5     1       2  ssippi#kiss$  *
  16      2     1       3  ssissippi#kiss$
max LCP over all pairs = 4
LCS = "iss" (length 3) at j = 5
```

두 구현이 같은 결과를 낸다. `from`은 접미사가 시작한 문자열이다(`-`는
구분자 `#`와 끝 `$`). `*`가 붙은 줄이 앞 이웃과 `from`이 다른 쌍, 곧 후보다.
LCP 전체의 최댓값은 4(j = 6, `issi`)지만 두 접미사가 모두 `mississippi`
쪽이라 공통 부분 문자열이 아니다. 그것은 `mississippi` 안의 반복(LRS)이다.
후보 중 최댓값은 j = 5의 3이고, `kiss`의 `iss`와 `mississippi`의 `iss`가
만난 자리다.

`#`는 이어 붙인 문자열에 한 번만 나온다. 그래서 `s1` 쪽 접미사와 `s2` 쪽
접미사의 공통 접두사는 `#`를 넘어갈 수 없고, LCP가 곧 공통 부분 문자열의
길이가 된다.

## 바꿔 보기

`s2`를 `"sip"`로 바꿔 보자. 답은 `sip`이다. `s2`를 `"xyz"`처럼 겹치는
글자가 없는 문자열로 바꾸면 후보 LCP가 모두 0이라 빈 문자열이 나온다.

## 다음

다음 예제(스택과 큐)는 LSD 기수 정렬의 버킷을 **큐**로 보면 왜 안정성이
저절로 지켜지는지 확인한다.
