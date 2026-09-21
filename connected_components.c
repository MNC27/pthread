#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, m, p;

int **g;
int *deg;

int *vis;
int *comp;

int *q;
int hd, tl;

int cur;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int id;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    while (1) {
        int u = -1;

        pthread_mutex_lock(&mtx);

        if (hd < tl) {
            u = q[hd++];
            comp[u] = cur;
        }

        pthread_mutex_unlock(&mtx);

        if (u == -1)
            break;

        for (int i = 0; i < deg[u]; i++) {

            int v = g[u][i];

            pthread_mutex_lock(&mtx);

            if (!vis[v]) {
                vis[v] = 1;
                q[tl++] = v;
            }

            pthread_mutex_unlock(&mtx);
        }
    }

    return NULL;
}

int main() {
    scanf("%d %d %d", &n, &m, &p);

    g = malloc(n * sizeof(int *));
    deg = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        g[i] = malloc(n * sizeof(int));

    for (int i = 0; i < m; i++) {
        int u, v;

        scanf("%d %d", &u, &v);

        g[u][deg[u]++] = v;
        g[v][deg[v]++] = u;
    }

    vis = calloc(n, sizeof(int));
    comp = malloc(n * sizeof(int));
    q = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        comp[i] = -1;

    cur = 0;

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    for (int s = 0; s < n; s++) {

        if (vis[s])
            continue;

        hd = 0;
        tl = 0;

        q[tl++] = s;
        vis[s] = 1;

        for (int i = 0; i < p; i++) {
            ar[i].id = i;
            pthread_create(&th[i], NULL, work, &ar[i]);
        }

        for (int i = 0; i < p; i++)
            pthread_join(th[i], NULL);

        cur++;
    }

    printf("Number of connected components = %d\n", cur);

    for (int i = 0; i < n; i++)
        printf("Vertex %d -> Component %d\n", i, comp[i]);

    for (int i = 0; i < n; i++)
        free(g[i]);

    free(g);
    free(deg);
    free(vis);
    free(comp);
    free(q);
    free(th);
    free(ar);

    pthread_mutex_destroy(&mtx);

    return 0;
}