#include <math.h>

using namespace cv;

// 计算两点直接的距离
int dirTwPit(Point p0, Point p1);
// 三边求面积
int triArea(int a, int b, int c);
// 三点求面积
int triAreaPit(Point p0, Point p1, Point p2);
bool isSafeFunc(Mat frame, Point cen);

int dirTwPit(Point p0, Point p1)
{
  // sqrt((x0-x1)^2 + (y0-y1)^2)
  return sqrt(pow((p0.x - p1.x), 2) + pow((p0.y - p1.y), 2));
}

int triArea(int a, int b, int c)
{
  // 海伦公式
  // AB=a BC =b AC=c p=(a+b+c)/2
  // S=sqrt(p*(p-a)*(p-b)*(p-c))
  int p = ( a + b + c )/2;
  return sqrt(p * (p - a) * (p - b) * (p - c));
}

int triAreaPit(Point p0, Point p1, Point p2)
{
  return triArea(dirTwPit(p0,p1), dirTwPit(p1,p2), dirTwPit(p0,p2));
}