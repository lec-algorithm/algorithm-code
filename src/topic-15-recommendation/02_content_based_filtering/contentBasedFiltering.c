/* 콘텐츠 기반 필터링 — 아이템의 특성(장르)과 사용자 프로필을 비교한다.
 *
 * 1) 작은 예: 장르 벡터와 "액션·코미디를 좋아함" 벡터의 코사인 유사도
 * 2) 사용자 A의 평점으로 프로필을 만들고, 안 본 영화를 코사인 유사도로 줄 세운다.
 *    아무도 평가하지 않은 새 영화(M7)도 장르만 있으면 점수를 받는다.
 * 3) 하이브리드: 협업 필터링 점수와 콘텐츠 점수를 가중치로 섞는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-15-recommendation/02_content_based_filtering/contentBasedFiltering.out
 */
#include <stdio.h>

#define MOVIES 7
#define GENRES 4

/* 제곱근. math.h의 sqrt는 링크 옵션(-lm)이 따로 필요해서 뉴턴 방법으로 구한다. */
double squareRoot(double x) {
    if (x <= 0.0) {
        return 0.0;
    }
    double r = x > 1.0 ? x : 1.0;
    for (int i = 0; i < 60; i++) {
        r = 0.5 * (r + x / r);
    }
    return r;
}

/* 두 벡터 사이 각도의 코사인. 어느 한쪽이 0벡터면 0이다. */
double cosineSimilarity(const double a[], const double b[], int n) {
    double dot = 0.0, na = 0.0, nb = 0.0;
    for (int i = 0; i < n; i++) {
        dot += a[i] * b[i];
        na += a[i] * a[i];
        nb += b[i] * b[i];
    }
    if (na == 0.0 || nb == 0.0) {
        return 0.0;
    }
    return dot / (squareRoot(na) * squareRoot(nb));
}

/* 본 영화마다 (평점 - 3)만큼 그 영화의 장르를 더한다.
 * 3점은 보통이라 0, 높게 준 영화의 장르는 +, 낮게 준 영화의 장르는 -가 된다. */
void buildProfile(const double features[][GENRES], const int ratings[],
                  int n, int g, double profile[]) {
    for (int t = 0; t < g; t++) {
        profile[t] = 0.0;
    }
    for (int i = 0; i < n; i++) {
        if (ratings[i] > 0) {
            int w = ratings[i] - 3;
            for (int t = 0; t < g; t++) {
                profile[t] += w * features[i][t];
            }
        }
    }
}

/* 안 본 영화의 점수를 score에 적고, 점수 내림차순으로 order에 담아 개수를 돌려준다.
 * 같은 점수면 번호가 작은 쪽이 먼저다(삽입 정렬이라 안정적이다). */
int rankByContent(const double features[][GENRES], const int ratings[], int n,
                  int g, const double profile[], double score[], int order[]) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        score[i] = 0.0;
        if (ratings[i] == 0) {
            score[i] = cosineSimilarity(profile, features[i], g);
            int j = count++;
            while (j > 0 && score[order[j - 1]] < score[i]) {
                order[j] = order[j - 1];
                j--;
            }
            order[j] = i;
        }
    }
    return count;
}

/* 협업 필터링 점수 cf(0~1)와 콘텐츠 점수를 alpha : 1-alpha로 섞는다.
 * cf가 음수면 "없음"(아무도 평가하지 않은 새 아이템)으로 보고 콘텐츠 점수만 쓴다. */
double hybridScore(double cf, double content, double alpha) {
    if (cf < 0.0) {
        return content;
    }
    return alpha * cf + (1.0 - alpha) * content;
}

/* 1) 작은 예: 장르 (액션, 코미디, 드라마) */
static void slideExample(void) {
    const double movies[3][GENRES] = {{1, 0, 0}, {1, 1, 0}, {0, 0, 1}};
    const double likes[GENRES] = {1, 1, 0};
    printf("toy: genres (action, comedy, drama), user likes (1 1 0)\n");
    for (int i = 0; i < 3; i++) {
        printf("  movie %d (%.0f %.0f %.0f)  %.3f\n", i + 1, movies[i][0],
               movies[i][1], movies[i][2], cosineSimilarity(likes, movies[i], 3));
    }
}

int main(void) {
    slideExample();

    /* 2) 영화 7편의 장르: 액션, 코미디, 드라마, SF. M7은 막 나온 새 영화다. */
    const double features[MOVIES][GENRES] = {
        {1, 1, 0, 0}, /* M1 */
        {1, 0, 1, 0}, /* M2 */
        {1, 0, 0, 1}, /* M3 */
        {1, 0, 0, 1}, /* M4 */
        {0, 1, 1, 0}, /* M5 */
        {0, 0, 1, 0}, /* M6 */
        {1, 1, 0, 1}, /* M7 (new) */
    };
    /* 예제 01과 같은 사용자 A의 평점. M7은 아무도 평가하지 않았다. */
    const int ratings[MOVIES] = {5, 5, 0, 4, 2, 0, 0};
    const int newMovie = 6;

    printf("\ngenres: action comedy drama sf\n");
    for (int i = 0; i < MOVIES; i++) {
        printf("  M%d  %.0f %.0f %.0f %.0f", i + 1, features[i][0], features[i][1],
               features[i][2], features[i][3]);
        if (ratings[i] > 0) {
            printf("   A rated %d  weight %+d\n", ratings[i], ratings[i] - 3);
        } else if (i == newMovie) {
            printf("   new, no ratings yet\n");
        } else {
            printf("   -\n");
        }
    }

    double profile[GENRES];
    buildProfile(features, ratings, MOVIES, GENRES, profile);
    printf("profile of A = sum of (rating - 3) * genres = (%.0f %.0f %.0f %.0f)\n",
           profile[0], profile[1], profile[2], profile[3]);

    double score[MOVIES];
    int order[MOVIES];
    int count = rankByContent(features, ratings, MOVIES, GENRES, profile, score, order);
    printf("\nscore = cosine(profile, genres), movies A has not rated\n");
    for (int i = 0; i < count; i++) {
        printf("  %d. M%d  %.3f\n", i + 1, order[i] + 1, score[order[i]]);
    }

    /* 3) 하이브리드. 작은 예의 점수를 반반 섞는다. */
    const double alpha = 0.5;
    const double slideCf[3] = {0.8, 0.7, 0.3};
    const double slideContent[3] = {0.6, 0.9, 0.4};
    printf("\nhybrid = %.1f * collaborative + %.1f * content\n", alpha, 1.0 - alpha);
    printf("  toy:");
    for (int i = 0; i < 3; i++) {
        printf(" %.3f", hybridScore(slideCf[i], slideContent[i], alpha));
    }
    printf("\n");

    /* 예제 01의 피어슨 예측(M3 4.464, M6 1.536)을 5로 나눠 0~1로 맞춘다.
     * M7은 평점이 하나도 없어 협업 필터링 점수가 없다(-1). */
    const double predicted[MOVIES] = {0, 0, 4.464, 0, 0, 1.536, -1};
    for (int i = 0; i < count; i++) {
        int m = order[i];
        double cf = predicted[m] < 0.0 ? -1.0 : predicted[m] / 5.0;
        printf("  A: M%d  ", m + 1);
        if (cf < 0.0) {
            printf("cf  none ");
        } else {
            printf("cf %.3f ", cf);
        }
        printf(" content %.3f  -> %.3f\n", score[m], hybridScore(cf, score[m], alpha));
    }
    return 0;
}
