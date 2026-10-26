/* 해시 함수 — 키를 배열 인덱스 [0, M-1]로 바꾼다.
 *
 * 정수 키는 나머지(모듈러 해싱), 문자열 키는 글자를 숫자로 바꿔 섞은 뒤
 * 나머지를 취한다. 마지막으로 키가 몇 개면 충돌이 생기는지 확률을 셈한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-08-hashing/01_hash_function/hashFunction.out
 */
#include <stdio.h>

/* 모듈러 해싱: 양의 정수 k를 M으로 나눈 나머지 */
int modularHash(int k, int m) {
    return k % m;
}

/* 나쁜 예: 글자 코드를 더하기만 한다. 순서가 다른 같은 글자들은 모두 같은 값 */
int sumHash(const char *s, int m) {
    int h = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        h = (h + (unsigned char)s[i]) % m;
    }
    return h;
}

/* 호너 방식: 자리마다 R을 곱해 순서를 반영한다. 매번 나머지를 취해 넘치지 않는다 */
int hornerHash(const char *s, int m) {
    const int R = 31;
    int h = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        h = (R * h + (unsigned char)s[i]) % m;
    }
    return h;
}

/* 키 n개를 m칸에 고르게 무작위로 넣을 때 적어도 한 번 충돌할 확률 */
double collisionChance(int n, int m) {
    double none = 1.0;
    for (int i = 0; i < n; i++) {
        none = none * (m - i) / m;
    }
    return 1.0 - none;
}

static void printBuckets(const int keys[], int n, int m) {
    int used[64] = {0};
    int count = 0;
    printf("  M = %d:", m);
    for (int i = 0; i < n; i++) {
        int h = modularHash(keys[i], m);
        printf(" %d", h);
        if (!used[h]) {
            used[h] = 1;
            count++;
        }
    }
    printf("  -> %d of %d buckets used\n", count, m);
}

int main(void) {
    /* 1. 글자 코드를 M = 8로 나눈 나머지 */
    const char letters[] = {'A', 'B', 'C', 'K'};
    int m = 8;
    printf("modular hashing, M = %d\n", m);
    for (int i = 0; i < 4; i++) {
        int h = modularHash(letters[i], m);
        printf("  %c (%d) -> %d", letters[i], letters[i], h);
        for (int j = 0; j < i; j++) {
            if (modularHash(letters[j], m) == h) {
                printf("  collides with %c", letters[j]);
                break;
            }
        }
        printf("\n");
    }

    /* 2. 4의 배수 키를 M = 8과 M = 7에 */
    int keys[8];
    printf("\nkeys");
    for (int i = 0; i < 8; i++) {
        keys[i] = 4 * i;
        printf(" %d", keys[i]);
    }
    printf("\n");
    printBuckets(keys, 8, 8);
    printBuckets(keys, 8, 7);

    /* 3. 같은 글자로 된 문자열 여섯 개, M = 97 */
    const char *words[] = {"stop", "pots", "tops", "spot", "opts", "post"};
    m = 97;
    printf("\nstring keys, M = %d\n", m);
    printf("  key   sum  horner\n");
    for (int i = 0; i < 6; i++) {
        printf("  %-4s %4d %7d\n", words[i], sumHash(words[i], m), hornerHash(words[i], m));
    }

    /* 4. 생일 문제 */
    const int cases[][2] = {{3, 16}, {5, 16}, {10, 16}, {23, 365}};
    printf("\nchance of at least one collision\n");
    for (int i = 0; i < 4; i++) {
        printf("  N = %2d, M = %3d: %5.1f%%\n", cases[i][0], cases[i][1],
               collisionChance(cases[i][0], cases[i][1]) * 100);
    }
    return 0;
}
