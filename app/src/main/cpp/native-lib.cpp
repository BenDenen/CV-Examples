#include <jni.h>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

#include <android/log.h>

#define  LOG_TAG    "JNI_OpenCV_Log"

#define  LOGD(__VA_ARGS__)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define CHECK_MAT(cond) if(!(cond)){ LOGD("FAILED: " #cond); return; }


extern "C"
JNIEXPORT jstring

JNICALL
Java_com_example_borisdenisenko_kotlin_1opencv_1counters_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//vector_Point
void Mat_to_vector_Point(Mat& mat, std::vector<Point>& v_point)
{
    v_point.clear();
    CHECK_MAT(mat.type()==CV_32SC2 && mat.cols==1);
    v_point = (vector<Point>) mat;
}

//vector_Mat
void Mat_to_vector_Mat(cv::Mat& mat, std::vector<cv::Mat>& v_mat)
{
    v_mat.clear();
    if(mat.type() == CV_32SC2 && mat.cols == 1)
    {
        v_mat.reserve(mat.rows);
        for(int i=0; i<mat.rows; i++)
        {
            Vec<int, 2> a = mat.at< Vec<int, 2> >(i, 0);
            long long addr = (((long long)a[0])<<32) | (a[1]&0xffffffff);
            Mat& m = *( (Mat*) addr );
            v_mat.push_back(m);
        }
    } else {
        LOGD("Mat_to_vector_Mat() FAILED: mat.type() == CV_32SC2 && mat.cols == 1");
    }
}


void Mat_to_vector_vector_Point(Mat& mat, std::vector< std::vector< Point > >& vv_pt)
{
    std::vector<Mat> vm;
    vm.reserve( mat.rows );
    Mat_to_vector_Mat(mat, vm);
    for(size_t i=0; i<vm.size(); i++)
    {
        std::vector<Point> vpt;
        Mat_to_vector_Point(vm[i], vpt);
        vv_pt.push_back(vpt);
    }
}

void vector_Point_to_Mat(std::vector<Point>& v_point, Mat& mat)
{
    mat = Mat(v_point, true);
}

void vector_Mat_to_Mat(std::vector<cv::Mat>& v_mat, cv::Mat& mat)
{
    int count = (int)v_mat.size();
    mat.create(count, 1, CV_32SC2);
    for(int i=0; i<count; i++)
    {
        long long addr = (long long) new Mat(v_mat[i]);
        mat.at< Vec<int, 2> >(i, 0) = Vec<int, 2>(addr>>32, addr&0xffffffff);
    }
}

void vector_vector_Point_to_Mat(std::vector< std::vector< Point > >& vv_pt, Mat& mat)
{
    std::vector<Mat> vm;
    vm.reserve( vv_pt.size() );
    for(size_t i=0; i<vv_pt.size(); i++)
    {
        Mat m;
        vector_Point_to_Mat(vv_pt[i], m);
        vm.push_back(m);
    }
    vector_Mat_to_Mat(vm, mat);
}


// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
JNIEXPORT void JNICALL
Java_com_example_borisdenisenko_kotlin_1opencv_1counters_MainActivity_FindBestSquare(JNIEnv *, jobject, jlong addrImage, jlong addrSquare,
                                                                               jint newHeight, jint newWidth)
{
    // Input Image
    Mat &image = *(Mat *) addrImage;

    // Mat to return data
    Mat &squareMat = *(Mat *) addrSquare;
    vector<Point> square;

    int height = (int) newHeight;
    int width = (int) newWidth;

    Size size = Size(width, height);
    Mat resizedImage(size, CV_8UC4), pyr, timg, grayImage(size, CV_8UC4),cannedImage(size, CV_8UC1);

    // Resize image to speed up method
    resize(image, resizedImage, size);

    // Set gray color
    cvtColor(resizedImage, grayImage, COLOR_RGBA2GRAY);
    // down-scale and upscale the image to filter out the noise
    pyrDown(grayImage, pyr, Size(grayImage.cols/2, grayImage.rows/2));
    pyrUp(pyr, timg, grayImage.size());
    // apply Canny. Take the upper threshold from slider
    // and set the lower to 0 (which forces edges merging)
    Canny(timg, cannedImage, 0, 50);

    // dilate canny output to remove potential
    // holes between edge segments
    dilate(cannedImage, cannedImage, Mat());

    vector<vector<Point> > contours;
    // find contours and store them all as a list
    findContours(cannedImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "approxArea = %d", contours.size());

    vector<Point> approx;

    double maximumArea = size.area() * 0.72;
    double minimumArea = size.area()/30;

    double lastKnownArea = 0;

    // test each contour
    for( size_t i = 0; i < contours.size(); i++ )
    {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.08, true);

        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        double approxArea = fabs(contourArea(Mat(approx)));
        if (approx.size() == 4 &&
            approxArea < maximumArea &&
            approxArea > minimumArea &&
            approxArea >= lastKnownArea &&
            isContourConvex(Mat(approx)))
        {
            double maxCosine = 0;

            for( int j = 2; j < 5; j++ )
            {
                // find the maximum cosine of the angle between joint edges
                double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                maxCosine = MAX(maxCosine, cosine);
            }

            // if cosines of all angles are small
            // (all angles are ~90 degree) then write quandrange
            // vertices to resultant sequence
            if( maxCosine < 0.6 ) {
                lastKnownArea = approxArea;
                square = approx;
            }

        }
    }

    vector_Point_to_Mat(square, squareMat);

}
