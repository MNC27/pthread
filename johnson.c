#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define INF LLONG_MAX / 4

typedef long long ll;

int n, m, p;

typedef struct {
    int u;
    int v;
    ll w;
} Edge;

Edge *e;

ll *h;
ll **d;

typedef struct {
    int s;
    int en;
} Arg;

void bellman_ford() {
    h = calloc(n, sizeof(ll));

    for (int i = 0; i < n; i++) {

        int change = 0;

        for (int j = 0; j < m; j++) {

            int u = e[j].u;
            int v = e[j].v;
            ll w = e[j].w;

            if (h[u] + w < h[v]) {
                h[v] = h[u] + w;
                change = 1;
            }
        }

        if (!change)
            break;

        if (i == n - 1) {
            printf("Negative cycle exists\n");
            exit(0);
        }
    }
}

void dijkstra(int src) {
    ll *ds = malloc(n * sizeof(ll));
    int *vis = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        ds[i] = INF;

    ds[src] = 0;

    for (int it = 0; it < n; it++) {

        int u = -1;
        ll mn = INF;

        for (int i = 0; i < n; i++) {

            if (!vis[i] && ds[i] < mn) {
                mn = ds[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        vis[u] = 1;

        for (int j = 0; j < m; j++) {

            if (e[j].u != u)
                continue;

            int v = e[j].v;

            ll w = e[j].w + h[u] - h[v];

            if (ds[u] + w < ds[v])
                ds[v] = ds[u] + w;
        }
    }

    for (int v = 0; v < n; v++) {

        if (ds[v] >= INF)
            d[src][v] = INF;

        else
            d[src][v] = ds[v] - h[src] + h[v];
    }

    free(ds);
    free(vis);
}

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int src = x->s; src < x->en; src++)
        dijkstra(src);

    return NULL;
}

int main() {

    scanf("%d %d %d", &n, &m, &p);

    e = malloc(m * sizeof(Edge));

    for (int i = 0; i < m; i++) {
        scanf("%d %d %lld",
              &e[i].u,
              &e[i].v,
              &e[i].w);
    }

    bellman_ford();

    d = malloc(n * sizeof(ll *));

    for (int i = 0; i < n; i++)
        d[i] = malloc(n * sizeof(ll));

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    int z = (n + p - 1) / p;

    for (int i = 0; i < p; i++) {

        ar[i].s = i * z;
        ar[i].en = (i + 1) * z;

        if (ar[i].en > n)
            ar[i].en = n;

        pthread_create(&th[i],
                       NULL,
                       work,
                       &ar[i]);
    }

    for (int i = 0; i < p; i++)
        pthread_join(th[i], NULL);

    printf("All-Pairs Shortest Paths:\n");

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) {

            if (d[i][j] >= INF)
                printf("INF ");

            else
                printf("%lld ", d[i][j]);
        }

        printf("\n");
    }

    for (int i = 0; i < n; i++)
        free(d[i]);

    free(d);
    free(e);
    free(h);
    free(th);
    free(ar);

    return 0;
}