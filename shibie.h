#ifndef SHIBIE_H
#define SHIBIE_H
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"
#include "qdebug.h"

using namespace cv;

int shibie( int argc, char** argv );
void learn();
double recognize();
void doPCA();
void storeTrainingData();
int loadTrainingData(CvMat** pTrainPersonNumMat);
double findNearestNeighbor(float* projectedTestFace);
int loadFaceImgArray(char* filename);
void printUsage();

#endif // SHIBIE_H
