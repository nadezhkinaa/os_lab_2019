#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t pthread_mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pthread_mutex_2 = PTHREAD_MUTEX_INITIALIZER;

void first_method(void* _) {
  while (true) {
    printf("first: thread 1 is free\n");

    pthread_mutex_lock(&pthread_mutex_1);
    printf("first: thread 1 locked\n");

    sleep(4);

    pthread_mutex_lock(&pthread_mutex_2);
    printf("first: thread 2 locked\n");

    pthread_mutex_unlock(&pthread_mutex_1);
    pthread_mutex_unlock(&pthread_mutex_2);
    printf("all unlocked!!!\n");
  }
}

void second_metod(void* _) {
  while (true) {
    printf("second: thread 2 is lock\n");

    pthread_mutex_lock(&pthread_mutex_2);
    printf("second: thread 2 locked\n");

    sleep(4);

    pthread_mutex_lock(&pthread_mutex_1);
    printf("second: thread 1 locked\n");

    pthread_mutex_unlock(&pthread_mutex_2);
    pthread_mutex_unlock(&pthread_mutex_1);
    printf("all unlocked!!!\n");
  }
}

int main() {
  pthread_t pthread1, pthread2;
  pthread_create(&pthread1, NULL, (void *)first_method, NULL);
  pthread_create(&pthread2, NULL, (void *) second_metod, NULL);
  
  pthread_join(pthread1, NULL);
  pthread_join(pthread2, NULL);
  return 0;
}