/* 폭 우선 검색(BFS) — 큐로 가까운 정점부터 층별로 방문한다.
 *
 * 간선 목록으로 받은 그래프를 인접 행렬과 인접 목록으로 바꿔 찍고,
 * 정점 0에서 BFS를 돌리며 꺼낼 때마다 큐의 내용을 보인다.
 * 끝나면 from과 dist 표, 그리고 0에서 4까지의 최단 경로를 찍는다.
 *
 * 실행: 편집기 오른쪽 위 ▶ 버튼, 또는
 *   make src/topic-12-graph/01_bfs/bfs.out
 */
#include <stdio.h>

#define MAX_V 16

static int V = 0;
static int matrix[MAX_V][MAX_V]; /* 인접 행렬: 간선이 있으면 1 */
static int adj[MAX_V][MAX_V];    /* 인접 목록: adj[v]에 이웃이 deg[v]개 */
static int deg[MAX_V];

static int marked[MAX_V];
static int from[MAX_V];
static int dist[MAX_V];

/* 무방향 간선 하나를 행렬과 목록 양쪽에 넣는다. */
static void addEdge(int u, int v) {
    matrix[u][v] = matrix[v][u] = 1;
    adj[u][deg[u]++] = v;
    adj[v][deg[v]++] = u;
}

/* 인접 목록을 오름차순으로 정렬한다(삽입 정렬). 편의상 작은 번호부터 본다. */
static void sortLists(void) {
    for (int v = 0; v < V; v++) {
        for (int i = 1; i < deg[v]; i++) {
            int x = adj[v][i];
            int j = i - 1;
            while (j >= 0 && adj[v][j] > x) {
                adj[v][j + 1] = adj[v][j];
                j--;
            }
            adj[v][j + 1] = x;
        }
    }
}

static void printQueue(const int queue[], int head, int tail) {
    printf("queue [");
    for (int i = head; i < tail; i++) {
        printf(i > head ? " %d" : "%d", queue[i]);
    }
    printf("]\n");
}

/* s에서 BFS. 꺼낼 때마다 본 이웃과 새로 방문한 정점, 큐를 한 줄에 찍는다. */
void bfs(int s, int order[]) {
    int queue[MAX_V];
    int head = 0, tail = 0, n = 0;
    for (int v = 0; v < V; v++) {
        marked[v] = 0;
        from[v] = -1;
        dist[v] = -1;
    }
    marked[s] = 1;
    dist[s] = 0;
    queue[tail++] = s;
    printf("visit %d, ", s);
    printQueue(queue, head, tail);

    while (head < tail) {
        int v = queue[head++];
        int fresh[MAX_V];
        int k = 0;
        order[n++] = v;
        for (int i = 0; i < deg[v]; i++) {
            int w = adj[v][i];
            if (!marked[w]) {
                marked[w] = 1;
                from[w] = v;
                dist[w] = dist[v] + 1;
                queue[tail++] = w;
                fresh[k++] = w;
            }
        }
        printf("dequeue %d: check", v);
        for (int i = 0; i < deg[v]; i++) {
            printf(" %d", adj[v][i]);
        }
        if (k > 0) {
            printf(" -> visit");
            for (int i = 0; i < k; i++) {
                printf(" %d", fresh[i]);
            }
        }
        printf(", ");
        printQueue(queue, head, tail);
    }
}

/* from을 거슬러 올라가 s에서 t까지의 경로를 path에 채우고 길이를 돌려준다. */
int pathTo(int t, int path[]) {
    int rev[MAX_V];
    int n = 0;
    for (int x = t; x != -1; x = from[x]) {
        rev[n++] = x;
    }
    for (int i = 0; i < n; i++) {
        path[i] = rev[n - 1 - i];
    }
    return n;
}

int main(void) {
    /* 정점 7개, 간선 9개 */
    const int edges[][2] = {
        {0, 1}, {0, 2}, {0, 5}, {0, 6}, {2, 3}, {3, 4}, {3, 5}, {4, 5}, {4, 6},
    };
    const int E = (int)(sizeof edges / sizeof edges[0]);
    V = 7;
    for (int i = 0; i < E; i++) {
        addEdge(edges[i][0], edges[i][1]);
    }
    sortLists();

    printf("V = %d, E = %d\n\n", V, E);
    printf("adjacency matrix\n   ");
    for (int v = 0; v < V; v++) {
        printf(" %d", v);
    }
    printf("\n");
    for (int u = 0; u < V; u++) {
        printf("  %d", u);
        for (int v = 0; v < V; v++) {
            printf(" %d", matrix[u][v]);
        }
        printf("\n");
    }
    printf("\nadjacency list\n");
    for (int v = 0; v < V; v++) {
        printf("  %d:", v);
        for (int i = 0; i < deg[v]; i++) {
            printf(" %d", adj[v][i]);
        }
        printf("\n");
    }

    printf("\nBFS(0)\n");
    int order[MAX_V];
    bfs(0, order);
    printf("order:");
    for (int i = 0; i < V; i++) {
        printf(" %d", order[i]);
    }
    printf("\n\n  v  from  dist\n");
    for (int v = 0; v < V; v++) {
        if (from[v] == -1) {
            printf("  %d     -  %4d\n", v, dist[v]);
        } else {
            printf("  %d  %4d  %4d\n", v, from[v], dist[v]);
        }
    }

    int path[MAX_V];
    int n = pathTo(4, path);
    printf("\npath 0 -> 4:");
    for (int i = 0; i < n; i++) {
        printf(" %d", path[i]);
    }
    printf(" (%d edges)\n", dist[4]);
    return 0;
}
