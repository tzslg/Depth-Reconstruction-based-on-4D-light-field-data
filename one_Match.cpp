#include"pch.h"
//#include"areaMatch.h"
//#include"ioImage.h"
//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <cmath>
//#include <vector>
//#include <opencv2/core/core.hpp>
//#include <exception>
//#include <thread>
//
//using namespace std;
//using namespace cv;
//Mat areaMatch::H1()
//{
//	Mat H;
//	H = (Mat_<double>(3, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1);
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++)
//			H.at<double>(i, j) /= 16;
//	}
//	return H;
//}
//double areaMatch::ResampleBilinear(double x, double y, double * scrImg, int y_row, int x_col)
//{
//	int x_1 = int(x);
//	int x_2 = int(x + 1);
//
//	int y_1 = int(y);
//	int y_2 = int(y + 1);
//
//	if (x<0 || y<0 || (y + 1) >(y_row - 1) || (x + 1) >(x_col - 1))
//		return 0;
//	//else if ( y_1 == y_2)
//	//{
//	//	return ((x_2 - x)*scrImg[y_1*y_row + x_1]
//	//		+ (x - x_1)*scrImg[y_1*y_row + x_2]) / (x_2 - x_1);
//	//}
//	//else if (x_1 == x_2)
//	//{
//	//	return ((y_2 - y)*scrImg[y_1*y_row + x_1]
//	//		+(y - y_1)*scrImg[y_2*y_row + x_1]) / (y_2 - y_1);
//	//}
//	else
//	{//x_2-x_1=1;y_2-y_1=1;
//		double r1 = ((x_2 - x)*scrImg[y_1*x_col + x_1]
//			+ (x - x_1)*scrImg[y_1*x_col + x_2]);
//
//		double r2 = ((x_2 - x)*scrImg[y_2*x_col + x_1]
//			+ (x - x_1)*scrImg[y_2*x_col + x_2]);
//
//		return ((y_2 - y)*r1 + (y - y_1)*r2);
//	}
//}
//void areaMatch::s_remap(Mat scrImg, Mat dstImg, double delta_y, double delta_x) {
//
//	Mat double_scrImg;
//	scrImg.convertTo(double_scrImg, CV_64FC1);
//	int stride = int(double_scrImg.step.p[0]);
//	double *data = new double[double_scrImg.cols* double_scrImg.rows];
//	for (int i = 0; i < double_scrImg.rows; i++) {
//		char* p = ((char*)data) + i * double_scrImg.cols * sizeof(double);
//		memcpy(p, double_scrImg.data + stride * i, double_scrImg.cols * sizeof(double));
//	}
//	/*interpolate, Bilinear*/
//	for (int r = 0; r < dstImg.rows; r++) {
//		for (int c = 0; c < dstImg.cols; c++) {
//			double y = r - delta_y;
//			double x = c + delta_x;
//			dstImg.at<double>(r, c) = ResampleBilinear(x, y, data, double_scrImg.rows, double_scrImg.cols);
//		}
//	}
//	delete data;
//	return;
//}
//double areaMatch::fdist(double* data_H, double* data_image_y_1, double* data_image_y, double* data_image_y1, uchar* data_I55_y_1, uchar* data_I55_y, uchar* data_I55_y1, int x)
//{
//	return sqrt(data_H[0] * pow((data_image_y_1[x - 1] - data_I55_y_1[x - 1]), 2)
//		+ data_H[1] * pow((data_image_y_1[x] - data_I55_y_1[x]), 2)
//		+ data_H[2] * pow((data_image_y_1[x + 1] - data_I55_y_1[x + 1]), 2)
//		+ data_H[3] * pow((data_image_y[x - 1] - data_I55_y[x - 1]), 2)
//		+ data_H[4] *
//		pow((data_image_y[x] - data_I55_y[x]), 2)
//		+ data_H[5] * pow((data_image_y[x + 1] - data_I55_y[x + 1]), 2)
//		+ data_H[6] * pow((data_image_y1[x - 1] - data_I55_y1[x - 1]), 2)
//		+ data_H[7] * pow((data_image_y1[x] - data_I55_y1[x]), 2)
//		+ data_H[8] * pow((data_image_y1[x + 1] - data_I55_y1[x + 1]), 2));
//}
//void areaMatch::s_f_match(double s, int k, Mat dstImg, Mat s_Errors,Mat k_Errors ,Mat k_Disparity)
//{
//	double data_Xout = 255;
//	for (int y = 1; y < Y_image.rows - 1; y++)
//	{
//	
//		double* data_H = H1().ptr<double>();
//		//
//		double* data_image_y_1 = dstImg.ptr<double>(y - 1);
//		double* data_image_y = dstImg.ptr<double>(y);
//		double* data_image_y1 = dstImg.ptr<double>(y + 1);
//		//
//		uchar* data_I55_y_1 = Y_image.ptr<uchar>(y - 1);
//		uchar* data_I55_y = Y_image.ptr<uchar>(y);
//		uchar* data_I55_y1 = Y_image.ptr<uchar>(y + 1);
//		//
//		double *data_Disparity = Disparity.ptr<double>(y);
//		double *data_Error = Error.ptr<double>(y);
//		double *data_s_Errors= s_Errors.ptr<double>(y);
//
//		//
//		double *data_k_Errors= k_Errors.ptr<double>(y);
//		double *data_k_Disparity = k_Disparity.ptr<double>(y);
//
//
//		for (int x = 1; x < Y_image.cols - 1; x++)
//		{
//			data_Xout = fdist(data_H, data_image_y_1, data_image_y, data_image_y1, data_I55_y_1, data_I55_y, data_I55_y1, x);
//			//初始化
//			if (s==s_min && k==1)
//			{
//				data_Disparity[x] = s;
//				data_Error[x] = data_Xout;
//			}
//			//求相邻视点的视差以及最小距离测度函数值
//			else if (data_Error[x]>data_Xout+0.01)
//			{
//				data_Disparity[x] = s;
//				data_Error[x] = data_Xout;
//			}
//
//
//            //搜索每个视差的最小匹配值
//			if (k==1)
//			{
//				data_s_Errors[x]=data_Xout;
//
//			}
//			else if (data_s_Errors[x]>data_Xout)
//			{
//				data_s_Errors[x]=data_Xout;
//			}
//			/////////////////////////////////////////
//			
//			if (s == s_min)
//			{
//				data_k_Disparity[x] =s;
//				data_k_Errors[x] = data_Xout;
//			}
//			else if(data_k_Errors[x]>data_Xout)
//			{
//			data_k_Errors[x]=data_Xout;
//			data_k_Disparity[x]=s;
//			}
//
//		}
//	}
//}
//
//void areaMatch::Disparity_compute(vector<Mat> s_Errors,vector<Mat> k_Errors,vector<Mat> k_Disparity)
//{
//	double d = 0.01,delta_y,delta_x; int s1 = 0;
//	for (double s = s_min; s_max - s >= -0.001; s += d,s1=s1+1)
//	{
//		
//		for (int k = 1; k <= ioI.get_phtonum(); k++)
//		{
//			if (k == 41) continue;
//			int oi = int((floor((k - 1) / 9) - 4));
//			int oj = int((k - 1) % 9 - 4);
//			delta_y = s * oi;
//			delta_x = s * oj;
//			Mat srcImage = ioI.getImg(k), dstImg;
//			dstImg.create(srcImage.size(), CV_64FC1);
//			s_remap(srcImage, dstImg, delta_y, delta_x);
//			s_f_match(s, k, dstImg,s_Errors[s1],k_Errors[k-1],k_Disparity[k-1]);
//		}
//	}
//	//保存视差
//	string imwriteName = ioI.get_str() + ioI.get_write_str() + "Disparity.bmp";
//	ioI.saveImage(imwriteName, Disparity);
//
//	//保存映射后的视差
//	string str_Disparity = ioI.get_str() + ioI.get_write_str() + "Disparity_map.bmp";
//	ioI.slg_save_Mat_Map(Disparity, s_max, s_min, str_Disparity,"float");
//
//	//以文本格式保存视差
//	string txt_str = ioI.get_str() + ioI.get_write_str() + "Disparity.txt";
//	ioI.write_Data(txt_str, Disparity,"double");
//
//	//以文本格式保存最小误差
//	string txt_str_Error = ioI.get_str() + ioI.get_write_str() + "Error.txt";
//	ioI.write_Data(txt_str_Error, Error,"float");
//	std::cout << "-ok"<<endl;
//}
//
//
//
//
//
//
