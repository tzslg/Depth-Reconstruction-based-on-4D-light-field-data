#pragma once
#include<string>
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

class ioImage
{

		public:
		string str;
		string read_str;
		string write_str;
		int phtonum = 81;
		int oi[81], oj[81];

		ioImage(string s1, string s2, string s3) :str(s1), read_str(s2), write_str(s3){

			for (int k = 0; k < 81; k++)
			{
				oi[k] = int((floor(k / 9) - 4));
				oj[k] = int(k % 9 - 4);
			}
		};
		void Read_RGB_Y(int k,Mat& srcImage_Y)
		{
			//k从1开始
			cout << "read  " << k << "  and RGB to Y ";
			Mat Y;
			vector<Mat> channels1;
			Mat image = imread(str + read_str + to_string(int(floor((k - 1) / 9) + 1)) + to_string(int((k - 1) % 9 + 1)) + ".bmp");
			if (image.empty())
				cout << "k image empty read error" << endl;
			cvtColor(image, Y, CV_BGR2YCrCb);
			split(Y, channels1);
			srcImage_Y = channels1.at(0);
			cout << ">>>>>>end.\n";
		};

		void Read_RGB_YCbCr(int k, Mat& image, Mat& srcImage_Y, Mat& srcImage_Cb, Mat& srcImage_Cr)
		{
			//k从1开始
			cout << "read  "<< k <<"  and RGB to YCbCr ";
			Mat YCbCr;
			vector<Mat> channels1;
			image = imread(str + read_str + to_string(int(floor((k - 1) / 9) + 1)) + to_string(int((k - 1) % 9 + 1)) + ".bmp");
			if (image.empty())
			cout << "k image read error" << endl;
			cvtColor(image, YCbCr, CV_BGR2YCrCb);
			split(YCbCr, channels1);
			srcImage_Y= channels1.at(0);
			srcImage_Cb = channels1.at(1);
			srcImage_Cr = channels1.at(2);
			cout << ">>>>>>end\n";
		};

		static int write_Data(string fileName, cv::Mat& mat_Data, string Type)
			{
				int retVal = 0;
				if (mat_Data.empty())
				{
					cout << "数据为空" << endl;
					retVal = 1;
					return (retVal);
				}

				ofstream outFile(fileName.c_str(), ios_base::out);
				if (!outFile.is_open())
				{
					cout << "文件打开失败" << endl;
					retVal = -1;
					return (retVal);
				}

				if (Type == "double")
					for (int r = 0; r < mat_Data.rows; r++)
					{
						for (int c = 0; c < mat_Data.cols; c++)
						{
							double data = mat_Data.at<double>(r, c);
							outFile << data << " ";
						}
						outFile << endl;
					}

				else if (Type == "float")
					for (int r = 0; r < mat_Data.rows; r++)
					{
						for (int c = 0; c < mat_Data.cols; c++)
						{
							float data = mat_Data.at<float>(r, c);
							outFile << data << " ";
						}
						outFile << endl;
					}
				else if (Type == "uchar")
					for (int r = 0; r < mat_Data.rows; r++)
					{
						for (int c = 0; c < mat_Data.cols; c++)
						{
							uchar data = mat_Data.at<uchar>(r, c);
							//读取的类型uchar，保存的类型int
							outFile << data << " ";
						}
						outFile << endl;
					}


				else if (Type == "int")
					for (int r = 0; r < mat_Data.rows; r++)
					{
						for (int c = 0; c < mat_Data.cols; c++)
						{
							int data = mat_Data.at<uchar>(r, c);
							outFile << data << " ";
						}
						outFile << endl;
					}
				else 
					cout<< "还没有该类型\n";

				return (retVal);
			};
	
		int cutJPG(string read_name, string write_name, string s_)
		{
			for (int i = 0, k = 1; k <= 81; k++)
			{
				int k1, k2;
				k1 = int(floor((k - 1) / 9) + 1);
				k2 = int((k - 1) % 9 + 1);
				if (k2 == 1)
				{
					i = i + 1;
				}
				string imreadName = read_name + to_string(k1) + to_string(k2) + s_;

				string imwriteName = write_name + to_string(k1) + to_string(k2) + s_;

				Mat im = imread(imreadName, -1);
				if (im.empty())
				{
					cout << imreadName << "Could not open or find the image" << std::endl;
					return -1;
				}
				Mat result = im(Rect(427, 161, 600, 600));
				imwrite(imwriteName, result);

			}
			return 0;
		}
		void saveImage(string imwriteName, Mat result)
		{

			imwrite(imwriteName, result);

		}

		void slg_save_Mat_Map(const Mat A, double max, double min, string Mat_str_name, string Type)
			{
				Mat A_map = A.clone();

				if (Type == "double")
					for (int y = 0; y < A.rows; y++)
					{
						double*Data_A_map = A_map.ptr<double>(y);
						for (int x = 1; x < A.cols - 1; x++)
							Data_A_map[x] = ((Data_A_map[x] - min) * 255) / (max - min);
					}
				else if (Type == "double")
				{
					for (int y = 0; y < A.rows; y++)
					{
						double *Data_A_map = A_map.ptr<double>(y);
						for (int x = 1; x < A.cols - 1; x++)
							Data_A_map[x] = double(((Data_A_map[x] - min) * 255) / (max - min));
					}
				}
				else
				{
					cout << "请重新写该类型保存映射函数\n";
				}
				saveImage(Mat_str_name, A_map);
			}

};