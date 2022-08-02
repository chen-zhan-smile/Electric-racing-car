#include"tracking_module.h"

Tracking_Module::Tracking_Module(const cv::Mat image){
    this->_image=image.clone();
    fmt::print("\n[{}] was created successfully !",green_flag);
}

void Tracking_Module::white_line_tracing(vector<unsigned char> &tracking_error){
    cv::Mat HLS_img=toHSLfindcontours(_image);
    float Error=0;
    Image_Processing(HLS_img,Error);
    if (Error>0)
    {
        //往右转
        tracking_error.push_back(0);
        tracking_error.push_back(abs(Error));
    }
    else if(Error<0)
    {
        //往左转
        tracking_error.push_back(1);
        tracking_error.push_back(abs(Error));
    }
    
}

void Tracking_Module::black_red_lines_tracing(vector<unsigned char> &tracking_error,int &flag){
    cv::Mat HSV_img=toHSVfindcontours(_image,flag);
    float Error=0;
    Image_Processing(HSV_img,Error);
    if (Error>0)
    {
        //往右转
        tracking_error.push_back(0);
        tracking_error.push_back(abs(Error));
    }
    else if(Error<0)
    {
        //往左转
        tracking_error.push_back(1);
        tracking_error.push_back(abs(Error));
    }
}


cv::Mat Tracking_Module::toHSLfindcontours(cv::Mat &image){
    cv::Mat HSL_img;
    cvtColor(image,HSL_img,COLOR_BGR2HLS);
    cv::Mat HSL_image=HSL_img.clone();
    vector<Mat> channels;
    cv::split(HSL_image,channels);
    Mat H, L, S;
    H = channels.at(0);
    L = channels.at(1);
    S = channels.at(2);
    inRange(H,Scalar(0),Scalar(170),H);
    inRange(L,Scalar(0),Scalar(255),L);
    inRange(S,Scalar(0),Scalar(51),S);
    Mat dstImage;
    cv::bitwise_and(H,L,dstImage);
    cv::bitwise_and(S,dstImage,dstImage);
    dstImage=~dstImage;
    //寻出白线
    imshow("HLS_原图",dstImage);
    inRange(HSL_img,Scalar(0,0,0),Scalar(170,255,51),HSL_img);
    imshow("HLS",HSL_img);
    return dstImage;
}

cv::Mat Tracking_Module::toHSVfindcontours(cv::Mat &image,int &flag){
    cv::Mat HSV_img;
    cvtColor(image,HSV_img,COLOR_BGR2HSV);

    //寻出黑线 or 红线  --黑线为0 红线为1
    if (flag==0)
    {
        inRange(HSV_img,Scalar(0,0,0),Scalar(179,255,46),HSV_img);
        imshow("HSV",HSV_img);
        return HSV_img;
    }
    else if (flag==1)
    {
        inRange(HSV_img,Scalar(156,43,42),Scalar(179,255,255),HSV_img);
        imshow("HSV",HSV_img);
        return HSV_img;
    }
    else{
        fmt::print("\n[{}] The parameter input is incorrect, the flag is 0 or 1 !",red_flag);
    }
    return HSV_img;
}

void Tracking_Module::Image_Processing(cv::Mat &process_image,float &Error){
    //进行高斯模糊，进行去噪声
    cv::GaussianBlur(process_image,process_image,Size(5,5),0);
    cv::Mat kernel=getStructuringElement(MORPH_RECT,Size(5,5));
    //开运算去处白噪点
    morphologyEx(process_image,process_image,MORPH_OPEN,kernel);
    kernel.release();
    vector<vector<cv::Point>> contours;
    vector<Vec4i> hierarchy;
    //找到最大的轮廓，该轮廓为所识别的白线
    findContours(process_image, contours,hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //储存最大轮廓的质心
    Point2f coutour_m_center;
    if (contours.size()>1)
    {
        float max_area=0;
        int index;
        for (size_t i = 0; i < contours.size(); i++)
        {
            if (contourArea(contours[i])>max_area)
            {
                max_area=contourArea(contours[i]);
                index=i;
            } 
        }
        cv::Moments M = moments(contours[index]);
        coutour_m_center.x=M.m10 / M.m00;   // mu[i].m01 / mu[i].m00
        coutour_m_center.y=M.m01 / M.m00;
        cv::line(_image,Point2f(coutour_m_center.x,0),Point2f(coutour_m_center.x,_image.rows),Scalar(0,0,255),1);
        cv::line(_image,Point2f(0,coutour_m_center.y),Point2f(_image.cols,coutour_m_center.y),Scalar(0,0,255),1);
        imshow("image",_image);
        Error =coutour_m_center.x-(_image.cols/2);
    }
    else if(contours.size()==1)
    {
        cv::Moments M = moments(contours[0]);
        coutour_m_center.x=M.m10 / M.m00;   // mu[i].m01 / mu[i].m00
        coutour_m_center.y=M.m01 / M.m00;
        cv::line(_image,Point2f(coutour_m_center.x,0),Point2f(coutour_m_center.x,_image.rows),Scalar(0,0,255),1);
        cv::line(_image,Point2f(0,coutour_m_center.y),Point2f(_image.cols,coutour_m_center.y),Scalar(0,0,255),1);
        imshow("image",_image);
        Error =coutour_m_center.x-(_image.cols/2);
    }
    vector<vector<cv::Point>> ().swap(contours);
    vector<Vec4i>().swap(hierarchy);

}



Tracking_Module::~Tracking_Module(){
    _image.release();
    //cv::destroyAllWindows();
    fmt::print("\n[{}] Tracking End !",green_flag);
}