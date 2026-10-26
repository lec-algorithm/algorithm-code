"""콘텐츠 기반 필터링 — 아이템의 특성(장르)과 사용자 프로필을 비교한다.

1) 작은 예: 장르 벡터와 "액션·코미디를 좋아함" 벡터의 코사인 유사도
2) 사용자 A의 평점으로 프로필을 만들고, 안 본 영화를 코사인 유사도로 줄 세운다.
   아무도 평가하지 않은 새 영화(M7)도 장르만 있으면 점수를 받는다.
3) 하이브리드: 협업 필터링 점수와 콘텐츠 점수를 가중치로 섞는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 content_based_filtering.py`
"""


def square_root(x):
    """제곱근. C 구현과 같은 뉴턴 방법으로 구해 출력이 똑같게 한다."""
    if x <= 0.0:
        return 0.0
    r = x if x > 1.0 else 1.0
    for _ in range(60):
        r = 0.5 * (r + x / r)
    return r


def cosine_similarity(a, b):
    """두 벡터 사이 각도의 코사인. 어느 한쪽이 0벡터면 0이다."""
    dot = na = nb = 0.0
    for x, y in zip(a, b):
        dot += x * y
        na += x * x
        nb += y * y
    if na == 0.0 or nb == 0.0:
        return 0.0
    return dot / (square_root(na) * square_root(nb))


def build_profile(features, ratings):
    """본 영화마다 (평점 - 3)만큼 그 영화의 장르를 더한다.
    3점은 보통이라 0, 높게 준 영화의 장르는 +, 낮게 준 영화의 장르는 -가 된다."""
    profile = [0.0] * len(features[0])
    for genres, rating in zip(features, ratings):
        if rating > 0:
            w = rating - 3
            for t, f in enumerate(genres):
                profile[t] += w * f
    return profile


def rank_by_content(features, ratings, profile):
    """안 본 영화의 점수를 매기고 (점수 목록, 점수 내림차순 번호 목록)을 돌려준다.
    같은 점수면 번호가 작은 쪽이 먼저다(sorted는 안정적이다)."""
    score = [0.0] * len(features)
    candidates = []
    for i, rating in enumerate(ratings):
        if rating == 0:
            score[i] = cosine_similarity(profile, features[i])
            candidates.append(i)
    order = sorted(candidates, key=lambda i: -score[i])
    return score, order


def hybrid_score(cf, content, alpha):
    """협업 필터링 점수 cf(0~1)와 콘텐츠 점수를 alpha : 1-alpha로 섞는다.
    cf가 None이면(아무도 평가하지 않은 새 아이템) 콘텐츠 점수만 쓴다."""
    if cf is None:
        return content
    return alpha * cf + (1.0 - alpha) * content


def slide_example():
    """1) 작은 예: 장르 (액션, 코미디, 드라마)"""
    movies = [[1, 0, 0], [1, 1, 0], [0, 0, 1]]
    likes = [1, 1, 0]
    print("toy: genres (action, comedy, drama), user likes (1 1 0)")
    for i, m in enumerate(movies):
        print(f"  movie {i + 1} ({m[0]} {m[1]} {m[2]})  {cosine_similarity(likes, m):.3f}")


if __name__ == "__main__":
    slide_example()

    # 2) 영화 7편의 장르: 액션, 코미디, 드라마, SF. M7은 막 나온 새 영화다.
    features = [
        [1, 1, 0, 0],  # M1
        [1, 0, 1, 0],  # M2
        [1, 0, 0, 1],  # M3
        [1, 0, 0, 1],  # M4
        [0, 1, 1, 0],  # M5
        [0, 0, 1, 0],  # M6
        [1, 1, 0, 1],  # M7 (new)
    ]
    # 예제 01과 같은 사용자 A의 평점. M7은 아무도 평가하지 않았다.
    ratings = [5, 5, 0, 4, 2, 0, 0]
    new_movie = 6

    print("\ngenres: action comedy drama sf")
    for i, genres in enumerate(features):
        line = f"  M{i + 1}  " + " ".join(str(f) for f in genres)
        if ratings[i] > 0:
            line += f"   A rated {ratings[i]}  weight {ratings[i] - 3:+d}"
        elif i == new_movie:
            line += "   new, no ratings yet"
        else:
            line += "   -"
        print(line)

    profile = build_profile(features, ratings)
    print("profile of A = sum of (rating - 3) * genres = ("
          + " ".join(f"{p:.0f}" for p in profile) + ")")

    score, order = rank_by_content(features, ratings, profile)
    print("\nscore = cosine(profile, genres), movies A has not rated")
    for rank, m in enumerate(order, start=1):
        print(f"  {rank}. M{m + 1}  {score[m]:.3f}")

    # 3) 하이브리드. 작은 예의 점수를 반반 섞는다.
    alpha = 0.5
    slide_cf = [0.8, 0.7, 0.3]
    slide_content = [0.6, 0.9, 0.4]
    print(f"\nhybrid = {alpha:.1f} * collaborative + {1.0 - alpha:.1f} * content")
    print("  toy:" + "".join(f" {hybrid_score(c, s, alpha):.3f}"
                                for c, s in zip(slide_cf, slide_content)))

    # 예제 01의 피어슨 예측(M3 4.464, M6 1.536)을 5로 나눠 0~1로 맞춘다.
    # M7은 평점이 하나도 없어 협업 필터링 점수가 없다(None).
    predicted = {2: 4.464, 5: 1.536, 6: None}
    for m in order:
        cf = None if predicted[m] is None else predicted[m] / 5.0
        cf_text = "cf  none " if cf is None else f"cf {cf:.3f} "
        print(f"  A: M{m + 1}  {cf_text} content {score[m]:.3f}"
              f"  -> {hybrid_score(cf, score[m], alpha):.3f}")
