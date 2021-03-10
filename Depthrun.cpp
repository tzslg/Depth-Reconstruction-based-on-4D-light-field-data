#include"pch.h"
#include"areaMatch.h"
#include <iostream>
#include<string>

using namespace std;
using namespace cv;

int main()
{
	string str = "C:/Users/App-Math/Desktop/", read_str = "full_data/full_data/additional/antinous/", write_str = "结果对比/新基准/additional/antinuos/";
	double s_min = -1.17, s_max = 0.89;
	double d = 0.01;
	string temp = "0";
	string str_temp;

	cout<<"str="<<str<<endl;
	cout<<"read_str="<<read_str<<endl;
	cout<<"write_str="<<write_str<<"\n"<<endl;

	std::cout << "是否更改当前路径(是：1；否：0;) \n:";
	cin >> temp;
	if (temp == "0");
	else
	{
		cout << "\n输入当前路径 (eg:"<<str<<endl;
		cin >> str_temp;
		str=str_temp;
	}

	std::cout << "\n是否输入读取数据的文件夹：(默认 0 :papillon/)\n:";
	cin >> temp;
	if (temp == "0");
	else
	{
		cout << "\n输入读取数据文件夹：(eg:papillon/) \n:";
		cin >> str_temp;
		read_str=str_temp;
	}
	std::cout << "\n是否输入写入数据的文件夹: (默认 0 :saveDisparity/)\n:";
	cin >> temp;
	if (temp == "0");
	else
	{
		cout << "\n输入写入数据文件夹：(eg:saveDisparity/)\n:";
		cin >> str_temp;
		write_str=str_temp;
	}

	std::cout << "\n是否输入搜索视差: (默认 0；s_max=0.89,s_min=-1.17)\n:";
	cin >> temp;
	if (temp == "0");
	else
	{

		cout << "\n依次输入搜索视差s_max,s_min\n:";
		cin >> s_max >> s_min;
	}

	std::cout << "\n是否需要修改视差搜索步长: (默认 0；d=0.01)\n:";
	cin >> temp;
	if (temp == "0");
	else
	{

		cout << "\n依次输入视差搜索步长d\n:";
		cin >> d;
	}



	Mat I55_ = imread(str + read_str + "55.bmp");
	areaMatch Match(s_max, s_min,d, str, read_str, write_str, I55_);
	try
	{
	    Match.run();

	}
	catch (const std::exception& e)
	{
		cout << "error" << endl;
		return 0;

	}
	std::system("pause");
	return 0;
}


//  运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
//  调试程序: F5 或调试 >“开始调试”菜单
//  入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

