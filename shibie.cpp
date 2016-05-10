#include <stdio.h>
#include <string.h>
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"
#include "qdebug.h"
#include <qmessagebox.h>

using namespace cv;

//globle variables
int nTrainFaces         = 0;    // 训练图片的数目
int nEigens             = 0;    // 特征值的数目
IplImage** faceImgArr       = 0;    // 人脸图像数组
CvMat* personNumTruthMat    = 0;    // 人数量数组
IplImage* pAvgTrainImg      = 0;    // 平均向量
IplImage** eigenVectArr     = 0;    // 特征向量
CvMat* eigenValMat      = 0;    // 特征值
CvMat* projectedTrainFaceMat    = 0;    //训练人脸映射

//// Function prototypes
void learn();
double recognize();
void doPCA();
void storeTrainingData();
int loadTrainingData(CvMat** pTrainPersonNumMat);
double findNearestNeighbor(float* projectedTestFace);
int loadFaceImgArray(char* filename);
void printUsage();

int shibie( int argc, char** argv )
{
    if((argc != 2) && (argc != 3)){
    printUsage();
    return -1;
    }

    if( !strcmp(argv[1], "train" )){
    learn();
    } else if( !strcmp(argv[1], "test") ){
    recognize();
    } else {
    printf("Unknown command: %s\n", argv[1]);
    }
    return 0;
}

void printUsage(){
    printf("Usage: eigenface <command>\n",
    "  Valid commands are\n"
    "    train\n"
    "    test\n"
    );
}

void learn(){
    int i;

    // load training data
    nTrainFaces = loadFaceImgArray("C:\\Users\\lenovo\\Documents\\untitled\\train.txt");
    if( nTrainFaces < 2){
    fprintf(
        stderr,
        "Need 2 or more training faces\n"
        "Input file contains only %d\n",
        nTrainFaces
    );
    return;
    }

    // do PCA on the training faces
    doPCA();

    // project the training images onto the PCA subspace
    projectedTrainFaceMat = cvCreateMat(nTrainFaces, nEigens, CV_32FC1);
    for(i = 0; i < nTrainFaces; i ++){
    cvEigenDecomposite(
        faceImgArr[i],
        nEigens,
        eigenVectArr,
        0, 0,
        pAvgTrainImg,
        projectedTrainFaceMat->data.fl + i*nEigens
    );
    }

    // store the recognition data as an xml file
    storeTrainingData();
}

int loadFaceImgArray(char* filename){   //加载训练图像
    FILE* imgListFile = 0;
    char imgFilename[512];
    int iFace, nFaces = 0;

    // 打开train.txt
    imgListFile = fopen(filename, "r");

    // 计算脸的数目
    while( fgets(imgFilename, 512, imgListFile) ) ++ nFaces;
    rewind(imgListFile);

    // allocate the face-image array and person number matrix
    faceImgArr = (IplImage **)cvAlloc( nFaces*sizeof(IplImage *) );
    personNumTruthMat = cvCreateMat( 1, nFaces, CV_32SC1 );

    // store the face images in an array
    for(iFace=0; iFace<nFaces; iFace++){
    //read person number and name of image file
    fscanf(imgListFile, "%d %s", personNumTruthMat->data.i+iFace, imgFilename);

    // load the face image
    faceImgArr[iFace] = cvLoadImage(imgFilename, CV_LOAD_IMAGE_GRAYSCALE);
    }

    fclose(imgListFile);

    return nFaces;
}

void doPCA(){    //对人脸训练集进行pca映射
    int i;
    CvTermCriteria calcLimit;
    CvSize faceImgSize;

    // set the number of eigenvalues to use
    nEigens = nTrainFaces - 1;

    // allocate the eigenvector images
    faceImgSize.width = faceImgArr[0]->width;
    faceImgSize.height = faceImgArr[0]->height;
    /**********************************************************************/
    eigenVectArr = (IplImage**)cvAlloc(sizeof(IplImage*) * nEigens);
    for(i=0; i<nEigens; i++){
    eigenVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);
    }
/**********************************************************************/
    // allocate the eigenvalue array
    eigenValMat = cvCreateMat( 1, nEigens, CV_32FC1 );

    // allocate the averaged image
    pAvgTrainImg = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

    // set the PCA termination criterion
    calcLimit = cvTermCriteria( CV_TERMCRIT_ITER, nEigens, 1);

    // compute average image, eigenvalues, and eigenvectors
    cvCalcEigenObjects(
    nTrainFaces,
    (void*)faceImgArr,
    (void*)eigenVectArr,
    CV_EIGOBJ_NO_CALLBACK,
    0,
    0,
    &calcLimit,
    pAvgTrainImg,
    eigenValMat->data.fl
    );
}

