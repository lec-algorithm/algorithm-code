# 01. LSD 기수 정렬 (LSD Radix Sort)

**가장 낮은 자리부터** 한 자리씩 카운팅 정렬을 반복한다. 자리마다 쓰는
카운팅 정렬은 주제 03의 `02_counting_sort`와 같고, 키가 값 전체가 아니라
한 자리일 뿐이다. 정수는 10진 자리로, 길이가 같은 문자열은 한 글자를 한
자리로 본다. 같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `lsdRadixSort.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `lsdRadixSort.c` | C 구현 |
| `lsd_radix_sort.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-09-radix-sort/01_lsd_radix_sort
make -f /work/Makefile lsdRadixSort.out && ./lsdRadixSort.out
python3 lsd_radix_sort.py
```

- 결과

```console
input   : 437 042 531 008 074
exp 1   : 531 042 074 437 008
exp 10  : 008 531 437 042 074
exp 100 : 008 042 074 437 531
sorted  : 008 042 074 437 531

input : dab add cab fad fee bad dad bee fed bed ebb ace
char 2: dab cab ebb add fad bad dad fed bed fee bee ace
char 1: dab cab fad bad dad ebb ace add fed bed fee bee
char 0: ace add bad bed bee cab dab dad ebb fad fed fee
sorted: ace add bad bed bee cab dab dad ebb fad fed fee
```

두 구현이 같은 결과를 낸다. 자릿수를 보기 쉽게 앞을 0으로 채워 찍었다.
최댓값 531이 세 자리라서 패스가 세 번(1의 자리, 10의 자리, 100의 자리)
돈다. `exp 10` 줄에서 십의 자리가 3으로 같은 `531`과 `437`은 앞 패스의
순서(531이 앞)를 그대로 지킨다. 아직 백의 자리를 보지 않았으므로 이것이
맞다. 마지막 패스에서 백의 자리가 정리되면 끝난다. 문자열도 같다.
세 글자짜리 단어 12개를 마지막 글자, 가운데 글자, 첫 글자 순으로 세 번
정렬하면 사전순이 된다.

## 바꿔 보기

`countingSortByDigit`의 3단계를 앞에서부터(`0 to n-1`)로 바꿔 보자. 패스
하나만 보면 여전히 그 자리로는 정렬되어 있지만, 같은 숫자끼리의 순서가
뒤집혀서 앞 패스가 만든 순서가 깨진다. 최종 결과가 틀리게 나온다.
배열에 `1000`을 넣으면 패스가 네 번으로 늘어난다. 시간이 원소 수만이
아니라 **자릿수**에도 비례한다는 뜻이다.

## 다음

다음 예제(MSD 기수 정렬)는 반대로 **가장 높은 자리부터** 버킷으로 나눈다.
