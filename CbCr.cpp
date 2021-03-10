#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

//out是图像分割后的结果
void areaMatch:: Division(Mat Cb, Mat Cr, int clusterCount,Mat& out)
{
	Mat A = min(Cb, Cr);
	//生成一维采样点,包括所有图像像素点,注意采样点格式为32bit浮点数。 
	Mat samples(X_Cols * Y_Rows, 1, CV_32FC1);
	//标记矩阵，32位整形 
	Mat labels(X_Cols * Y_Rows, 1, CV_32SC1);
	int i, j, k = 0;
	uchar* p0;
	for (i = 0; i < Y_Rows; i++)
	{
		p0 = A.ptr<uchar>(i);
		for (j = 0; j < X_Cols; j++)
		{
			samples.at<float>(i*X_Cols+j,0)= float(p0[j]);
		}
	}
	Mat centers;

	kmeans(samples, clusterCount, labels,
		TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 0.01),
		100, KMEANS_RANDOM_CENTERS, centers);

	//samples表示数据，clusterCount表示分k类，labels表示分类标签，TermCriteria（迭代终止条件，迭代次数，预期精度），
	//K-means算法执行次数，初始化均值点，输出的均值点
	float step = float(255 / (clusterCount - 1));
	k = 0;
	uchar * p;
	for (i = 0; i < out.rows; i++)
	{
		p = out.ptr<uchar>(i);
		for (j = 0; j < out.cols; j++)
		{
			int tt = labels.at<int>(k, 0);
			k++;
			p[j] = uchar(255 - tt * step);
		}
	}

	imwrite(WriteStr + "ChromaBlock.bmp", out);
}
//返回的是图像的边缘
void areaMatch::BoundaryContours(Mat src,Mat& dst ) {
	Mat canny_output;
	int threshold_value = 100;
	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	Canny(src, canny_output, threshold_value, threshold_value * 2, 3, false);
	findContours(canny_output, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<vector<Point>> it = contours;
	for (size_t i = 0; i < contours.size(); i++) {
		Rect rect = boundingRect(contours[i]);
		if (rect.area() > 3000)
			//更改提取的面积
			drawContours(dst, contours, i, Scalar::all(255));
	}
	imwrite(WriteStr + "Boundary.bmp", dst);
	write_Data(WriteStr + "Boundary.txt", dst, "int");
}
//返回的是扩充的边缘
void areaMatch::BigBoundaryCountours(int element_size, Mat src, Mat& dst){
	int s = element_size;
	Mat structureElement = getStructuringElement(MORPH_RECT, Size(s, s), Point(0, 0));
	erode(src, dst, structureElement);
	dilate(src, dst, structureElement, Point(0, 0), 1);
	imwrite(WriteStr + "Big_Boundary.bmp", dst);
	write_Data(WriteStr + "Big_Boundary.txt", dst, "int");
}