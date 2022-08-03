#include"detector_num.h"

Detector_num::Detector_num(const cv::Mat &image){
    this->_image=image.clone();
    fmt::print("\n[{}] Detector_num was created successfully !",green_flag);
	detect_response.push_back(0x02);
}

cv::Mat Detector_num::roi_img_finder(){ 
    cv::Mat gray;
	inRange(_image,Scalar(0,0,0),Scalar(179,255,46),gray);
    imshow("原图",gray);
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	dilate(gray,gray,element);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(gray, contours,hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	fmt::print("\nsize :{}",contours.size());
	if(contours.size()>0&&contours.size()<100){
	for(size_t i=0;i<contours.size();i++){
		fmt::print("\n第{}个轮廓的面积为：{}",i,contourArea(contours[i]));
		if(contourArea(contours[i])>2000&&contourArea(contours[i])<10000){
			RotatedRect Rota_=minAreaRect(contours[i]);
			_roi_images_rect.push_back(Rota_);
			}
		}
		vector<vector<Point>>().swap(contours);
		vector<Vec4i> ().swap(hierarchy);
		sort_roi_image();
	}
	return gray;
}

bool Detector_num::sort_roi_image(){
    sort(_roi_images_rect.begin(),
	     _roi_images_rect.end(),
	     cmp);
	for(size_t i=0;i<_roi_images_rect.size();i++){
			RotatedRect Rota_=_roi_images_rect[i];
			//_roi_images_rect.push_back(Rota_);
			Rect tempRect=Rota_.boundingRect();
			Point2f vertice[4];
			Rota_.points(vertice);
			for (int i = 0; i < 4; i++){
			line(_image, vertice[i] , vertice[(i + 1) % 4] , Scalar(0, 255, 0), 2);
				}
			cv::Mat img_adjust = RotatedRect_adjust(Rota_);
			cv::Mat img_adjust_clone =img_adjust.clone();
			cv::cvtColor(img_adjust,img_adjust,CV_RGB2GRAY);
			//imwrite("/home/chen/Test/write/save.jpg", img_adjust_clone);
			threshold(img_adjust, img_adjust, 100, 255, cv::THRESH_BINARY_INV);
			//imshow("adjust",img_adjust);
			vector<int>response;
			template_matching(img_adjust,response);
			if(response[1]>150000&&response[1]<170000){
				fmt::print("\nresponse :{}",response[0]);
				fmt::print("\nrate :{}",response[1]);
				detect_response.push_back(response[0]);
				_roi_images_sort.push_back(img_adjust_clone);
				}
	}
    return true;
}

int Detector_num::response_num(vector<unsigned char> &send_number_detect_response){
	for(size_t i=0;i<_roi_images_sort.size();i++){
			int response=svmpredict(_roi_images_sort[i]);
    
			fmt::print("\n[{}]:RESPONSE IS NUMBER :{}",green_flag,response);
			send_number_detect_response.push_back(response);
	}
	return 1;
}

cv::Mat Detector_num::RotatedRect_adjust(cv::RotatedRect &rect ){
		cv::Mat src=_image.clone();
		cv::Mat M, rotated;
		cv::Mat cropped;
		// get angle and size from the bounding box
		double angle = static_cast<double>(rect.angle);
		cv::Size rect_size = rect.size;
		if (angle < -45.0) 
		{
			angle += 90.0;
			cv::swap(rect_size.width, rect_size.height);
		}
		if (angle > 45.0) 
		{
			angle -= 90.0;
			cv::swap(rect_size.width, rect_size.height);
		}
		// get the rotation matrix
		M = getRotationMatrix2D(rect.center, angle, 1.0);
		// perform the affine transformation
		warpAffine(src, rotated, M, src.size(), cv::INTER_CUBIC);
		// crop the resulting image
		getRectSubPix(rotated, rect_size, rect.center, cropped);
		return cropped;

}

int Detector_num::getPixelSum(Mat &image){
	int a = 0;
	for (int row = 0; row < image.rows; row++) {
		uchar* current_pixel = image.ptr<uchar>(row);
		for (int col = 0; col < image.cols; col++) {
			a += *current_pixel++;	//指针遍历像素点反转颜色
		}
	}
	return a;

}

void Detector_num::template_matching(cv::Mat &matching_img,vector<int> &response){
	Mat imgSub;
	double min = 10e5;
	int num = -1;
	int rate = -1;
	//cout<<"111"<<endl;
	for (int i = 0; i < 11; i++) {
		Mat templatimg = imread("/home/chen/Test/image_num/" + std::to_string(i) + ".jpg");
		cv::cvtColor(templatimg,templatimg,CV_RGB2GRAY);
		//imshow("10",templatimg);
		//waitKey(0);
		resize(matching_img, matching_img, Size(32, 48), 0, 0, cv::INTER_LINEAR);	//将两图像大小调至相同
		resize(templatimg, templatimg, Size(32, 48), 0, 0, cv::INTER_LINEAR);
		absdiff(templatimg, matching_img, imgSub);
		rate = getPixelSum(imgSub);
		if (rate < min) {
			min = rate;
			num = i;
		}
	}
	if(num==10){
		num=4;
	}
	response.push_back(num);
	response.push_back(rate);
	fmt::print("\nfinish!\n");

}

int Detector_num::svmpredict(Mat &image){
	Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>("/home/chen/Raspberry_Pi/detector_numbers/svm.xml");
	HOGDescriptor hog(Size(128, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
			vector<float> descriptors;
			cv::Mat inMat;
			inMat = image;
			//fmt::print("\n{}  ,   {}",image.cols,image.rows);
			if ((inMat.cols!=128)||(inMat.cols!=128))
			{
				resize(inMat, inMat, Size(128,128));
				//fmt::print("\n{}  ,   {}",inMat.cols,inMat.rows);
			}
			hog.compute(inMat, descriptors);
			cv::Mat temp;
			temp.push_back(descriptors);
			temp = temp.reshape(1, 1);
			temp.convertTo(temp, CV_32FC1);
			int response = (int)svm->predict(temp);
			fmt::print("\n[{}] svm result: {}",green_flag,response);
			temp.release();
			inMat.release();
			return response;

}

Detector_num::~Detector_num(){
	cout<<"\nend!!!!\n"<<endl;
}
