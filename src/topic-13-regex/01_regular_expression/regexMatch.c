/* 정규식 매칭 — 재귀와 백트래킹으로 '.'과 '*'를 처리한다.
 *
 * Match는 패턴을 한 글자씩 맞춰 나가고, 'x*'를 만나면 MatchStar가
 * x를 0개, 1개, 2개, ... 먹어 보며 나머지를 다시 시도한다.
 * 비교 횟수(Match 호출 수)와 최대 재귀 깊이를 함께 센다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-13-regex/01_regular_expression/regexMatch.out
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static long g_comparisons = 0;      /* Match 호출 수 */
static int g_currentRecursion = 0;  /* 지금 재귀 깊이 */
static int g_maxRecursion = 0;      /* 가장 깊었던 재귀 깊이 */
static bool g_trace = false;        /* true면 Match 호출마다 한 줄 찍는다 */

static bool matchStar(char c, const char *pattern, const char *text);

static void enter(void) {
    g_currentRecursion++;
    if (g_currentRecursion > g_maxRecursion) {
        g_maxRecursion = g_currentRecursion;
    }
}

/* text의 앞부분이 pattern에 맞으면 true */
bool match(const char *pattern, const char *text) {
    g_comparisons++;
    enter();
    if (g_trace) {
        printf("  %*smatch(\"%s\", \"%s\")\n", 2 * g_currentRecursion, "", pattern, text);
    }

    bool result;
    if (*pattern == '\0') {                       /* 패턴이 끝나면 성공 */
        result = true;
    } else if (*(pattern + 1) == '*') {           /* 다음 글자가 '*' (x* 패턴) */
        result = matchStar(*pattern, pattern + 2, text);
    } else if (*text != '\0' && (*pattern == '.' || *pattern == *text)) {
        result = match(pattern + 1, text + 1);    /* 한 글자 맞추고 전진 */
    } else {
        result = false;
    }
    g_currentRecursion--;
    return result;
}

/* c를 0개 이상 먹은 뒤 나머지 pattern이 맞으면 true */
static bool matchStar(char c, const char *pattern, const char *text) {
    enter();
    bool result = false;
    if (match(pattern, text)) {                   /* 0개 매칭 시도 */
        result = true;
    } else {
        while (*text != '\0' && (*text == c || c == '.')) {   /* 1개 이상 */
            text++;
            if (match(pattern, text)) {
                result = true;
                break;
            }
        }
    }
    g_currentRecursion--;
    return result;
}

/* pattern이 처음 맞는 위치. 없으면 -1. 정규식 .*pattern 과 같다. */
int search(const char *pattern, const char *text) {
    int n = (int)strlen(text);
    for (int i = 0; i <= n; i++) {
        if (g_trace) {
            printf("  start %d\n", i);
        }
        if (match(pattern, text + i)) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    /* 1) 호출 하나하나를 찍어 본다. 들여쓰기가 재귀 깊이다. */
    printf("search(\"ab*c\", \"aabbc\")\n");
    g_trace = true;
    g_comparisons = 0;
    int at = search("ab*c", "aabbc");
    g_trace = false;
    printf("  -> found at %d, comparisons = %ld\n\n", at, g_comparisons);

    /* 2) 여러 패턴과 텍스트 */
    const char *tests[][2] = {
        {"ab*c", "ac"},     {"ab*c", "abbbbc"},    {"ab*c", "abd"},
        {"a.c", "xxabcxx"}, {".*ab*c", "zzabbbcz"},
    };
    printf("tests\n");
    printf("  %-8s %-10s %3s %12s\n", "pattern", "text", "at", "comparisons");
    for (int i = 0; i < 5; i++) {
        g_comparisons = 0;
        int found = search(tests[i][0], tests[i][1]);
        printf("  %-8s %-10s %3d %12ld\n", tests[i][0], tests[i][1], found, g_comparisons);
    }

    /* 3) 'a*'를 n번 이은 뒤 b. 텍스트는 a를 n개. 맞지 않는데, 그걸 알기까지 */
    printf("\nblow-up: match((a*)^n b, a^n)\n");
    printf("  %-3s %-12s %12s %10s\n", "n", "result", "comparisons", "max depth");
    for (int n = 2; n <= 10; n += 2) {
        char pattern[64] = "";
        char text[64] = "";
        for (int k = 0; k < n; k++) {
            strcat(pattern, "a*");
            strcat(text, "a");
        }
        strcat(pattern, "b");
        g_comparisons = 0;
        g_maxRecursion = 0;
        bool ok = match(pattern, text);
        printf("  %-3d %-12s %12ld %10d\n", n, ok ? "match" : "no match", g_comparisons,
               g_maxRecursion);
    }
    return 0;
}
