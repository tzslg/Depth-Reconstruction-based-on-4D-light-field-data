#include"pch.h"
//#include"areaMatch.h"
//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <cmath>
//#include <set>
//
//using namespace std;
//using namespace cv;
//
//Mat areaMatch::slg_seek_Depth(Mat** slg_set_temp1) {
//	cout << "误差集合生成.....";
//	set<float>** slg_set_temp = new set<float> * [y_rows * x_cols];
//	for (int set_i = 0; set_i < y_rows; set_i++)
//		for (int set_j = 0; set_j < y_rows; set_j++)
//		{
//			slg_set_temp[set_i * y_rows + set_j] = new set<float>;
//		}
//
//	cout << "..end\n";
//
//	std::cout << "start seek_Disparity..\n";
//	//边界限定
//
//	//边缘区域
//
//	//深度连续区域
//
//	for(int i=0;i<y_rows;i++)
//		for (int j = 0; j < x_cols; j++)
//		{
//			//四邻域表示
//			//当前像素点 
//			int x_y=i*y_rows+j;
//			//上 
//			int yy_=(i-1)*y_rows+j;
//			//下 
//			int y_=(i+1)*y_rows+j;
//			//左
//			int xx_=i*y_rows+(j-1);
//			//右 
//			int x_=i*y_rows+(j+1);
//			float temp=0,temp_out=0; int ep = 0.001;
//			set<float>::iterator x_y_it,yy_it,y_it,xx_it,x_it;
//			for (x_y_it = slg_set_temp[x_y]->begin(), yy_it = slg_set_temp[yy_]->begin(), y_it = slg_set_temp[y_]->begin(),
//				xx_it = slg_set_temp[xx_]->begin(), x_it = slg_set_temp[x_y]->begin();
//				x_y_it != slg_set_temp[x_y]->end(); x_y_it++) {
//				for (yy_it != slg_set_temp[yy_]->end();	yy_it != slg_set_temp[yy_]->end(); yy_it++)
//				{
//					temp_out = log(1 / abs(*x_y_it - *yy_it + ep));
//				}
//				for (y_it != slg_set_temp[y_]->end(); y_it != slg_set_temp[y_]->end(); y_it++)
//				{
//					temp_out += log(1 / abs(*x_y_it - *y_it + ep));
//				}
//				for (xx_it != slg_set_temp[xx_]->end(); xx_it != slg_set_temp[xx_]->end(); xx_it++)
//				{
//					temp_out += log(1 / abs(*x_y_it - *xx_it + ep));
//				}
//				for (x_it != slg_set_temp[x_]->end(); x_it != slg_set_temp[x_]->end(); x_it++)
//				{
//					temp_out += log(1 / abs(*x_y_it - *x_it + ep));
//				}
//			if (x_y_it == slg_set_temp[x_y]->begin())
//			{
//				temp=temp_out;
//			}
//			else
//			{
//				if (temp< temp_out)
//				{
//		//需要建立新的set集合吗？
//		//如何取最大项呢？
//		//建立key 临时变量 字典
//		/*
//		把视差值作为字典的索引值，概率值作为字典的值
//		通过字典的值，去除字典元素，然后赋值给视差集合
//		*/
//				}
//				printf("%d\t", *x_y_it);
//			}
//			slg_set_temp[x_y];
//		}
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	。，；lk