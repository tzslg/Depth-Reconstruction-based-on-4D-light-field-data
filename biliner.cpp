#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

//out是图像分割后的结果
void areaMatch::biliner(Mat* dstImg,double s,int k)
{
	//以下变量用于双线性插值
	double delta_y, delta_x;
	double y_remap, x_remap;
	uchar index00, index01, index10, index11;
	int xi, yi, r, c;
	double xd, yd;
	double* data_dstImg = dstImg->ptr<double>(0);
	
	
	delta_y = s * oi[k];
	delta_x = s * oj[k];
	
	for (r = 0; r < Y_Rows; r++)
	{
		y_remap = r - delta_y;
		data_dstImg = dstImg->ptr<double>(r);
		for (c = 0; c < X_Cols; c++)
		{
			x_remap = c + delta_x;
			//整数部分
			xi = (int)x_remap;
			yi = (int)y_remap;
			//小数部分
			xd = x_remap - xi;
			yd = y_remap - yi;
			//边界处理
			xi = xi > 0 ? xi : 0;
			xi = xi < X_Cols - 1 ? xi : X_Cols - 2;
			yi = yi > 0 ? yi : 0;
			yi = yi < Y_Rows - 1 ? yi : Y_Rows - 2;
			index00 = srcImg.at<uchar>(yi, xi);
			index01 = srcImg.at<uchar>(yi, xi + 1);
			index10 = srcImg.at<uchar>(yi + 1, xi);
			index11 = srcImg.at<uchar>(yi + 1, xi + 1);
			//计算，得到插值数据
			data_dstImg[c] = (1 - xd) * (1 - yd) * index00 + (1 - xd) * yd * index01 + xd * (1 - yd) * index10 + xd * yd * index11;
		}
	}
	//双线性插值结束



}