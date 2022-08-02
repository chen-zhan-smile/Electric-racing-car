/*
 * @Description: 
 * @Version: 1.0
 * @Autor: chen zhan
 * @Date: 2022-07-19 16:50:31
 * @LastEditors: chen zhan
 * @LastEditTime: 2022-07-20 22:35:22
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "QRcode_detector/QRcode.h"
#include "serial_port/serial.h"
#include "detector_numbers/detector_num.h"
#include "tracking_module/tracking_module.h"
#include  <thread>
#include  <mutex>

mutex m;//实例化m对象，不要理解为定义变量

void thread_tracking(cv::Mat &frame){
    Mat src=frame.clone();
    Tracking_Module tracking(src);
    vector<uchar> tracking_error;
    tracking_error.push_back(0x01);
    int flag=1;
    tracking.black_red_lines_tracing(tracking_error,flag);
    if (tracking_error.size()>1)
    {   
        {unique_lock<mutex> g1(m,try_to_lock);
        //尝试加锁，但如果没有锁定成功，会立即返回，不会阻塞在那里；   
        serial_send(tracking_error);}
        // for (size_t i = 0; i < tracking_error.size(); i++)
        // {   
        //     fmt::print("\n [{}] tracking_error:{}",i,tracking_error[i]);
            
        // }
    }
    src.release();
}

void thread_detector_num(cv::Mat &frame){
    Mat src=frame.clone();
    Detector_num detector(src);
    Mat temp=detector.roi_img_finder();
    // fmt::print("{} \n",detector.detect_response.size());
    // for (size_t i = 0; i < detector.detect_response.size(); i++)
    // {
    //     fmt::print("\n send result : {} \n",detector.detect_response[i]);
    // }      
    if(detector.detect_response.size()>1){
        {unique_lock<mutex> g2(m,try_to_lock);
        serial_send(detector.detect_response);}
    } 
    src.release();
    
}



int main(int argc, const char** argv) {
   string car_video_openflag = "ON"; 
   int count=0;
   try
   {
    if (!car_video_openflag.empty())
	{
		VideoCapture capture;
		capture.open(0);
		Mat frame;
		while(capture.read(frame))//读取视频的一帧
		{
        thread proc1(thread_tracking, std::ref(frame));
        thread proc2(thread_detector_num,std::ref(frame));
        proc1.join();
        proc2.join();
        waitKey(1);
        }
    }
   }
   catch(const std::exception& e)
   {
    std::cerr << e.what() << '\n';
   }
   
    return 0;
}
