/* 협업 필터링 — 평점 행렬만 보고 빈칸을 예측한다.
 *
 * 1) 작은 4x4 행렬로 사용자끼리 코사인 유사도를 잰다.
 * 2) 사용자 기반: 대상 사용자와 비슷한 이웃을 골라 그들의 평점을
 *    유사도로 가중 평균한다. 코사인과 피어슨 상관을 나란히 본다.
 * 3) 아이템 기반: 빈칸의 아이템과 비슷한 아이템(대상 사용자가 이미 본 것)을
 *    골라 대상 사용자 자신의 평점을 가중 평균한다.
 * 평점 0은 아직 보지 않았다는 뜻이다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-15-recommendation/01_collaborative_filtering/collaborativeFiltering.out
 */
#include <stdio.h>

#define USERS 5
#define ITEMS 6
#define MAX_N 8

/* 제곱근. math.h의 sqrt는 링크 옵션(-lm)이 따로 필요해서 뉴턴 방법으로 구한다.
 * r을 (r + x/r)/2로 거듭 고치면 r*r이 x에 빠르게 다가간다. */
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

/* 두 평점 벡터 사이 각도의 코사인. 안 본 칸(0)도 0으로 그대로 넣는다. */
double cosineSimilarity(const int a[], const int b[], int n) {
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

/* 피어슨 상관. 둘 다 본 아이템만 쓰고, 각자의 평균을 빼고 잰다. */
double pearsonSimilarity(const int a[], const int b[], int n) {
    double sa = 0.0, sb = 0.0;
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > 0 && b[i] > 0) {
            sa += a[i];
            sb += b[i];
            count++;
        }
    }
    if (count == 0) {
        return 0.0;
    }
    double ma = sa / count, mb = sb / count;
    double num = 0.0, da = 0.0, db = 0.0;
    for (int i = 0; i < n; i++) {
        if (a[i] > 0 && b[i] > 0) {
            num += (a[i] - ma) * (b[i] - mb);
            da += (a[i] - ma) * (a[i] - ma);
            db += (b[i] - mb) * (b[i] - mb);
        }
    }
    if (da == 0.0 || db == 0.0) {
        return 0.0;
    }
    return num / squareRoot(da * db);
}

/* sim이 0보다 큰 것 중 큰 순서로 최대 k개를 neighbors에 담고 개수를 돌려준다.
 * skip(자기 자신)은 뺀다. 같은 값이면 번호가 작은 쪽이 먼저다. */
int topNeighbors(const double sim[], int m, int skip, int k, int neighbors[]) {
    int used[MAX_N] = {0};
    int count = 0;
    while (count < k) {
        int best = -1;
        for (int v = 0; v < m; v++) {
            if (v == skip || used[v] || sim[v] <= 0.0) {
                continue;
            }
            if (best < 0 || sim[v] > sim[best]) {
                best = v;
            }
        }
        if (best < 0) {
            break;
        }
        used[best] = 1;
        neighbors[count++] = best;
    }
    return count;
}

/* 이웃들의 평점을 유사도로 가중 평균한다. 이웃이 안 본 아이템은 건너뛴다. */
double predictUserBased(const int ratings[][ITEMS], const double sim[],
                        const int neighbors[], int count, int item) {
    double num = 0.0, den = 0.0;
    for (int i = 0; i < count; i++) {
        int v = neighbors[i];
        if (ratings[v][item] > 0) {
            num += sim[v] * ratings[v][item];
            den += sim[v];
        }
    }
    return den == 0.0 ? 0.0 : num / den;
}

/* item 열과 target이 본 아이템 열들의 코사인을 sim에 적고,
 * 가장 비슷한 k개로 target 자신의 평점을 가중 평균한다. near에 고른 아이템을 담는다. */
double predictItemBased(const int ratings[][ITEMS], int m, int n, int target,
                        int item, int k, double sim[], int near[], int *nearCount) {
    int col[MAX_N], other[MAX_N];
    for (int u = 0; u < m; u++) {
        col[u] = ratings[u][item];
    }
    for (int j = 0; j < n; j++) {
        sim[j] = 0.0;
        if (j != item && ratings[target][j] > 0) {
            for (int u = 0; u < m; u++) {
                other[u] = ratings[u][j];
            }
            sim[j] = cosineSimilarity(col, other, m);
        }
    }
    *nearCount = topNeighbors(sim, n, item, k, near);
    double num = 0.0, den = 0.0;
    for (int i = 0; i < *nearCount; i++) {
        num += sim[near[i]] * ratings[target][near[i]];
        den += sim[near[i]];
    }
    return den == 0.0 ? 0.0 : num / den;
}

