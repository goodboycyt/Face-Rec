#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat detectAndDisplay( Mat frame );

String face_cascade_name = "C:/Users/lenovo/Documents/untitled/haarcascade_frontalface_alt_tree.xml";

CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

/**
 * @function main
 */
int jiance( void )
{
  VideoCapture capture;
  Mat frame;


 face_cascade.load( face_cascade_name );

  capture.open( 0 );
  if( capture.isOpened() )
  {
    for(;;)
    {
      capture >> frame;
Mat output=detectAndDisplay( frame );
cv::Size tem;
tem.width = 112;
tem.height = 112;
if(output.cols>140 &&output.rows>140){
resize(output,output,tem);
bool flag = imwrite("D:\\saveImage.jpg",output);
if(flag == true){
capture.release();
return 0;
}
}

    int c = waitKey(10);
      if( (char)c == 'c' ) { break; }

    }
  }
  return 0;
}
int add_jiance( void )
{
  VideoCapture capture;
  Mat frame;


 face_cascade.load( face_cascade_name );

  capture.open( 0 );
  if( capture.isOpened() )
  {
    for(;;)
    {
      capture >> frame;
Mat output=detectAndDisplay( frame );
cv::Size tem;
tem.width = 112;
tem.height = 112;
if(output.cols>140 &&output.rows>140){
resize(output,output,tem);
bool flag = imwrite("D:\\saveImage1.jpg",output);
if(flag == true){
capture.release();
return 0;
}
}

    int c = waitKey(10);
      if( (char)c == 'c' ) { break; }

    }
  }
  return 0;
}



Mat detectAndDisplay( Mat frame )
{
   std::vector<Rect> faces;
   Mat frame_gray;
   Mat faceROI;

   cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
   equalizeHist( frame_gray, frame_gray );
   //-- Detect faces
   face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

   for( size_t i = 0; i < faces.size(); i++ )
    {
      Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
      ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );

      faceROI = frame_gray( faces[i] );


    }

   imshow( window_name, frame );
   return faceROI ;
}
