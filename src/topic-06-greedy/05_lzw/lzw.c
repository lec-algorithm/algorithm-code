/* LZW 압축 — 반복되는 문자열에 코드를 붙여 가며 압축한다.
 *
 * 인코더는 읽으면서 문자열 표를 만들고, 디코더는 코드만 보고 같은 표를
 * 스스로 다시 만든다. 표는 보내지 않는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-06-greedy/05_lzw/lzw.out
 */
#include <stdio.h>
#include <string.h>

#define MAX_CODES 4096
#define MAX_LEN 64
#define MAX_TEXT 256

static char table[MAX_CODES][MAX_LEN];
static int tableSize = 0;

/* 단일 문자 256개로 표를 초기화한다. 코드 0~255는 ASCII다. */
static void initTable(void) {
    for (int c = 0; c < 256; c++) {
        table[c][0] = (char)c;
        table[c][1] = '\0';
    }
    tableSize = 256;
}

/* s의 코드를 돌려준다. 표에 없으면 -1. */
static int findCode(const char *s) {
    for (int i = 0; i < tableSize; i++) {
        if (strcmp(table[i], s) == 0) {
            return i;
        }
    }
    return -1;
}

static int addEntry(const char *s) {
    strcpy(table[tableSize], s);
    return tableSize++;
}

/* text를 코드 목록으로 바꾸고 코드 개수를 돌려준다. 걸음마다 한 줄 찍는다. */
int lzwEncode(const char *text, int codes[]) {
    char p[MAX_LEN];
    char pc[MAX_LEN];
    int n = 0;

    initTable();
    p[0] = text[0];
    p[1] = '\0';
    printf("  %-5s %-3s %-5s %s\n", "P", "C", "P+C", "action");
    for (const char *t = text + 1; *t; t++) {
        snprintf(pc, MAX_LEN, "%s%c", p, *t);
        printf("  %-5s %-3c %-5s ", p, *t, pc);
        if (findCode(pc) != -1) {
            printf("in table, P = %s\n", pc);
            strcpy(p, pc);
        } else {
            codes[n++] = findCode(p);
            int added = addEntry(pc);
            printf("out %d (%s), add %d = %s\n", codes[n - 1], p, added, pc);
            p[0] = *t;
            p[1] = '\0';
        }
    }
    codes[n++] = findCode(p);
    printf("  %-5s %-3s %-5s out %d (%s)\n", p, "", "", codes[n - 1], p);
    return n;
}

/* 코드 목록을 원래 문자열로 되돌린다. 걸음마다 한 줄 찍는다. */
void lzwDecode(const int codes[], int n, char out[]) {
    char s[MAX_LEN];
    char entry[MAX_LEN];
    int old = codes[0];

    initTable();
    strcpy(out, table[old]);
    char c = table[old][0];
    printf("  %-5s %-5s %-5s %-3s %s\n", "OLD", "NEW", "S", "C", "add");
    printf("  %-5s %-5s %s\n", "", "", table[old]);
    for (int i = 1; i < n; i++) {
        int code = codes[i];
        int known = code < tableSize;
        if (!known) {
            snprintf(s, MAX_LEN, "%s%c", table[old], c);
        } else {
            strcpy(s, table[code]);
        }
        strcat(out, s);
        c = s[0];
        snprintf(entry, MAX_LEN, "%s%c", table[old], c);
        int added = addEntry(entry);
        printf("  %-5d %-5d %-5s %-3c %d = %s%s\n", old, code, s, c, added, entry,
               known ? "" : "  (NEW not in table yet)");
        old = code;
    }
}

int main(void) {
    const char *text = "BABAABAAA";
    int codes[MAX_TEXT];
    char decoded[MAX_TEXT];

    printf("text = %s\n\nencode\n", text);
    int n = lzwEncode(text, codes);

    printf("codes:");
    for (int i = 0; i < n; i++) {
        printf(" %d", codes[i]);
    }
    printf("\n\ndecode\n");
    lzwDecode(codes, n, decoded);
    printf("decoded = %s\n", decoded);

    int length = (int)strlen(text);
    printf("\nbits: ascii %d x 8 = %d, lzw %d x 9 = %d\n",
           length, 8 * length, n, 9 * n);
    return 0;
}
