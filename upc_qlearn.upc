#include <upc.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  int i;
  char host_system_name[256];
  gethostname(host_system_name,256);

  for(i=0; i<THREADS; i++){
    if(MYTHREAD==i){
      printf("Hello, World!  from node %s (thread %d of %d)\n",host_system_name,MYTHREAD,THREADS-1);
    }
  }
  return 0;  
}
