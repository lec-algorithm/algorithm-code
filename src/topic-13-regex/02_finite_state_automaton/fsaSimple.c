/* 유한 상태 오토마타 — 패턴 ab*c를 상태와 전이로 인식한다.
 *
 * 입력을 한 글자씩 읽으며 현재 상태를 옮긴다. S1에서는 입력 없이
 * S2로 넘어간다(ε 전이). 입력이 끝났을 때 S4에 있으면 수락한다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-13-regex/02_finite_state_automaton/fsaSimple.out
 */
#include <stdbool.h>
#include <stdio.h>

/* 상태 정의 */
typedef enum {
    S0 = 0,  /* 시작 상태 */
    S1 = 1,  /* 'a'를 읽은 상태 */
    S2 = 2,  /* b* 시작 상태 (0개의 'b' 허용) */
    S3 = 3,  /* 'b'를 1개 이상 읽은 상태 */
    S4 = 4   /* 'c'를 읽어서 패턴 매칭 완료 (Accept 상태) */
} State;

/* input 전체가 ab*c에 맞으면 true. 전이마다 한 줄 찍는다. */
bool accept(const char *input) {
    State currentState = S0;
    printf("  start S%d\n", currentState);

    for (int i = 0; input[i] != '\0'; i++) {
        char ch = input[i];
        State nextState = currentState;

        /* 상태 전이 규칙 */
        switch (currentState) {
        case S0:
            if (ch == 'a') {
                nextState = S1;
                printf("  '%c' -> S%d\n", ch, nextState);
                currentState = S2;            /* S1에서 ε 전이로 S2로 자동 이동 */
                printf("  eps -> S%d\n", currentState);
                continue;
            }
            printf("  '%c' -> no move from S%d: reject\n", ch, currentState);
            return false;
        case S2:                              /* b* 시작 상태 */
        case S3:                              /* 'b'를 1개 이상 읽은 상태 */
            if (ch == 'b') {
                nextState = S3;               /* S3에서는 제자리 */
            } else if (ch == 'c') {
                nextState = S4;
            } else {
                printf("  '%c' -> no move from S%d: reject\n", ch, currentState);
                return false;
            }
            break;
        default:                              /* S4: 수락 후에는 더 이상 입력 불가 */
            printf("  '%c' -> no move from S%d: reject\n", ch, currentState);
            return false;
        }

        printf("  '%c' -> S%d\n", ch, nextState);
        currentState = nextState;
    }

    /* 최종 상태가 수락 상태(S4)인지 확인 */
    bool result = (currentState == S4);
    printf("  final S%d: %s\n", currentState, result ? "accept" : "reject");
    return result;
}

int main(void) {
    const char *inputs[] = {"abc", "abbbbc", "ac", "ab", "abd", "abcc"};
    const int n = 6;
    bool results[6];

    printf("pattern ab*c\n");
    for (int i = 0; i < n; i++) {
        printf("\ninput \"%s\"\n", inputs[i]);
        results[i] = accept(inputs[i]);
    }

    printf("\nsummary\n");
    for (int i = 0; i < n; i++) {
        printf("  %-8s %s\n", inputs[i], results[i] ? "accept" : "reject");
    }
    return 0;
}
