#ifndef JIANCE_H
#define JIANCE_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat detectAndDisplay( Mat frame );
int jiance( void );
int add_jiance( void );
#endif // JIANCE_H
