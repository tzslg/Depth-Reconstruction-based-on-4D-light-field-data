#pragma once
#include <iostream>
#include"ioImage.h"
#include<set>
#include<vector>
#include <iterator>
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv;

class areaMatch : public ioImage {

	//视差搜索范围
	double s_max, s_min;
	//视差搜索步长
	double d;

	//行列取值
	int Y_Rows;
	int X_Cols;
	//存储路径
	string WriteStr;
	//中心视图	
	Mat I55, Y_Img, Cb_Img,Cr_Img;
	//待匹配视图
	Mat srcImg;
	Mat dstImg;
	Mat Window_R, Window_D, Boundary, B_Boundary, ChromaBlock;
	//最小视差对应的距离测度函数值
	Mat Error;
	//视差
	Mat Disparity;
public:
	areaMatch(double s_max_, double s_min_, double d_,string s1, string s2, string s3, Mat I55_) :s_max(s_max_), s_min(s_min_), d(d_),ioImage(s1, s2, s3),I55(I55_)
	{
		Y_Rows = I55_.rows;
		X_Cols = I55_.cols;
	
		Y_Img = Mat(Y_Rows,X_Cols, CV_8UC1, Scalar::all(0));
		Cb_Img = Mat(Y_Rows,X_Cols, CV_8UC1, Scalar::all(0));
		Cr_Img = Mat(Y_Rows,X_Cols, CV_8UC1, Scalar::all(0));

		srcImg = Mat(Y_Rows, X_Cols, CV_8UC1, Scalar::all(0));
		dstImg = Mat(Y_Rows, X_Cols, CV_64FC1, Scalar::all(0));

		Window_R = Mat(Y_Rows, X_Cols, CV_8UC1, Scalar::all(0));
		Window_D = Mat(Y_Rows, X_Cols, CV_8UC1, Scalar::all(0));
		Boundary = Mat(Y_Rows, X_Cols, CV_8UC1, Scalar::all(0));
		B_Boundary = Mat(Y_Rows, X_Cols, CV_8UC1, Scalar::all(0));
		ChromaBlock = Mat(Y_Rows, X_Cols, CV_8UC1, Scalar::all(0));

		Error = Mat(Y_Rows,X_Cols, CV_64FC1, Scalar::all(0));
		//合理初始化
		Disparity = Mat(Y_Rows,X_Cols, CV_64FC1, Scalar::all(s_min));
	};
	void read()
	{
		WriteStr = str + write_str;
		cout << "imread and imwrite Y_Img ... ";
		Read_RGB_YCbCr(41,I55, Y_Img, Cb_Img,Cr_Img);
		saveImage(str + write_str + "color_55.bmp",I55);//保存图像
		saveImage(WriteStr + "55Y.bmp", Y_Img);//保存图像
		saveImage(WriteStr + "55Cb.bmp", Cb_Img);//保存图像
		saveImage(WriteStr + "55Cr.bmp", Cr_Img);//保存图像
	};
	void test_adapt_Win_D();
	void adapt_window_D(Mat Cb, Mat Cr);//更新window_D
	void Division(Mat Cb, Mat Cr, int clusterCount,Mat& out);//更新ChromaBlock
	void BoundaryContours(Mat src,Mat & dst);//更新Boundary
	void BigBoundaryCountours(int element_size, Mat src,Mat &dst);//更新BigBoundary
	void adapt_window_R(Mat B_Boundary, Mat Boundary, Mat Window_D);//更新Window_R
	void adapt_H(Mat *,int);
	void adapt_H(Mat* , int,int);
	void biliner(Mat* ,double s, int k);
	void Disparity_compute();
	double Update(double s, int y, int x, int k);
	void TV(int iteration);
	void run()
	{
		read();//进一步初始化
		Division(Cb_Img,Cr_Img,5,ChromaBlock);
		BoundaryContours(ChromaBlock,Boundary);
		BigBoundaryCountours(2, Boundary, B_Boundary);
		adapt_window_D(Cb_Img ,Cr_Img);//更新Window_D
		adapt_window_R(B_Boundary,Window_D,Boundary);//更新Window_R
		Disparity_compute();

		Mat Disparity_New = Disparity.clone();
		int iterator_true = 10;//TV迭代次数
		TV(iterator_true);
		slg_save_Mat_Map(Disparity, s_max, s_min, WriteStr + "Disparity_opt.bmp", "double");
	};


};


//	filterSpeckles(Disparity_New,20,50,50);
//	imwrite(WriteStr + "Disparity_New.bmp",Disparity_New);
/** @brief Filters off small noise blobs (speckles) in the disparity map

@param img The input 16-bit signed disparity image
@param newVal The disparity value used to paint-off the speckles
@param maxSpeckleSize The maximum speckle size to consider it a speckle. Larger blobs are not
affected by the algorithm
@param maxDiff Maximum difference between neighbor disparity pixels to put them into the same
blob. Note that since StereoBM, StereoSGBM and may be other algorithms return a fixed-point
disparity map, where disparity values are multiplied by 16, this scale factor should be taken into
account when specifying this parameter value.
@param buf The optional temporary buffer to avoid memory allocation within the function.
 */
