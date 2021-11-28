#include "detect.hpp"
// #include "subsrc/inQuad.cpp"

/** @function main */
int main( int argc, const char *argv[] )
{
  VideoCapture capture(0);
  Mat frame;
  if ( argc != 2 )
    argv[1] = "1";

  //-- 1. Load the cascades
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

  //-- 2. Read the video stream
  // capture.open( -1 );
  if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture 4\n"); return -1; }

  p1 = Point(400,100);
  p2 = Point(700,100);
  p3 = Point(600,400);
  p4 = Point(300,400);

  // while ( capture.read(frame) )
  while(1)
  {
    capture >> frame;
    if( frame.empty() )
    {
      printf(" --(!) No captured frame -- Break!");
      break;
    }

    // set the mouse callback function.
    setMouseCallback( window_name, onMouse, 0 );

    // 画点! 描的四边形的点
    circle( frame, p[0], 3, Scalar(0, 0, 0), -1, 8, 0 );
    circle( frame, p[1], 3, Scalar(0, 0, 0), -1, 8, 0 );
    circle( frame, p[2], 3, Scalar(0, 0, 0), -1, 8, 0 );
    circle( frame, p[3], 3, Scalar(0, 0, 0), -1, 8, 0 );

    //-- 3. Apply the classifier to the frame
    detectAndDisplay( frame, string(argv[1]) );
    imshow( window_name, frame );

    int c = waitKey(10);
    if( (char)c == 27 ) { break; } // escape
  }
  return 0;
}