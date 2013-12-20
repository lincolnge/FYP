#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame, String argv );

/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

String windows_text = "";
Rect unsafe_area = Rect(400, 400, 200, 200);
String box_text = format("UNSAFE AREA");

/** @function main */
int main( int argc, const char *argv[] )
{
  VideoCapture capture;
  Mat frame;
  if ( argc != 2 )
    argv[1] = "1";
  
  //-- 1. Load the cascades
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
  if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

  //-- 2. Read the video stream
  capture.open( -1 );
  if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

  while ( capture.read(frame) )
  {
    if( frame.empty() )
    {
      printf(" --(!) No captured frame -- Break!");
      break;
    }

    //-- 3. Apply the classifier to the frame
    detectAndDisplay( frame, string(argv[1]) );

    int c = waitKey(10);
    if( (char)c == 27 ) { break; } // escape
  }
  return 0;
}

void liner_equation( Point p )
{
  Point p1 = Point(300,100);
  Point p4 = Point(600,100);
  Point p3 = Point(500,400);
  Point p2 = Point(200,400);

  float b = 0.00, k = 0.00;
  b = ( p1.y * p2.x - p2.y * p1.x ) / ( p2.x - p1.x );
  k = ( p1.y - p2.y ) / ( p1.x - p2.x );
  float A1 = k, B1 = -1, C1 = b;
  b = ( p3.y * p4.x - p4.y * p3.x ) / ( p4.x - p3.x );
  k = ( p3.y - p4.y ) / ( p3.x - p4.x );
  float A2 = k, B2 = -1, C2 = b;

  if (( A1 * p.x + B1 * p.y + C1 ) * ( A2 * p.x + B2 * p.y + C2 ) < 0 || true )
    printf("It is on the inside of parallelogram!");
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame, String argv )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  bool isNotWarning = 0;
  bool isSafe = 0;

  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
  
  Point p1 = Point(400,100);
  Point p2 = Point(700,100);
  Point p3 = Point(600,400);
  Point p4 = Point(300,400);

  vector<Point> contour;
  contour.push_back(p1);  // X-axis, Y-axis
  contour.push_back(p2);
  contour.push_back(p3);
  contour.push_back(p4);
  const cv::Point *pts = (const cv::Point*) Mat(contour).data;
  int npts = Mat(contour).rows;

  if ( argv == "2" ) {
    polylines(frame, &pts,&npts, 1,
      true,             // draw closed contour (i.e. joint end to start) 
      Scalar(0,255,0),  // colour RGB ordering (here = green) 
      3,                // line thickness
      CV_AA, 0);
  } else {
    // unsafe area
    rectangle(frame, unsafe_area, CV_RGB(255, 0, 0), 3);
  }

  putText(frame, box_text, Point(unsafe_area.x, unsafe_area.y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255, 0, 0), 2.0);
 
  if (faces.size() > 1 || faces.size() == 0)
    isNotWarning = 1;
  else
    isNotWarning = 0;
  windows_text = format("Faces = %d  %s", faces.size(), (isNotWarning) ? "Safe" : "unSafe" );

  for ( size_t i = 0; i < faces.size(); i++ )
  {
    int X_axis = 0;
    int Y_axis = 0;
    X_axis = faces[i].x + faces[i].width/2;
    Y_axis = faces[i].y + faces[i].height/2;

    // Test the parallelogram
    float b = 0.00, k = 0.00;
    b = ( p1.y * p4.x - p4.y * p1.x ) / ( p4.x - p1.x );
    k = ( p1.y - p4.y ) / ( p1.x - p4.x );
    float A1 = k, B1 = -1, C1 = b;
    b = ( p3.y * p2.x - p2.y * p3.x ) / ( p2.x - p3.x );
    k = ( p3.y - p2.y ) / ( p3.x - p2.x );
    float A2 = k, B2 = -1, C2 = b;

    Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );

    if ( argv == "2" ) {
      if (( A1 * X_axis + B1 * Y_axis + C1 ) * ( A2 * X_axis + B2 * Y_axis + C2 ) < 0 \
        && Y_axis > p1.y && Y_axis < p4.y ) {
        printf("It is inside the parallelogram! \a\n");
        isSafe = 0;
      } else 
        isSafe = 1;
    }
    else {    
      if( X_axis > unsafe_area.x && X_axis < unsafe_area.x + unsafe_area.width \
        && Y_axis > unsafe_area.y && Y_axis < unsafe_area.y + unsafe_area.height)
      {  
        printf("warnnning \a\n");
        isSafe = 0;
      }
      else
        isSafe = 1;
    }
    putText(frame, windows_text, Point(20, 600), FONT_HERSHEY_PLAIN, 2.0, (isSafe) ? CV_RGB(0, 255, 0) : CV_RGB(255, 0, 0), 2.0);
    circle( frame, center, 3, Scalar(0, 255, 0), -1, 8, 0 );
    // Scalar(0, 255, 255) is yellow; Scalar( 0, 0, 255 )) is red
    ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, \
      (isSafe) ? ((isNotWarning) ? Scalar( 0, 255, 0 ) : Scalar( 0, 255, 255 )) : Scalar( 0, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;

    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

    for ( size_t j = 0; j < eyes.size(); j++ )
    {
      Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
      int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
      // circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 ); // circle the eyes
    }
  }
  //-- Show what you got
  imshow( window_name, frame );
}
