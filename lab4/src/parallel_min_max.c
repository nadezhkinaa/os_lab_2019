#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int timeout = -1; //Таймаут по умолчанию

void handle_timeout(int sig){
  // Посылаем SIGKILL всем дочерним процессам
  if (sig == SIGALRM){
    printf("Timeout reached! Terminating child processes...\n");
    kill(0, SIGKILL);
  }
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;
  int timeout=-1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed < 0){
              printf("seed must be > 0");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size < 0){
              printf("array_size must be >0");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
             if(pnum < 0){
              printf("pnum must be >0");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout= atoi(optarg);
             if(timeout < 0){
              printf("timeout must be >0");
              return 1;
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);


  pid_t process_ids[pnum];
  int descriptors_channel[pnum][2];
  int process_id_size = array_size / pnum;

if (timeout !=-1) {
    signal(SIGALRM, handle_timeout);
    alarm(timeout);
    printf("Begin timeout %ds\n", timeout);
  }

  if (!with_files){
    for (int i = 0; i < pnum; i++) {
        if (pipe(descriptors_channel[i]) == -1) {
            printf("pipe failed, number is %d", i);
            return 1;
  }
    }
  }

  for (int i = 0; i < pnum; i++) {
    process_ids[i] = fork();
    pid_t child_pid = process_ids[i];

    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process

        int process_ids_start = process_id_size * i;
        int process_ids_end = (i == pnum - 1) ? array_size : process_ids_start + process_id_size;
        struct MinMax process_id_min_max = GetMinMax(array, process_ids_start, process_ids_end);
        if (with_files) {
          char filename[256];
          snprintf(filename, sizeof(filename), "%s%d.txt", "result_", i);
          FILE *file = fopen(filename, "w");
          if (file == NULL) {
 printf("fopen failed, number is %d\n", i);
              return 1;
          }
          fprintf(file, "%d %d\n", process_id_min_max.min, process_id_min_max.max);
          fclose(file);
        } else {
          close(descriptors_channel[i][0]); 
          write(descriptors_channel[i][1], &process_id_min_max.min, sizeof(int));
          write(descriptors_channel[i][1], &process_id_min_max.max, sizeof(int));
          close(descriptors_channel[i][1]); 
          close(descriptors_channel[i][1]);
        }
        return 0;
      }}
     else {
      printf("Fork failed!\n");
      return 1;
    }
  }


 int status;
  int i = 0;
  while (active_child_processes > 0) {
    waitpid(process_ids[i], &status, 0);
    if (!WIFEXITED(status)) {
      printf("Child process %d did not exit normally\n", i);
    }
    i++;

    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      char fname[256];
      snprintf(fname, sizeof(fname), "%s%d.txt", "result_", i);
      FILE *file = fopen(fname, "r");
      if (file == NULL) {
          printf("fopen (r) failed, number is %d\n", i);
          continue;
      }
      fscanf(file, "%d %d", &min, &max);
      fclose(file);
      remove(fname);
    } else {
      close(descriptors_channel[i][1]);  
      read(descriptors_channel[i][0], &min, sizeof(int));
      read(descriptors_channel[i][0], &max, sizeof(int));
      close(descriptors_channel[i][0]);  
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
    
