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
      if(iter == ITERATIONS-1) {
        collect(&s, &p, &frame_buffer, 0);
        frame_iter = frame_buffer;
        while(frame_iter != NULL) {
          state cur_state = *frame_iter;
          printf("sim:b:(%f, %f) p_l:%f p_r:%f    act:a_l:%d a_r:%d\n", cur_state.sim.b_x, cur_state.sim.b_y, cur_state.sim.p_l, cur_state.sim.p_r, cur_state.a_l, cur_state.a_r);
          frame_iter = cur_state.next;
        }
      }
      else {
        collect(&s, &p, &frame_buffer, EPSILON);
        frame_iter = frame_buffer;
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
