#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

pid_t *child_pids;
int pnum;

int pipefd[2];
int pipefd2[2];

void kill_proc(int sig) {
  if (sig == SIGALRM) {
    for (int i = 0; i < pnum; i++) {
      kill(child_pids[i], SIGKILL);
    }
    printf("\nEND\n");
  }
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  pnum = -1;
  int timeout = 0;
  bool with_files = false;
  pipe(pipefd);

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"timeout", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if (seed <= 0){
              printf("seed is a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (array_size <= 0){
              printf("array_size is a positive number\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (pnum <= 0){
              printf("pnum is a positive number\n");
              return 1;
            }
            break;
          case 3:
            timeout = atoi(optarg);
            // your code here
            // error handling
            if (timeout < 0){
              printf("timeout is a non-negative number\n");
              return 1;
            }
            break;
          case 4:
            with_files = true;
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
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" --timeout \"num\" \n",
           argv[0]);
    return 1;
  }

  alarm(timeout);

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  FILE *outfile;
  outfile=fopen("tmpfile.txt", "w");

  child_pids = malloc(sizeof(pid_t) * pnum);
  pipe(pipefd2);
  // close(pipefd2[0]);
  // printf("Time:");

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        int time;
        // close(pipefd2[0]);
        read(pipefd2[0], &time, sizeof(time));
        printf("%d ", time);
        sleep(time);
        // parallel somehow
        struct MinMax min_max = GetMinMax(array, (array_size*i)/pnum, (array_size*(i+1))/pnum);

        if (with_files) {
          // use files here
          fprintf(outfile, "%d %d\n", min_max.min, min_max.max);
        } else {
          // use pipe here
          close(pipefd[0]);
          write(pipefd[1], &min_max.max, sizeof(min_max.max));
          write(pipefd[1], &min_max.min, sizeof(min_max.min));
          // close(pipefd[1]);
        }
        return 0;
      }
      else{
        // parent process
        for (int j = 0; j < i; j++) {
          write(pipefd2[1], &j, sizeof(j));
        }
        child_pids[i] = child_pid;
        signal(SIGALRM, kill_proc);
      }
    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  while (active_child_processes > 0) {
    wait(NULL);
    // signal(SIGALRM, kill_proc);
    active_child_processes -= 1;
  }

  struct pollfd pfd[1];
    pfd[0].fd = pipefd[0];
    pfd[0].events = POLLIN;

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  outfile = fopen("tmpfile.txt", "r");

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
      fscanf(outfile, "%d %d", &min, &max);
    } else {
      // read from pipes
      if (poll(pfd, 1, 0) != 0) {
        read(pipefd[0], &max, sizeof(max));
        read(pipefd[0], &min, sizeof(min));
      }
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }
  
  fclose(outfile);

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  if (min_max.max == INT_MIN || min_max.min == INT_MAX) {
    printf("\nResults are not found\n");
    return 0;
  }
  printf("\nMin: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
