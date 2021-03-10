#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;


void areaMatch::adapt_window_R(Mat B_Boundary,Mat Window_D,Mat Boundary)
{
	std::cout << "get adapt Windows_R ";
	int temp = 3;//模板边长的临时变量
	int temp1 = 0;
	int sum_temp = 0;
	float temp3 = 0;

	float tn = 0.6;//判断灰度差别足够大的像素是否足够多
	int tv = 0;//判断邻域像素间灰度值差别是否足够大
	int s, t;
	int i, j;

	uchar* data_i_I55 = Y_Img.ptr<uchar>(0);
	uchar* data_Img = Y_Img.ptr<uchar>(0);
	uchar* data_Window_R = Window_R.ptr<uchar>(0);

#pragma loop(hint_parallel(800))
	for (int y = 0; y < Y_Rows; y++)//行
	{
		uchar* data_Img = Y_Img.ptr<uchar>(y);
		uchar* data_Window_R = Window_R.ptr<uchar>(y);
#pragma loop(hint_parallel(800))
		for (int x = 0; x < X_Cols; x++)//列
		{
			if (x == 0 || y == 0 || x == Y_Rows - 1 || y == X_Cols - 1)
				//边界点的模板为1
				data_Window_R[x] = 1;
			else if (x == 1 || y == 1 || x == X_Cols - 2 || y == Y_Rows - 2)
				//边界点的模板为3
			{
				data_Window_R[x] = 3;
			}
			else if (x == 2 || y == 2 || x == X_Cols - 3 || y == Y_Rows - 3)
				//边界点的模板为3
			{
				data_Window_R[x] = 5;
			}
			else
			{
				if (B_Boundary.at<uchar>(y, x) == 255)
				{
					data_Window_R[x] = 7;


				}
				else
				{
					temp = 5;//以5为初始模板大小
					do {
						sum_temp = 0;
						//判断temp+2是否满足
						temp1 = (temp + 1) / 2;
						if (x-temp1 >= 0 && y- temp1 >= 0 && x + temp1< X_Cols && y + temp1< Y_Rows)
						{//判断是否越界
							temp = temp + 2;
							for (i = 0; i < temp; i++)//行
							{
								s = y - temp1 + i;//行
								data_i_I55 = Y_Img.ptr<uchar>(s);
								for (j = 0; j < temp; j++)
								{
									t = x - temp1 + j;//列
									if (t == x || s == y) continue;//矩形中心
									else if (data_Img[x] != data_i_I55[t])
										sum_temp = sum_temp + 1;
								}								
							}
							temp3 = float(sum_temp)/(temp * temp);
						}
						else
							break;					
					} while (temp3 < tn);

				 data_Window_R[x] = temp;
				}
			}
		}
	}

	imwrite(WriteStr + "Window_R.bmp", Window_R*10);
 	write_Data(WriteStr + "Window_R.txt",Window_R, "int");
	std::cout << ">>>>>>>>>end\n";
}
void areaMatch::adapt_window_D(Mat Cb, Mat Cr)
{
	std::cout << "start get_window_D ";
	int muban_min = 0, muban[8] = {0};
	double temp = 0;
	Mat A = min(Cb, Cr);
	for (int i = 3; i < A.rows - 3; i++)//行
		for (int j = 3; j < A.cols - 3; j++)//列
		{
			muban[0] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 3));
			muban[1] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 2))+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 3));
			muban[2] = abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j + 1));
			muban[3] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 3));
			muban[4] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 3));
			muban[5] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 3));
			muban[6] = abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 1));
			muban[7] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 3));
		    temp = muban[0];
			muban_min = 0;
			for (int t = 0; t < 8; t++)
			{
				if (temp >= muban[t])
				{
					temp = muban[t];
					muban_min = t;
				}
				
			}

			Window_D.at<uchar>(i, j) = muban_min;
		}

	for (int i = 0; i < 3; i++)//上3行
		for (int j = 3; j < A.cols - 3; j++)
		{
			muban[5] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 3));
			muban[6] = abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 1));
			muban[7] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 3));
			temp = muban[5];
			muban_min = 5;
			for (int i = 6; i < 8; i++)
			{
				if (temp > muban[i])
				{
					temp = muban[i];
					muban_min = i;
				}
			}
			Window_D.at<uchar>(i, j) = muban_min;
		}

	for (int i = A.rows - 3; i < A.rows; i++)//下行
		for (int j = 3; j < A.cols - 3; j++)
		{
			muban[1] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 3));
			muban[2] = abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j + 1));
			muban[3] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 3));
			temp = muban[1];
			muban_min = 1;
			for (int i = 2; i < 4; i++)
			{
				if (temp > muban[i])
				{
					temp = muban[i];
					muban_min = i;
				}
			}
			Window_D.at<uchar>(i, j) = muban_min;
		}

	for (int i = 3; i < A.rows - 3; i++)//右行
		for (int j = 0; j < 3; j++)
		{
			muban[0] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 3));
			muban[1] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j + 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 3));
			muban[2] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j + 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j + 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j + 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j + 3));
			temp = muban[0];
			muban_min = 0;
			for (int i = 1; i < 3; i++)
			{
				if (temp > muban[i])
				{
					muban_min = i;
					temp = muban[i];
					if (i == 2)
						muban_min = 7;
				}
			}
			Window_D.at<uchar>(i, j) = muban_min;
		}

	for (int i = 3; i < A.rows - 3; i++)//左两行
		for (int j = A.cols - 3; j < A.cols; j++)
		{
			muban[3] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 2, j - 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 3, j - 3));
			muban[4] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i - 1, j - 3))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 3));
			muban[5] = abs(A.at<uchar>(i, j) - A.at<uchar>(i, j - 1))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 1, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 1)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 2, j - 2))
				+ abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 2)) + abs(A.at<uchar>(i, j) - A.at<uchar>(i + 3, j - 3));
			temp = muban[3];
			muban_min = 3;
			for (int i = 4; i < 6; i++)
			{
				if (temp > muban[i])
				{
					temp = muban[i];
					muban_min = i;
				}
			}
			Window_D.at<uchar>(i, j) = muban_min;
		}
	//处理四个顶点
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			Window_D.at<uchar>(i, j) = 7;
	for (int i = A.rows - 3; i < A.rows; i++)
		for (int j = A.cols - 3; j < A.cols; j++)
			Window_D.at<uchar>(i, j) = 3;
	for (int i = A.rows - 3; i < A.rows; i++)
		for (int j = 0; j < 3; j++)
			Window_D.at<uchar>(i, j) = 1;
	for (int i = 0; i < 3; i++)
		for (int j = A.cols - 3; j < A.cols; j++)
			Window_D.at<uchar>(i, j) = 5;
	
	imwrite(WriteStr + "Window_D.bmp", Window_D * 36);
	imwrite(WriteStr + "A.bmp", A);

	write_Data(WriteStr + "Window_D.txt", Window_D, "int");
	std::cout << ">>>>>>>>>end\n";
}