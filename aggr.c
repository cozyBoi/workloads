/* aggr.c
 * do aggregation for 8G dataset
 * memory bound workload
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

const int block_size = 16000000, block_cnt = 500; //8G
const int num_cnt_per_blk = block_size / 4;
const int iter = 100;

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

int main(){
  int i, j, it;
  int* nums[block_cnt + 1];
  struct timespec start, finish, delta;

  srand(time(NULL));

  clock_gettime(CLOCK_REALTIME, &start);
  for(i=0; i<block_cnt; i++){
    nums[i] = malloc(block_size);
  }

  for(i=0; i<block_cnt; i++){
    for(j=0; j<num_cnt_per_blk; j++){
      nums[i][j] = rand() % 1000000;
    }
  }

  int sum = 0;

  for(it=0; it<iter; it++){
    for(i=0; i<block_cnt; i++){
      for(j=0; j<num_cnt_per_blk; j++){
        sum += nums[i][j] % 1000000000;
      }
    } 
  }

   clock_gettime(CLOCK_REALTIME, &finish);
   sub_timespec(start, finish, &delta);

   fprintf(stdout, "aggr %ld.%.9ld\n", delta.tv_sec, delta.tv_nsec);

  return 0;
}
