#ifndef _Tracking_Module_
#define _Tracking_Module_
#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <fmt/core.h>
#include <fmt/color.h>
#include <string>


using namespace std;
using namespace cv;


class Tracking_Module
{
private:
    cv::Mat _image;
    string green_flag=fmt::format(fg(fmt::color::green)| fmt::emphasis::bold ,"Tracking_Module");
    string red_flag=fmt::format(fg(fmt::color::red)| fmt::emphasis::bold ,"Tracking_Module");
    /* data */
    cv::Mat toHSLfindcontours(cv::Mat &image);
    cv::Mat toHSVfindcontours(cv::Mat &image,int &flag);
    void Image_Processing(cv::Mat &process_image,float &Error);

public:

    Tracking_Module(const cv::Mat image);

    void white_line_tracing(vector<unsigned char> &tracking_error);

    void black_red_lines_tracing(vector<unsigned char> &tracking_error,int &flag);

    ~Tracking_Module();
};



#endif // !_Tracking_Module_