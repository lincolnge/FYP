#include "opencv2/highgui.hpp"

// show the mouse position
int i = 0;

void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ );

void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
  if( event == EVENT_LBUTTONDOWN )
  {
    point = Point2f((float)x, (float)y);
    // printf("%d %d \n", x, y);
    // set point
    if ( i < 4 )
    {
      p[i] = Point(x,y);
      i = i + 1;
    }
  }
}