# 01. 협업 필터링 (Collaborative Filtering)

사용자 × 아이템 평점 행렬만 보고 빈칸을 예측한다. 아이템이 무엇인지는
전혀 보지 않는다. **사용자 기반**은 나와 비슷한 사람들의 평점을,
**아이템 기반**은 빈칸의 아이템과 비슷한 아이템에 내가 준 평점을
유사도로 가중 평균한다. 같은 절차를 의사코드 하나에 C와 Python 두 구현으로
담았다.

| 파일 | 내용 |
| --- | --- |
| `collaborativeFiltering.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `collaborativeFiltering.c` | C 구현 |
| `collaborative_filtering.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-15-recommendation/01_collaborative_filtering
make -f /work/Makefile collaborativeFiltering.out && ./collaborativeFiltering.out
python3 collaborative_filtering.py
```

- 결과

```console
toy: users A-D, items 1-4 (0 = not rated)
cosine        A      B      C      D
  A       1.000  0.857  0.000  0.000
  B       0.857  1.000  0.000  0.000
  C       0.000  0.000  1.000  0.781
  D       0.000  0.000  0.781  1.000

ratings: users A-E, movies M1-M6 (0 = not rated)
       M1  M2  M3  M4  M5  M6
  A     5   5   0   4   2   0
  B     5   4   5   4   3   1
  C     4   4   4   3   1   2
  D     1   2   1   1   4   5
  E     4   3   4   4   5   5

user-based: similarity to A
  user  cosine  pearson
  B      0.835    0.866
  C      0.820    1.000
  D      0.466   -0.833
  E      0.705   -0.866

predict A (k = 3, similarity > 0)
  cosine : neighbors B C E  M3 = 4.354  M6 = 2.542
  pearson: neighbors C B    M3 = 4.464  M6 = 1.536

item-based: movies A rated, by cosine to the blank (k = 2)
  M3:  M4 0.845  M1 0.836  M2 0.785  M5 0.761
      -> (0.845*4 + 0.836*5) / (0.845 + 0.836) = 4.497
  M6:  M5 0.909  M4 0.620  M2 0.596  M1 0.562
      -> (0.909*2 + 0.620*4) / (0.909 + 0.620) = 2.811

recommend to A (pearson): M3 (4.464)
```

두 구현이 같은 결과를 낸다.

- **첫 표**는 사용자 4명, 아이템 4개의 작은 행렬이다. A와 B가 0.857, C와 D가 0.781로
  서로 비슷하고, 겹치는 아이템이 없는 쌍은 0이다.
- **사용자 기반**: 코사인은 B·C·E를 모두 0.7\~0.8로 비슷하게 본다. 평점이
  전부 양수라서 정반대 취향인 D도 0.466이 나온다. 피어슨은 각자의 평균을
  빼고 재므로 A보다 1점씩 짜게 줄 뿐 취향이 같은 C를 1.000, 취향이 반대인
  D와 E를 음수로 본다.
- 그래서 이웃이 달라진다. 코사인은 E까지 끌어들여 A의 M6(드라마)을
  2.542로 보고, 피어슨은 C·B만 써서 1.536으로 본다.
- **아이템 기반**: M3과 가장 비슷한 것은 A가 높게 준 M4·M1이라 4.497,
  M6과 가장 비슷한 것은 A가 낮게 준 M5라 2.811이 나온다.

`math.h`의 `sqrt`는 Linux에서 링크 옵션(`-lm`)이 따로 필요해서, 제곱근은
뉴턴 방법(`squareRoot`)으로 직접 구했다. Python도 같은 함수를 써서 출력이
글자 하나까지 같다.

## 바꿔 보기

- `k`를 2로 줄여 보자. 코사인도 B·C만 이웃으로 골라 피어슨과 같은 사람을
  쓰게 되고, M6 예측이 크게 내려간다.
- 예측을 평균 중심으로 바꿔 보자. 이웃의 평점 대신 **이웃 평균에서 벗어난
  만큼**을 가중 평균하고 A의 평균에 더한다. 짜게 주는 C의 평점이 A의
  눈금으로 옮겨진다.
- A의 행을 전부 0으로 바꿔 보자. 처음 온 사용자는 유사도가 모두 0이라
  이웃이 없고 예측도 0이다. 콜드 스타트다.

## 다음

다음 예제(콘텐츠 기반 필터링)는 평점 대신 영화의 장르를 보고 추천한다.