void storeTrainingData(){    //将训练好的数据保存为xml文件
    CvFileStorage* fileStorage;
    int i;

    // create a file-storage interface
    fileStorage = cvOpenFileStorage( "C:\\Users\\lenovo\\Documents\\untitled\\facedata.xml", 0, CV_STORAGE_WRITE);

    // store all the data
    cvWriteInt( fileStorage, "nEigens", nEigens);
    cvWriteInt( fileStorage, "nTrainFaces", nTrainFaces );
    cvWrite(fileStorage, "trainPersonNumMat", personNumTruthMat, cvAttrList(0, 0));
    cvWrite(fileStorage, "eigenValMat", eigenValMat, cvAttrList(0,0));
    cvWrite(fileStorage, "projectedTrainFaceMat", projectedTrainFaceMat, cvAttrList(0,0));
    cvWrite(fileStorage, "avgTrainImg", pAvgTrainImg, cvAttrList(0,0));

    for(i=0; i<nEigens; i++){
    char varname[200];
    sprintf( varname, "eigenVect_%d", i);
    cvWrite(fileStorage, varname, eigenVectArr[i], cvAttrList(0,0));
    }

    //release the file-storage interface
    cvReleaseFileStorage( &fileStorage );
}

double recognize(){     //对输入图像识别，输出为相似度
    int i, nTestFaces = 0;      // the number of test images
    CvMat* trainPersonNumMat = 0;   // the person numbers during training
    float* projectedTestFace = 0;

    // load test images and ground truth for person number
    nTestFaces = loadFaceImgArray("C:\\Users\\lenovo\\Documents\\untitled\\text.txt");
    printf("%d test faces loaded\n", nTestFaces);

    // load the saved training data
    if( !loadTrainingData( &trainPersonNumMat ) ) return 0;

    // project the test images onto the PCA subspace
    projectedTestFace = (float*)cvAlloc( nEigens*sizeof(float) );
    for(i=0; i<nTestFaces; i++){
    int iNearest, nearest, truth;

    // project the test image onto PCA subspace
    cvEigenDecomposite(
        faceImgArr[i],
        nEigens,
        eigenVectArr,
            0, 0,
        pAvgTrainImg,
        projectedTestFace
    );

//    iNearest = findNearestNeighbor(projectedTestFace);
    double leastDistSq2;
    leastDistSq2= findNearestNeighbor(projectedTestFace);
  return leastDistSq2;
//    truth = personNumTruthMat->data.i[i];
//    nearest = trainPersonNumMat->data.i[iNearest];

//    if(nearest==3){
//       qDebug()<< "this is yushuai" <<endl;
//    }

//    printf("nearest = %d, Truth = %d\n", nearest, truth);
    }

}

int loadTrainingData(CvMat** pTrainPersonNumMat){ //加载训练好数据文件
    CvFileStorage* fileStorage;
    int i;

    // create a file-storage interface
    fileStorage = cvOpenFileStorage( "C:\\Users\\lenovo\\Documents\\untitled\\facedata.xml", 0, CV_STORAGE_READ );
    if( !fileStorage ){
    fprintf(stderr, "Can't open facedata.xml\n");
    return 0;
    }

    nEigens = cvReadIntByName(fileStorage, 0, "nEigens", 0);
    nTrainFaces = cvReadIntByName(fileStorage, 0, "nTrainFaces", 0);
    *pTrainPersonNumMat = (CvMat*)cvReadByName(fileStorage, 0, "trainPersonNumMat", 0);
    eigenValMat = (CvMat*)cvReadByName(fileStorage, 0, "eigenValMat", 0);
    projectedTrainFaceMat = (CvMat*)cvReadByName(fileStorage, 0, "projectedTrainFaceMat", 0);
    pAvgTrainImg = (IplImage*)cvReadByName(fileStorage, 0, "avgTrainImg", 0);
    eigenVectArr = (IplImage**)cvAlloc(nTrainFaces*sizeof(IplImage*));
    for(i=0; i<nEigens; i++){
    char varname[200];
    sprintf( varname, "eigenVect_%d", i );
    eigenVectArr[i] = (IplImage*)cvReadByName(fileStorage, 0, varname, 0);
    }

    // release the file-storage interface
    cvReleaseFileStorage( &fileStorage );

    return 1;
}

double findNearestNeighbor(float* projectedTestFace){ //计算待测图像与训练集中距离最小的图像
    double leastDistSq = DBL_MAX;

    int i, iTrain, iNearest = 0;

    for(iTrain=0; iTrain<nTrainFaces; iTrain++){
    double distSq = 0;

    for(i=0; i<nEigens; i++){
        float d_i = projectedTestFace[i] -
        projectedTrainFaceMat->data.fl[iTrain*nEigens + i];
        distSq += d_i*d_i;
    }


    if(distSq < leastDistSq){
        leastDistSq = distSq;
        iNearest = iTrain;

    }
    qDebug()<< distSq <<endl;
    }

//    return iNearest;
    return leastDistSq;
  }
