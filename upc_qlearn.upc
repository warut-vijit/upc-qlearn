#include <upc.h>
#include <stdio.h>
#include <unistd.h>
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
    simulator s;
    init(&s); 
    print(&s);
    step(&s, 1, -1);
    print(&s);
  }

  return 0;  
}
