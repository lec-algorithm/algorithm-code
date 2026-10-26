# 02. 콘텐츠 기반 필터링 (Content-Based Filtering)

아이템마다 특성 벡터(여기서는 장르)를 두고, 사용자가 준 평점으로
**사용자 프로필**을 만든 뒤 안 본 아이템을 프로필과의 코사인 유사도로 줄
세운다. 다른 사용자의 평점은 쓰지 않는다. 끝에서 협업 필터링 점수와
섞는 **하이브리드**도 함께 본다. 같은 절차를 의사코드 하나에 C와 Python
두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `contentBasedFiltering.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `contentBasedFiltering.c` | C 구현 |
| `content_based_filtering.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-15-recommendation/02_content_based_filtering
make -f /work/Makefile contentBasedFiltering.out && ./contentBasedFiltering.out
python3 content_based_filtering.py
```

- 결과

```console
slides: genres (action, comedy, drama), user likes (1 1 0)
  movie 1 (1 0 0)  0.707
  movie 2 (1 1 0)  1.000
  movie 3 (0 0 1)  0.000

genres: action comedy drama sf
  M1  1 1 0 0   A rated 5  weight +2
  M2  1 0 1 0   A rated 5  weight +2
  M3  1 0 0 1   -
  M4  1 0 0 1   A rated 4  weight +1
  M5  0 1 1 0   A rated 2  weight -1
  M6  0 0 1 0   -
  M7  1 1 0 1   new, no ratings yet
profile of A = sum of (rating - 3) * genres = (5 1 1 1)

score = cosine(profile, genres), movies A has not rated
  1. M3  0.802
  2. M7  0.764
  3. M6  0.189

hybrid = 0.5 * collaborative + 0.5 * content
  slides: 0.700 0.800 0.350
  A: M3  cf 0.893  content 0.802  -> 0.847
  A: M7  cf  none  content 0.764  -> 0.764
  A: M6  cf 0.307  content 0.189  -> 0.248
```

두 구현이 같은 결과를 낸다.

- **첫 세 줄**은 슬라이드의 예다. 액션·코미디를 좋아하는 사용자에게
  액션+코미디 영화가 1.000, 액션 영화가 0.707, 드라마가 0이다.
- **프로필**: A는 액션 영화 셋(M1·M2·M4)을 높게 주고 코미디·드라마인 M5를
  낮게 줬다. 평점에서 3을 뺀 만큼 장르를 더하면 액션이 5로 압도한다.
- **순위**: 액션·SF인 M3이 0.802로 1위, 드라마뿐인 M6이 0.189로 꼴찌다.
  예제 01의 협업 필터링과 같은 결론이다.
- **새 영화 M7**은 아무도 평가하지 않았지만 장르만으로 0.764를 받았다.
  협업 필터링은 M7에 점수를 줄 수 없다(아이템 콜드 스타트).
- **하이브리드**: 슬라이드의 두 점수를 반반 섞으면 0.700, 0.800, 0.350이다.
  A에게는 예제 01의 피어슨 예측을 5로 나눠 0\~1로 맞춘 뒤 섞었고, 협업
  필터링 점수가 없는 M7은 콘텐츠 점수를 그대로 쓴다.

## 바꿔 보기

- `alpha`를 1.0과 0.0으로 바꿔 보자. 각각 협업 필터링만, 콘텐츠 기반만 쓴
  점수가 된다. 1.0이어도 M7은 협업 필터링 점수가 없어 콘텐츠 점수로 남는다.
- A의 평점을 전부 0으로 바꿔 보자. 프로필이 0벡터라 모든 점수가 0이다.
  콘텐츠 기반도 **새 사용자**의 콜드 스타트는 풀지 못한다.
- 장르에 특성을 하나 더해 보자(예: 감독, 배우). `GENRES`와 각 행의 길이만
  늘리면 같은 코드가 돈다.

## 다음

이 주제의 끝이다. 두 방식의 차이와 한계는 강의 자료에서 정리한다.
