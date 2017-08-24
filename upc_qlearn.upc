#include <upc.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "qlearn.h"

int main()
{
  int i;
  char host_system_name[256];
  gethostname(host_system_name,256);

  for(i=0;i<THREADS;i++){
    if(i==MYTHREAD) printf("Hello, World!  from node %s (thread %d of %d)\n",host_system_name,MYTHREAD,THREADS-1);
  }
  if(MYTHREAD==THREADS-1){
    srand(time(NULL));
    simulator s;
    policy p;
    state *frame_iter, *frame_buffer;
    policy_init(&p);

    int iter;
    for(iter = 0; iter < ITERATIONS; iter++) {
      printf("beginning iteration #%d\n", iter+1);
      frame_buffer = NULL;
      sim_init(&s);

      // run through simulator, collect states
      collect(&s, &p, &frame_buffer);
      frame_iter = frame_buffer;
      while(frame_iter != NULL) {
        state cur_state = *frame_iter;
        frame_iter = cur_state.next;
      }

      // perform backprop and policy updates with SGD
      policy_backprop(&p, frame_buffer);

      // delete frame buffer
      state_delete(frame_buffer);
    }
    policy_delete(&p);
  }
  return 0; 
}
