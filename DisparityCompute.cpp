#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void areaMatch::Disparity_compute()
{

	std::cout << "start Disparity_compute...\n";
	//���±������ڶ�̬ģ��ƥ��
	double data_Xout = 0;
	double sum = 0.0;
	int truesum = 0;
	double* data_Disparity = Disparity.ptr<double>(0);
	double* data_Error = Error.ptr<double>(0);

	//���±�������ģ��
	int ss = 0, tt = 0;
	int win_D, H_ksize = 0;
	int temp_zhongxin;
	//x��y����ƥ���ʾ��&��
	int x, y;
	double s;
	for (int k = 0; k < 81; k++)
	{
		if (k == 40) continue;
		//���溯������k��1��ʼ
		Read_RGB_Y(k + 1, srcImg);
		for (s = s_min; s <= s_max; s = s + d)
		{
			Mat* dstImg_p = &dstImg;
			biliner(dstImg_p, s, k);
			double* data_dstImg = dstImg_p->ptr<double>(0);
			uchar* data_Img_Y = Y_Img.ptr<uchar>(0);
			for (y = 0; y < Y_Rows; y++)
			{
				//�����ӵ�ͼ��
				data_Img_Y = Y_Img.ptr<uchar>(y);
				//��ƥ��˫���Բ�ֵͼ��
				data_dstImg = dstImg_p->ptr<double>(y);
				data_Disparity = Disparity.ptr<double>(y);
				data_Error = Error.ptr<double>(y);
				for (x = 0; x < X_Cols; x++)
				{
					//��ʡ��������ʱ��  
					if (abs(data_Img_Y[x] - data_dstImg[x]) >= 0.01)
						continue;

					H_ksize = Window_R.at<uchar>(y, x);
					Mat * templateMatrix = new Mat(H_ksize, H_ksize, CV_8UC1, Scalar::all(0));
					if (B_Boundary.at<uchar>(y, x) == 255)
					{
						win_D = Window_D.at<uchar>(y, x);
						adapt_H(templateMatrix, H_ksize, win_D);
					}
					else
						adapt_H(templateMatrix, H_ksize);


					temp_zhongxin = H_ksize / 2;//����
#pragma loop(hint_parallel(10))
					for (int i = 0; i < H_ksize; i++)
					{
						//��һ��ѭ�������� 
						ss = y - temp_zhongxin + i;//ģ����
						data_Img_Y = Y_Img.ptr<uchar>(ss);
						data_dstImg = dstImg_p->ptr<double>(ss);
						for (int j = 0; j < H_ksize; j++)
						{//�ڶ���ѭ��������
							tt = x - temp_zhongxin + j;
							if (templateMatrix->at<uchar>(i, j) != 0)
							{
								if (abs(data_Img_Y[tt] - data_dstImg[tt]) < 0.1)
								{
									truesum = truesum + 1;
								}
					//			sum = sum + templateMatrix->at<uchar>(i, j) * pow((data_Img_Y[tt] - data_dstImg[tt]), 2);								
							}
						}
					}
					
	//cout << "y="<<y<<"x="<<x<<"\t"<<H_ksize<<"\tB_Boundary="<< int(B_Boundary.at<uchar>(y, x))<<" "<<win_D<<"\t" << float(truesum)/(H_ksize * H_ksize) <<"\t"<<truesum<<"\t"<< sqrt(sum) << endl;
					//data_Xout =sum; 
					data_Xout = -truesum;

					delete templateMatrix;
					//��ʼ��
					if (s == s_min && k == 0)
					{
						data_Error[x] = data_Xout;
					}
					else if (data_Xout < data_Error[x])
					{
						data_Disparity[x] = s;
						
						data_Error[x] = data_Xout+Update(s, y, x, k);

					}
					sum = 0.0; truesum = 0;
				}
			}
			//imwrite(WriteStr + "dst.bmp", dstImg);
		}
	}
	//�Ӳ�ֵ���ǹ����ģ�ƥ��ģ���������ж����������
	//Ϊʲô��������þ���������ƥ��ģ���أ�����Ϊ���������Ӳ�ֵ���ǹ����ģ����ǳ�Ƭ��
	//����ӳ�����Ӳ�
	slg_save_Mat_Map(Disparity, s_max, s_min, WriteStr + "Disparity.bmp", "double");
	//���ı���ʽ�����Ӳ�
	write_Data(WriteStr + "Disparity.txt", Disparity, "double");
};
double areaMatch::Update(double s, int y, int x, int k)
{//sum>5,˵��ģ����������4���Ӳ�ֵ����ͬ�ģ�Ҳ������ʵ
	if (x == 0 || y == 0 || x == Y_Rows - 1 || y == X_Cols - 1 || B_Boundary.at<uchar>(y, x) == 255 ||k<30)
		//�߽���ģ��Ϊ1
		return 0;
	else
	{
		double a = 0.1;
		double temp = (Disparity.at<double>(y - 1, x) + Disparity.at<double>(y + 1, x) + Disparity.at<double>(y, x - 1) + Disparity.at<double>(y, x + 1)) / 4;
		if (s - temp < Disparity.at<double>(y, x) - temp)
			return -a;
		else
		{
			return a;
		}
	}
	
};