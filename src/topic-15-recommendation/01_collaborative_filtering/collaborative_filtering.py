"""협업 필터링 — 평점 행렬만 보고 빈칸을 예측한다.

1) 슬라이드의 4x4 행렬로 사용자끼리 코사인 유사도를 잰다.
2) 사용자 기반: 대상 사용자와 비슷한 이웃을 골라 그들의 평점을
   유사도로 가중 평균한다. 코사인과 피어슨 상관을 나란히 본다.
3) 아이템 기반: 빈칸의 아이템과 비슷한 아이템(대상 사용자가 이미 본 것)을
   골라 대상 사용자 자신의 평점을 가중 평균한다.
평점 0은 아직 보지 않았다는 뜻이다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 collaborative_filtering.py`
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
    """두 평점 벡터 사이 각도의 코사인. 안 본 칸(0)도 0으로 그대로 넣는다(슬라이드와 같다)."""
    dot = na = nb = 0.0
    for x, y in zip(a, b):
        dot += x * y
        na += x * x
        nb += y * y
    if na == 0.0 or nb == 0.0:
        return 0.0
    return dot / (square_root(na) * square_root(nb))


def pearson_similarity(a, b):
    """피어슨 상관. 둘 다 본 아이템만 쓰고, 각자의 평균을 빼고 잰다."""
    common = [i for i in range(len(a)) if a[i] > 0 and b[i] > 0]
    if not common:
        return 0.0
    sa = sb = 0.0
    for i in common:
        sa += a[i]
        sb += b[i]
    ma, mb = sa / len(common), sb / len(common)
    num = da = db = 0.0
    for i in common:
        num += (a[i] - ma) * (b[i] - mb)
        da += (a[i] - ma) * (a[i] - ma)
        db += (b[i] - mb) * (b[i] - mb)
    if da == 0.0 or db == 0.0:
        return 0.0
    return num / square_root(da * db)


def top_neighbors(sim, skip, k):
    """sim이 0보다 큰 것 중 큰 순서로 최대 k개. skip(자기 자신)은 뺀다.
    같은 값이면 번호가 작은 쪽이 먼저다."""
    neighbors = []
    while len(neighbors) < k:
        best = -1
        for v, s in enumerate(sim):
            if v == skip or v in neighbors or s <= 0.0:
                continue
            if best < 0 or s > sim[best]:
                best = v
        if best < 0:
            break
        neighbors.append(best)
    return neighbors


def predict_user_based(ratings, sim, neighbors, item):
    """이웃들의 평점을 유사도로 가중 평균한다. 이웃이 안 본 아이템은 건너뛴다."""
    num = den = 0.0
    for v in neighbors:
        if ratings[v][item] > 0:
            num += sim[v] * ratings[v][item]
            den += sim[v]
    return 0.0 if den == 0.0 else num / den


def predict_item_based(ratings, target, item, k):
    """item 열과 target이 본 아이템 열들의 코사인을 재고, 가장 비슷한 k개로
    target 자신의 평점을 가중 평균한다. (예측값, 유사도 목록, 고른 아이템)을 돌려준다."""
    col = [row[item] for row in ratings]
    sim = []
    for j in range(len(ratings[target])):
        if j != item and ratings[target][j] > 0:
            sim.append(cosine_similarity(col, [row[j] for row in ratings]))
        else:
            sim.append(0.0)
    near = top_neighbors(sim, item, k)
    num = den = 0.0
    for j in near:
        num += sim[j] * ratings[target][j]
        den += sim[j]
    return (0.0 if den == 0.0 else num / den), sim, near


def slide_example():
    """1) 슬라이드의 행렬: 사용자 A~D, 아이템 1~4"""
    r = [
        [5, 3, 0, 0],
        [4, 0, 0, 0],
        [0, 0, 5, 4],
        [0, 0, 4, 0],
    ]
    print("slides: users A-D, items 1-4 (0 = not rated)")
    print("cosine        A      B      C      D")
    for u in range(4):
        cells = "".join(f" {cosine_similarity(r[u], r[v]):6.3f}" for v in range(4))
        print(f"  {'ABCD'[u]}     {cells}")


def print_ratings(r):
    print("ratings: users A-E, movies M1-M6 (0 = not rated)")
    print("       M1  M2  M3  M4  M5  M6")
    for u, row in enumerate(r):
        print(f"  {'ABCDE'[u]}  " + "".join(f"{x:4d}" for x in row))


if __name__ == "__main__":
    slide_example()

    # 2) 사용자 5명 x 영화 6편. A의 M3·M6이 빈칸이다.
    r = [
        [5, 5, 0, 4, 2, 0],  # A: 대상 사용자
        [5, 4, 5, 4, 3, 1],  # B
        [4, 4, 4, 3, 1, 2],  # C
        [1, 2, 1, 1, 4, 5],  # D
        [4, 3, 4, 4, 5, 5],  # E
    ]
    target, k = 0, 3
    blanks = [2, 5]  # M3, M6

    print()
    print_ratings(r)

    print("\nuser-based: similarity to A")
    print("  user  cosine  pearson")
    cos_sim = [cosine_similarity(r[target], row) for row in r]
    pear_sim = [pearson_similarity(r[target], row) for row in r]
    for v in range(len(r)):
        if v != target:
            print(f"  {'ABCDE'[v]}     {cos_sim[v]:6.3f}   {pear_sim[v]:6.3f}")

    print(f"\npredict A (k = {k}, similarity > 0)")
    best, best_item = -1.0, -1
    for s, (name, sim) in enumerate((("cosine ", cos_sim), ("pearson", pear_sim))):
        nb = top_neighbors(sim, target, k)
        line = f"  {name}: neighbors"
        for i in range(k):
            line += f" {'ABCDE'[nb[i]]}" if i < len(nb) else "  "
        for b in blanks:
            p = predict_user_based(r, sim, nb, b)
            line += f"  M{b + 1} = {p:.3f}"
            if s == 1 and p > best:
                best, best_item = p, b
        print(line)

    print("\nitem-based: movies A rated, by cosine to the blank (k = 2)")
    for b in blanks:
        p, sim, near = predict_item_based(r, target, b, 2)
        # 비교한 아이템 전부를 유사도 순으로 보인다.
        order = top_neighbors(sim, b, len(sim))
        print(f"  M{b + 1}:" + "".join(f"  M{j + 1} {sim[j]:.3f}" for j in order))
        terms = " + ".join(f"{sim[j]:.3f}*{r[target][j]}" for j in near)
        weights = " + ".join(f"{sim[j]:.3f}" for j in near)
        print(f"      -> ({terms}) / ({weights}) = {p:.3f}")

    print(f"\nrecommend to A (pearson): M{best_item + 1} ({best:.3f})")
