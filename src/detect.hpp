#include "displayImage.hpp"
#include "triarea.cpp"
#include "onMouse.cpp"

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame, String argv )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  bool isNotWarning = 0;
  bool isSafe = 0;
  int X_axis = 0;
  int Y_axis = 0;

  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

  if ( argv == "2" ) {
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

    X_axis = faces[i].x + faces[i].width/2;
    Y_axis = faces[i].y + faces[i].height/2;

    if ( argv == "2" ) {
      // isSafe = isSafeInQuadrangle( frame, X_axis, Y_axis );
      isSafe = isSafeFunc( frame, Point(X_axis, Y_axis) );
    } else {
      if( X_axis > unsafe_area.x && X_axis < unsafe_area.x + unsafe_area.width \
        && Y_axis > unsafe_area.y && Y_axis < unsafe_area.y + unsafe_area.height)
      {  
        printf("Warnnning \a\n");
        isSafe = 0;
      }
      else
        isSafe = 1;
    }

    // The midpoint of circle
    Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );

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

bool isSafeFunc(Mat frame, Point cen)
{
  // p[0] = Point(100,100);
  // p[1] = Point(300,100);
  // p[2] = Point(300,300);
  // p[3] = Point(100,300);

  int quadArea = 0;
  int cenQuaArea = 0;
  quadArea = triAreaPit(p[0], p[1], p[2]) + triAreaPit(p[0], p[3], p[2]);

  for (int i = 0; i < 3; ++i)
  {
    cenQuaArea = cenQuaArea + triAreaPit(cen, p[i], p[i+1]);
  }
  cenQuaArea = cenQuaArea + triAreaPit(cen, p[0], p[3]);

  vector<Point> contour;
  contour.push_back(p[0]);  // X-axis, Y-axis
  contour.push_back(p[1]);
  contour.push_back(p[2]);
  contour.push_back(p[3]);
  const cv::Point *pts = (const cv::Point*) Mat(contour).data;
  int npts = Mat(contour).rows;

  circle( frame, cen, 3, Scalar(0, 255, 0), -1, 8, 0 );
  // todo: 初始化为0时, 不显示
  if ( p[3].x != 0) {
  polylines(frame, &pts,&npts, 1,
    true,             // draw closed contour (i.e. joint end to start) 
    Scalar(0,255,0),  // colour RGB ordering (here = green) 
    3,                // line thickness
    CV_AA, 0);
  }

  // printf("%d %d %d %d\n", p[0].x, p[1].x, p[2].x, p[3].x);
  // printf("%d %d %d %d\n", p[0].y, p[1].y, p[2].y, p[3].y);

  printf("%d : %d\n", quadArea, cenQuaArea);
  if (quadArea > cenQuaArea) {
    return 0;
  } else {
    return 1;
  }
}

bool isSafeInQuadrangle( Mat frame, int X_axis, int Y_axis )
{
  // clockwise
  // Point p1 = Point(400,100);
  // Point p2 = Point(700,100);
  // Point p3 = Point(600,400);
  // Point p4 = Point(300,400);

  p[1] = Point(400,100);
  p[2] = Point(700,100);
  p[3] = Point(600,400);
  p[0] = Point(300,400);

  vector<Point> contour;
  contour.push_back(p[0]);  // X-axis, Y-axis
  contour.push_back(p[1]);
  contour.push_back(p[2]);
  contour.push_back(p[3]);
  printf("p[0] %d %d\n", p[0].x, p[0].y);
  printf("p[1] %d %d\n", p[1].x, p[1].y);
  printf("p[2] %d %d\n", p[2].x, p[2].y);
  printf("p[3] %d %d\n", p[3].x, p[3].y);

  const cv::Point *pts = (const cv::Point*) Mat(contour).data;
  int npts = Mat(contour).rows;

  // todo: 初始化为0时, 不显示
  polylines(frame, &pts,&npts, 1,
    true,             // draw closed contour (i.e. joint end to start) 
    Scalar(0,255,0),  // colour RGB ordering (here = green) 
    3,                // line thickness
    CV_AA, 0);

  float b = 0.00, k = 0.00;
  float A1 = 0.00, B1 = 0.00, C1 = 0.00;
  float A2 = 0.00, B2 = 0.00, C2 = 0.00;
  b = ( p1.y * p4.x - p4.y * p1.x ) / ( p4.x - p1.x );
  k = ( p1.y - p4.y ) / ( p1.x - p4.x );
  A1 = k, B1 = -1, C1 = b;
  b = ( p3.y * p2.x - p2.y * p3.x ) / ( p2.x - p3.x );
  k = ( p3.y - p2.y ) / ( p3.x - p2.x );
  A2 = k, B2 = -1, C2 = b;

  if (( A1 * X_axis + B1 * Y_axis + C1 ) * ( A2 * X_axis + B2 * Y_axis + C2 ) < 0 \
    && Y_axis > p1.y && Y_axis < p4.y ) {
    printf("It is inside the parallelogram! \a\n");
    return 0;
  } else 
    return 1;
}
