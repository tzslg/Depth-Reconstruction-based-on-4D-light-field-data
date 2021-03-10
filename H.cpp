#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

//out��ͼ��ָ��Ľ��
void areaMatch::adapt_H(Mat* templateMatrix,int H_ksize,int Window_D)
{
	int H_i, H_j, H_k = 1,i ;
	int temp_zhongxin = H_ksize / 2;//����														
	int temp_zhongxin_yiban = H_ksize / 4; // ��ģ�������
	switch (Window_D)
	{
	case 0:
		for (H_i = temp_zhongxin - temp_zhongxin_yiban; H_i <= temp_zhongxin + temp_zhongxin_yiban; H_i = H_i + 1)
		{//H_i������
			for (H_j = temp_zhongxin + 1; H_j < H_ksize; H_j = H_j + 1)
			{//H_j������
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // ��� 
		}
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;

	case 1:
		for (H_i = temp_zhongxin, H_j = temp_zhongxin,i=0; i <temp_zhongxin+1;i++, H_i = H_i - 1, H_j = H_j + 1)
		{//H_i���Ʋ���
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
		{//H_i����
			for (H_j = temp_zhongxin - temp_zhongxin_yiban; H_j <= temp_zhongxin + temp_zhongxin_yiban; H_j = H_j + 1)
			{
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // ��� 
		}
	
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;
	case 3:
		for (H_i = temp_zhongxin, H_j = temp_zhongxin, i = 0; i < temp_zhongxin+1; i++, H_i = H_i - 1, H_j = H_j - 1)
		{//H_i���Ʋ���
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
		{//H_i���Ʋ���
			for (H_j = 0; H_j < temp_zhongxin; H_j = H_j + 1)
			{
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // ��� 
		}
		
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;
	case 5:
		for (H_i = temp_zhongxin, H_j = temp_zhongxin, i = 0; i < temp_zhongxin+1; i++, H_i = H_i + 1, H_j = H_j -1)
		{//H_i���Ʋ���
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
		{//H_i���Ʋ���
			for (H_j = temp_zhongxin - temp_zhongxin_yiban; H_j <= temp_zhongxin + temp_zhongxin_yiban; H_j = H_j + 1)
			{
				templateMatrix->at<uchar>(H_i, H_j) = H_k;
			} // ��� 
		}
		
		templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;
		break;
	case 7:

		for (H_i = temp_zhongxin, H_j = temp_zhongxin, i = 0; i < temp_zhongxin+1; i++, H_i = H_i + 1, H_j = H_j + 1)
		{//H_i���Ʋ���
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
	int temp_zhongxin = H_ksize / 2;//����														
	int temp_zhongxin_yiban = H_ksize / 4; // ��ģ�������
	for (H_i = 0; H_i < temp_zhongxin; H_i = H_i + 1, H_k = H_k * 2)
	{
		for (H_j = H_i; H_j < H_ksize - H_i - 1; H_j = H_j + 1) { templateMatrix->at<uchar>(H_j, H_i) = H_k; } // ��� 
		for (H_j = H_i; H_j < H_ksize - H_i - 1; H_j = H_j + 1) { templateMatrix->at<uchar>(H_ksize - 1 - H_i, H_j) = H_k; } // �·� 		
		for (H_j = H_ksize - 1 - H_i; H_j > H_i; H_j = H_j - 1) { templateMatrix->at<uchar>(H_j, H_ksize - 1 - H_i) = H_k; } // �Ҳ� 
		for (H_j = H_ksize - 1 - H_i; H_j > H_i; H_j = H_j - 1) { templateMatrix->at<uchar>(H_i, H_j) = H_k; } // �Ϸ� 
	}
	templateMatrix->at<uchar>(temp_zhongxin, temp_zhongxin) = H_k;



}