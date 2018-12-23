/* Simple example of multithreading in C using pthreads. */

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#define THRESHOLD 6

int fib(int n) {
  if (n < 2)
    return n;
  return fib(n - 1) + fib(n - 2);
}

typedef struct {
  int n;
  int result;
} args_t;

typedef struct {
  pthread_t pid;
  args_t arg;
} fib_td_data_t;

void *fib_thread_wrapper(void *args) {
  ((args_t *)args)->result = fib(((args_t *)args)->n);
}

void fib_spawn_thread(int n, fib_td_data_t *td_data) {
  td_data->arg.n = n;
  pthread_create(&(td_data->pid), NULL, fib_thread_wrapper, &(td_data->arg));
}

void fib_multithreaded(int n, int tc, fib_td_data_t **free_td_data) {
  if (tc == 1 || n <= THRESHOLD) {
    fib_spawn_thread(n, (*free_td_data)++);
    return;
  }

  int half = tc / 2;
  fib_multithreaded(n - 1, tc - half, free_td_data);
  fib_multithreaded(n - 2, half, free_td_data);
}

int fib_threaded_main_wrapper(int n, int tc) {
  if (tc <= 1)
    return fib(n);

  if (n <= THRESHOLD)
    return fib(n);

  fib_td_data_t *tds = malloc(sizeof(fib_td_data_t) * tc);
  fib_td_data_t *last_free_td_data = tds;
  fib_multithreaded(n, tc, &last_free_td_data);
  int used_tds = last_free_td_data - tds;

  for (int i = 0; i < used_tds; i++)
    pthread_join(tds[i].pid, NULL);

  int result = 0;
  for (int i = 0; i < used_tds; i++)
    result += tds[i].arg.result;

  free(tds);
  return result;
}

int main(int argc, char **argv) {
  int n = atoi(argv[1]);

  if (argc >= 4 && argv[2][0] == 't') {
    int tc = atoi(argv[3]);
    int r1 = fib_threaded_main_wrapper(n, 8);
    assert(r1 == fib(n));
  } else {
    fib(n);
  }

  return 0;
}
