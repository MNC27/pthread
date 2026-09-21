#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *a;
int n, p;

typedef struct {
    int l;
    int r;
} Task;

Task *q;
int cap;
int hd = 0, tl = 0;
int cnt = 0;
int done = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

int part(int l, int r) {
    int x = a[r];
    int i = l - 1;

    for (int j = l; j < r; j++) {
        if (a[j] <= x) {
            i++;

            int t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }

    int t = a[i + 1];
    a[i + 1] = a[r];
    a[r] = t;

    return i + 1;
}

void push(Task x) {
    q[tl++] = x;
    cnt++;
}

Task pop() {
    Task x = q[hd++];
    cnt--;
    return x;
}

void *work(void *arg) {

    while (1) {

        pthread_mutex_lock(&mtx);

        while (cnt == 0 && !done)
            pthread_cond_wait(&cv, &mtx);

        if (done) {
            pthread_mutex_unlock(&mtx);
            break;
        }

        Task x = pop();

        pthread_mutex_unlock(&mtx);

        if (x.l >= x.r) {

            pthread_mutex_lock(&mtx);

            if (cnt == 0)
                done = 1;

            pthread_cond_broadcast(&cv);
            pthread_mutex_unlock(&mtx);

            continue;
        }

        int m = part(x.l, x.r);

        pthread_mutex_lock(&mtx);

        if (x.l < m - 1)
            push((Task){x.l, m - 1});

        if (m + 1 < x.r)
            push((Task){m + 1, x.r});

        if (cnt == 0)
            done = 1;

        pthread_cond_broadcast(&cv);

        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}

int main() {

    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    cap = 2 * n + 5;

    q = malloc(cap * sizeof(Task));

    pthread_t *th = malloc(p * sizeof(pthread_t));

    pthread_mutex_lock(&mtx);

    push((Task){0, n - 1});

    pthread_mutex_unlock(&mtx);

    for (int i = 0; i < p; i++)
        pthread_create(&th[i], NULL, work, NULL);

    pthread_cond_broadcast(&cv);

    for (int i = 0; i < p; i++)
        pthread_join(th[i], NULL);

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    free(a);
    free(q);
    free(th);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cv);

    return 0;
}