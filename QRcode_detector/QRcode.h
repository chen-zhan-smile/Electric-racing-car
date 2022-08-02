#include "string"
#ifndef QRcode
#define QRcode
    #include "opencv2/opencv.hpp"
    #include "opencv2/core/core.hpp"
    #include "opencv2/imgproc/imgproc.hpp"
    #include "opencv2/highgui/highgui.hpp"
    using namespace cv;
    using namespace std;
    extern string QRcode_run(Mat image);
#endif // !QRcode