#include<iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "string"
#include "QRcode.h"
using namespace cv;
using namespace std;
 string QRcode_run(Mat image){	
        Mat img = image.clone();//imread("E:\\cars\\QRcode4.jpg");
        //cout << img.rows << img.cols << endl;
        resize(img, img, Size(640, 640 * img.rows / img.cols));
        //cout << img.rows << img.cols << endl;
        Mat gray;
        cvtColor(img, gray, COLOR_RGB2GRAY);
        threshold(gray, gray, 100, 255, THRESH_BINARY);
        //imshow("threshold", gray);
        Mat SE = getStructuringElement(MORPH_RECT, Size(3, 3));
        erode(gray, gray, SE);
        //imshow("erode", gray);
        cv::QRCodeDetector QRdetecter;
        std::vector<cv::Point> list;
        cv::Mat  res;
        string str = QRdetecter.detectAndDecode(gray, list, res);
        cout<<"information: "<<str<<endl;
        //printf("1111\n");
        for (int j = 0; j < list.size(); j++)
        {
            if (j == 3)
                line(img, list[j], list[0], Scalar(0, 255, 0), 2);
            else
                line(img, list[j], list[j + 1], Scalar(0, 255, 0), 2);
        }
        //resize(img, img, Size(640, 640 * img.rows / img.cols));
        imshow("RESULT", img);
        //waitKey(10);
        //system("pause");
    return str;
}
	
