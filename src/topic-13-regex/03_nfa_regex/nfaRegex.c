/* 정규식 → NFA — 정규식에서 ε 전이 그래프를 만들고 상태 집합으로 시뮬레이션한다.
 *
 * 상태 i는 정규식의 i번째 글자다. 글자 상태는 그 글자를 읽고 i+1로 가고,
 * 괄호·'*'·'|'는 ε 전이를 만든다. 텍스트를 읽을 때는 "있을 수 있는 상태
 * 전부"를 집합으로 들고 다니므로 되돌아가지 않는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-13-regex/03_nfa_regex/nfaRegex.out
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_STATES 128
#define MAX_EDGES 512

static char re[MAX_STATES];      /* 괄호로 감싼 정규식. 상태 i = re[i] */
static int M;                    /* 수락 상태 번호 = 정규식 길이 */
static int edgeFrom[MAX_EDGES];  /* ε 전이 목록 */
static int edgeTo[MAX_EDGES];
static int edgeCount;
static long g_work;              /* 집합에 넣은 상태 수의 합 */

static void addEdge(int v, int w) {
    edgeFrom[edgeCount] = v;
    edgeTo[edgeCount] = w;
    edgeCount++;
}

/* regex를 괄호로 감싸 re에 두고 ε 전이 그래프를 만든다. */
void buildNfa(const char *regex) {
    int ops[MAX_STATES];
    int top = 0;

    snprintf(re, sizeof re, "(%s)", regex);
    M = (int)strlen(re);
    edgeCount = 0;
    for (int i = 0; i < M; i++) {
        int lp = i;                               /* '*'가 붙을 덩어리의 시작 */
        if (re[i] == '(' || re[i] == '|') {
            ops[top++] = i;
        } else if (re[i] == ')') {
            int orPos = ops[--top];
            if (re[orPos] == '|') {
                lp = ops[--top];                  /* 짝이 되는 '(' */
                addEdge(lp, orPos + 1);           /* 왼쪽 갈래를 건너뛰고 오른쪽으로 */
                addEdge(orPos, i);                /* 왼쪽 갈래 끝에서 ')'로 */
            } else {
                lp = orPos;
            }
        }
        if (i < M - 1 && re[i + 1] == '*') {
            addEdge(lp, i + 1);                   /* 0번 (건너뛰기) */
            addEdge(i + 1, lp);                   /* 한 번 더 (되풀이) */
        }
        if (re[i] == '(' || re[i] == '*' || re[i] == ')') {
            addEdge(i, i + 1);                    /* 기호는 글자 없이 다음으로 */
        }
    }
}

static void dfs(int v, bool marked[]) {
    marked[v] = true;
    g_work++;
    for (int e = 0; e < edgeCount; e++) {
        if (edgeFrom[e] == v && !marked[edgeTo[e]]) {
            dfs(edgeTo[e], marked);
        }
    }
}

/* in에서 ε 전이만 따라 갈 수 있는 상태 전부를 out에 표시한다. */
void closure(const bool in[], bool out[]) {
    memset(out, 0, sizeof(bool) * (MAX_STATES + 1));
    for (int v = 0; v <= M; v++) {
        if (in[v] && !out[v]) {
            dfs(v, out);
        }
    }
}

static bool isLiteral(char c) {
    return c != '(' && c != ')' && c != '*' && c != '|';
}

static void printSet(const char *label, const bool set[]) {
    printf("  %-10s {", label);
    bool first = true;
    for (int v = 0; v <= M; v++) {
        if (set[v]) {
            printf(first ? "%d" : " %d", v);
            first = false;
        }
    }
    printf("}\n");
}

/* text 전체가 정규식에 맞으면 true. trace면 글자마다 상태 집합을 찍는다. */
bool recognizes(const char *text, bool trace) {
    bool start[MAX_STATES + 1] = {false};
    bool pc[MAX_STATES + 1];
    bool match[MAX_STATES + 1];

    start[0] = true;
    closure(start, pc);
    if (trace) {
        printSet("start", pc);
    }
    for (int k = 0; text[k] != '\0'; k++) {
        memset(match, 0, sizeof match);
        for (int i = 0; i < M; i++) {
            if (pc[i] && isLiteral(re[i]) && (re[i] == text[k] || re[i] == '.')) {
                match[i + 1] = true;              /* 글자를 읽고 i+1로 */
            }
        }
        closure(match, pc);
        if (trace) {
            char label[16];
            snprintf(label, sizeof label, "read '%c'", text[k]);
            printSet(label, match);
            printSet("closure", pc);
        }
    }
    return pc[M];
}

int main(void) {
    /* 1) 3의 배수를 이진수로 나타내는 정규식 */
    const char *regex = "(0|1(01*0)*1)*";
    buildNfa(regex);
    printf("regex = %s\n", regex);
    printf("nfa   = %s, states 0..%d, accept %d\n\n", re, M, M);
    printf("  state");
    for (int i = 0; i <= M; i++) {
        printf("%3d", i);
    }
    printf("\n  char ");
    for (int i = 0; i < M; i++) {
        printf("%3c", re[i]);
    }
    printf("\n\n  eps  ");
    for (int e = 0; e < edgeCount; e++) {
        printf(" %d->%d", edgeFrom[e], edgeTo[e]);
    }
    printf("\n\ntext = 1001\n");
    bool ok = recognizes("1001", true);
    printf("  -> %s\n\n", ok ? "accept" : "reject");

    const char *texts[] = {"11", "110", "1001", "1100", "10", "1011", "10000"};
    printf("  %-6s %5s  %s\n", "text", "value", "result");
    for (int t = 0; t < 7; t++) {
        int value = 0;
        for (const char *p = texts[t]; *p; p++) {
            value = 2 * value + (*p - '0');
        }
        printf("  %-6s %5d  %s\n", texts[t], value,
               recognizes(texts[t], false) ? "accept" : "reject");
    }

    /* 2) 01의 백트래킹이 폭발하던 입력: (a*)^n b 와 a^n */
    printf("\nblow-up: (a*)^n b on a^n\n");
    printf("  %-3s %4s %4s  %-9s %6s\n", "n", "M", "N", "result", "work");
    for (int n = 2; n <= 10; n += 2) {
        char pattern[64] = "";
        char text[64] = "";
        for (int k = 0; k < n; k++) {
            strcat(pattern, "a*");
            strcat(text, "a");
        }
        strcat(pattern, "b");
        buildNfa(pattern);
        g_work = 0;
        bool found = recognizes(text, false);
        printf("  %-3d %4d %4d  %-9s %6ld\n", n, M, n, found ? "match" : "no match",
               g_work);
    }
    return 0;
}
