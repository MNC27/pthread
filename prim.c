#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

int n, p;
int **g;

int *key;
int *par;
int *vis;

pthread_barrier_t bar;

typedef struct {
    int s;
    int e;
} Arg;

int mn;
int mv;

void *find_min(void *arg) {
    Arg *x = (Arg *)arg;

    int v = -1;
    int val = INT_MAX;

    for (int i = x->s; i < x->e; i++) {
        if (!vis[i] && key[i] < val) {
            val = key[i];
            v = i;
        }
    }

    if (v != -1) {
        static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

        pthread_mutex_lock(&mtx);

        if (key[v] < mn) {
            mn = key[v];
            mv = v;
        }

        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}

void *update(void *arg) {
    Arg *x = (Arg *)arg;

    for (int v = x->s; v < x->e; v++) {

        if (!vis[v] && g[mv][v] != 0 &&
            g[mv][v] < key[v]) {

            key[v] = g[mv][v];
            par[v] = mv;
        }
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    g = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        g[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++)
            scanf("%d", &g[i][j]);
    }

    key = malloc(n * sizeof(int));
    par = malloc(n * sizeof(int));
    vis = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        key[i] = INT_MAX;
        par[i] = -1;
    }

    key[0] = 0;

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    for (int it = 0; it < n; it++) {

        mn = INT_MAX;
        mv = -1;

        int z = (n + p - 1) / p;

        for (int i = 0; i < p; i++) {

            ar[i].s = i * z;
            ar[i].e = (i + 1) * z;

            if (ar[i].e > n)
                ar[i].e = n;

            pthread_create(&th[i], NULL, find_min, &ar[i]);
        }

        for (int i = 0; i < p; i++)
            pthread_join(th[i], NULL);

        if (mv == -1)
            break;

        vis[mv] = 1;

        for (int i = 0; i < p; i++)
            pthread_create(&th[i], NULL, update, &ar[i]);

        for (int i = 0; i < p; i++)
            pthread_join(th[i], NULL);
    }

    printf("Edges in MST:\n");

    int cost = 0;

    for (int i = 1; i < n; i++) {

        if (par[i] != -1) {
            printf("%d - %d : %d\n",
                   par[i], i, g[par[i]][i]);

            cost += g[par[i]][i];
        }
    }

    printf("Cost = %d\n", cost);

    for (int i = 0; i < n; i++)
        free(g[i]);

    free(g);
    free(key);
    free(par);
    free(vis);
    free(th);
    free(ar);

    return 0;
}