#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

//out是图像分割后的结果
void areaMatch::adapt_H(Mat* templateMatrix,int H_ksize,int Window_D)
{
	int H_i, H_j, H_k = 1,i ;
	int temp_zhongxin = H_ksize / 2;//整除														
	int temp_zhongxin_yiban = H_ksize / 4; // 以模板的中心
	switch (Window_D)
	{
	case 0:
		for (H_i = temp_zhongxin - temp_zhongxin_yiban; H_i <= temp_zhongxin + temp_zhongxin_yiban; H_i = H_i + 1)
		{//H_i控制行
			for (H_j = temp_zhongxin + 1; H_j < H_ksize; H_j = H_j + 1)
			{//H_j控制列
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // 左侧 
		}
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;

	case 1:
		for (H_i = temp_zhongxin, H_j = temp_zhongxin,i=0; i <temp_zhongxin+1;i++, H_i = H_i - 1, H_j = H_j + 1)
		{//H_i控制层数
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin, H_j = temp_zhongxin+1,i=0; i <temp_zhongxin;i++, H_i = H_i - 1, H_j = H_j + 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin-1, H_j = temp_zhongxin,i=0;i < temp_zhongxin; i++,H_i = H_i - 1, H_j = H_j + 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		break;
	case 2:
		for (H_i = 0; H_i < temp_zhongxin; H_i = H_i + 1)
		{//H_i控制
			for (H_j = temp_zhongxin - temp_zhongxin_yiban; H_j <= temp_zhongxin + temp_zhongxin_yiban; H_j = H_j + 1)
			{
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // 左侧 
		}
	
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;
	case 3:
		for (H_i = temp_zhongxin, H_j = temp_zhongxin, i = 0; i < temp_zhongxin+1; i++, H_i = H_i - 1, H_j = H_j - 1)
		{//H_i控制层数
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin-1, H_j = temp_zhongxin ,i=0; i < temp_zhongxin;i++, H_i = H_i - 1, H_j = H_j - 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin , H_j = temp_zhongxin -1,i=0; i < temp_zhongxin; i++, H_i = H_i - 1, H_j = H_j - 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		break;
	case 4:
		for (H_i = temp_zhongxin - temp_zhongxin_yiban; H_i <= temp_zhongxin + temp_zhongxin_yiban; H_i = H_i + 1)
		{//H_i控制层数
			for (H_j = 0; H_j < temp_zhongxin; H_j = H_j + 1)
			{
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // 左侧 
		}
		
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;
	case 5:
		for (H_i = temp_zhongxin, H_j = temp_zhongxin, i = 0; i < temp_zhongxin+1; i++, H_i = H_i + 1, H_j = H_j -1)
		{//H_i控制层数
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin, H_j = temp_zhongxin-1,i=0; i < temp_zhongxin; i++, H_i = H_i + 1, H_j = H_j - 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin +1, H_j = temp_zhongxin,i=0; i < temp_zhongxin; i++, H_i = H_i + 1, H_j = H_j - 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		break;
	case 6:
		for (H_i = temp_zhongxin + 1; H_i < H_ksize; H_i = H_i + 1)
		{//H_i控制层数
			for (H_j = temp_zhongxin - temp_zhongxin_yiban; H_j <= temp_zhongxin + temp_zhongxin_yiban; H_j = H_j + 1)
			{
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // 左侧 
		}
		
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;
	case 7:

		for (H_i = temp_zhongxin, H_j = temp_zhongxin, i = 0; i < temp_zhongxin+1; i++, H_i = H_i + 1, H_j = H_j + 1)
		{//H_i控制层数
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin, H_j = temp_zhongxin +1,i=0; i < temp_zhongxin; i++, H_i = H_i + 1, H_j = H_j + 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		for (H_i = temp_zhongxin + 1, H_j = temp_zhongxin,i=0; i < temp_zhongxin; i++, H_i = H_i + 1, H_j = H_j + 1)
		{
			templateMatrix->at<uchar>(H_i, H_j) = H_k;
		}
		break;
	}

}
void areaMatch::adapt_H(Mat* templateMatrix, int H_ksize)
{
	int H_i, H_j, H_k = 1;
	int temp_zhongxin = H_ksize / 2;//整除														
	int temp_zhongxin_yiban = H_ksize / 4; // 以模板的中心
	for (H_i = 0; H_i < temp_zhongxin; H_i = H_i + 1, H_k = H_k * 2)
	{
		for (H_j = H_i; H_j < H_ksize - H_i - 1; H_j = H_j + 1) { templateMatrix->at<uchar>(H_j, H_i) = H_k; } // 左侧 
		for (H_j = H_i; H_j < H_ksize - H_i - 1; H_j = H_j + 1) { templateMatrix->at<uchar>(H_ksize - 1 - H_i, H_j) = H_k; } // 下方 		
		for (H_j = H_ksize - 1 - H_i; H_j > H_i; H_j = H_j - 1) { templateMatrix->at<uchar>(H_j, H_ksize - 1 - H_i) = H_k; } // 右侧 
		for (H_j = H_ksize - 1 - H_i; H_j > H_i; H_j = H_j - 1) { templateMatrix->at<uchar>(H_i, H_j) = H_k; } // 上方 
	}
	templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;



}