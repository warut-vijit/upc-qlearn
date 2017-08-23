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
    state *frame_iter, *frame_buffer = NULL;
    init(&s, &p);
    collect(&s, &p, &frame_buffer);
    frame_iter = frame_buffer;
    int i;
    while(frame_iter != NULL) {
      state cur_state = *frame_iter;
      //printf("b:(%f, %f) a_l:%d a_r:%d r:(%d, %d)\n", cur_state.sim.b_x, cur_state.sim.b_y, cur_state.a_l, cur_state.a_r, cur_state.reward.first, cur_state.reward.second);
      frame_iter = cur_state.next;
    }
    state_delete(frame_buffer); 
  }
  return 0; 
}
