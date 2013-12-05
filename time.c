#include "stdio.h"
#include "stdlib.h" 
#include "time.h"
int main( void ) 
{ 
  clock_t start, finish; 
  double duration; 
  start = clock();
  printf( "%d seconds\n", (int)duration ); 
  for(;;){
    finish = clock(); 
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    if (duration == (int)duration)
      printf( "%d seconds\n", (int)duration%60 ); 
  }
} 