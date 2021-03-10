#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>
void areaMatch::TV(int iteration)
{
	double lambda = 1;
	double ep = 0.02;
	IplImage* src = (IplImage*)& IplImage(Disparity);
	int i, j;
	int M = src->height;
	int N = src->width;
	CvMat* dst = cvCreateMat(M, N, CV_64FC1);//存放浮点图像
	cvConvert(src, dst);

	double t = getTickCount();//当前滴答数
	double UO, UN, UW, US, UE, UNE, UNW, USW, USE;
	double Un, Ue, Uw, Us;
	double Wn, We, Ww, Ws, Wo;
	double Hon, Hoe, How, Hos;
	double Hoo;
	while (iteration)
	{
		std::cout << iteration << std::endl;
		for (i = 1; i < M - 1; i++)
		{
			for (j = 1; j < N - 1; j++)
			{
				if (B_Boundary.at<uchar>(i,j) != 255)//坏损区
				{
					UO = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * i))[j];
					UN = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * (i - 1)))[j];
					US = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * (i + 1)))[j];
					UE = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * i))[j + 1];
					UW = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * i))[j - 1];

					UNE = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * (i - 1)))[j + 1];
					UNW = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * (i - 1)))[j - 1];
					USE = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * (i + 1)))[j + 1];
					USW = ((double*)(void*)(dst->data.ptr + (size_t)dst->step * (i + 1)))[j - 1];

					Un = sqrt((UO - UN) * (UO - UN) + ((UNW - UNE) / 2.0) * ((UNW - UNE) / 2.0));
					Ue = sqrt((UO - UE) * (UO - UE) + ((UNE - USE) / 2.0) * ((UNE - USE) / 2.0));
					Uw = sqrt((UO - UW) * (UO - UW) + ((UNW - USW) / 2.0) * ((UNW - USW) / 2.0));
					Us = sqrt((UO - US) * (UO - US) + ((USW - USE) / 2.0) * ((USW - USE) / 2.0));

					Wn = 1.0 / sqrt(Un * Un + ep * ep);
					We = 1.0 / sqrt(Ue * Ue + ep * ep);
					Ww = 1.0 / sqrt(Uw * Uw + ep * ep);
					Ws = 1.0 / sqrt(Us * Us + ep * ep);
					//ep是为了防止梯度为0引入的系数
					Wo = 0.5 * lambda;

					Hon = Wn / (Wn + We + Ww + Ws + Wo);
					Hoe = We / (Wn + We + Ww + Ws + Wo);
					How = Ww / (Wn + We + Ww + Ws + Wo);
					Hos = Ws / (Wn + We + Ww + Ws + Wo);

					Hoo = Wo / (Wn + We + Ww + Ws + Wo);
					((double*)(void*)(dst->data.ptr + (size_t)dst->step * i))[j] = (Hon * UN + Hoe * UE + How * UW + Hos * US + Hoo * UO);
				}
			}
		}
		iteration--;
	}


	cvConvert(dst, src);
	t = ((double)getTickCount() - t) / getTickFrequency();
	std::cout<<"算法用时："<<t<<"秒\n";
}