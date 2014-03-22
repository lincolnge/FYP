#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/legacy/blobtrack.hpp" // for CV_AA & CV_RGB
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

String windows_text = "";
Rect unsafe_area = Rect(400, 400, 200, 200);
String box_text = format("UNSAFE AREA");

Point p1 = Point(0,0);
Point p2 = Point(0,0);
Point p3 = Point(0,0);
Point p4 = Point(0,0);

// 定义四个点
Point p[4];

Point2f point;

/** Function Headers */
void detectAndDisplay( Mat frame, String argv );
bool isSafeInQuadrangle( Mat frame, int X_axis, int Y_axis );