/* 1) 작은 행렬: 사용자 A~D, 아이템 1~4 */
static void slideExample(void) {
    const int r[4][4] = {
        {5, 3, 0, 0},
        {4, 0, 0, 0},
        {0, 0, 5, 4},
        {0, 0, 4, 0},
    };
    printf("toy: users A-D, items 1-4 (0 = not rated)\n");
    printf("cosine        A      B      C      D\n");
    for (int u = 0; u < 4; u++) {
        printf("  %c     ", 'A' + u);
        for (int v = 0; v < 4; v++) {
            printf(" %6.3f", cosineSimilarity(r[u], r[v], 4));
        }
        printf("\n");
    }
}

static void printRatings(const int r[][ITEMS]) {
    printf("ratings: users A-E, movies M1-M6 (0 = not rated)\n");
    printf("       M1  M2  M3  M4  M5  M6\n");
    for (int u = 0; u < USERS; u++) {
        printf("  %c  ", 'A' + u);
        for (int i = 0; i < ITEMS; i++) {
            printf("%4d", r[u][i]);
        }
        printf("\n");
    }
}

int main(void) {
    slideExample();

    /* 2) 사용자 5명 x 영화 6편. A의 M3·M6이 빈칸이다. */
    const int r[USERS][ITEMS] = {
        {5, 5, 0, 4, 2, 0}, /* A: 대상 사용자 */
        {5, 4, 5, 4, 3, 1}, /* B */
        {4, 4, 4, 3, 1, 2}, /* C */
        {1, 2, 1, 1, 4, 5}, /* D */
        {4, 3, 4, 4, 5, 5}, /* E */
    };
    const int target = 0, k = 3;
    const int blanks[] = {2, 5}; /* M3, M6 */
    double cosSim[USERS], pearSim[USERS];

    printf("\n");
    printRatings(r);

    printf("\nuser-based: similarity to A\n");
    printf("  user  cosine  pearson\n");
    for (int v = 0; v < USERS; v++) {
        cosSim[v] = cosineSimilarity(r[target], r[v], ITEMS);
        pearSim[v] = pearsonSimilarity(r[target], r[v], ITEMS);
        if (v != target) {
            printf("  %c     %6.3f   %6.3f\n", 'A' + v, cosSim[v], pearSim[v]);
        }
    }

    printf("\npredict A (k = %d, similarity > 0)\n", k);
    const char *names[2] = {"cosine ", "pearson"};
    const double *sims[2] = {cosSim, pearSim};
    double best = -1.0;
    int bestItem = -1;
    for (int s = 0; s < 2; s++) {
        int nb[USERS];
        int count = topNeighbors(sims[s], USERS, target, k, nb);
        printf("  %s: neighbors", names[s]);
        for (int i = 0; i < k; i++) {
            if (i < count) {
                printf(" %c", 'A' + nb[i]);
            } else {
                printf("  ");
            }
        }
        for (int b = 0; b < 2; b++) {
            double p = predictUserBased(r, sims[s], nb, count, blanks[b]);
            printf("  M%d = %.3f", blanks[b] + 1, p);
            if (s == 1 && p > best) {
                best = p;
                bestItem = blanks[b];
            }
        }
        printf("\n");
    }

    printf("\nitem-based: movies A rated, by cosine to the blank (k = 2)\n");
    for (int b = 0; b < 2; b++) {
        double sim[ITEMS];
        int near[ITEMS], nearCount, order[ITEMS];
        double p = predictItemBased(r, USERS, ITEMS, target, blanks[b], 2,
                                    sim, near, &nearCount);
        /* 비교한 아이템 전부를 유사도 순으로 보인다. */
        int rated = topNeighbors(sim, ITEMS, blanks[b], ITEMS, order);
        printf("  M%d:", blanks[b] + 1);
        for (int i = 0; i < rated; i++) {
            printf("  M%d %.3f", order[i] + 1, sim[order[i]]);
        }
        printf("\n      -> (");
        for (int i = 0; i < nearCount; i++) {
            printf("%s%.3f*%d", i ? " + " : "", sim[near[i]], r[target][near[i]]);
        }
        printf(") / (");
        for (int i = 0; i < nearCount; i++) {
            printf("%s%.3f", i ? " + " : "", sim[near[i]]);
        }
        printf(") = %.3f\n", p);
    }

    printf("\nrecommend to A (pearson): M%d (%.3f)\n", bestItem + 1, best);
    return 0;
}
