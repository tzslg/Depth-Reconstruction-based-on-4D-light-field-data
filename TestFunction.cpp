#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void areaMatch::test_adapt_Win_D()
{
	while (true)
	{
		int H_ksize, win_D;
		cout << "input ksize" << endl;
		cin >> H_ksize;
		Mat* templateMatrix = new Mat(H_ksize, H_ksize, CV_8UC1, Scalar::all(0));
		cout << "input win_D" << endl;
		cin >> win_D;
		adapt_H(templateMatrix, H_ksize, win_D);
		for (int i = 0; i < H_ksize; i++)
		{
			for (int j = 0; j < H_ksize; j++)
				cout << int(templateMatrix->at<uchar>(i, j)) << "\t";
			cout << endl;
		}
	}
}
