#include"pch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

/*
filterSpeckles（）视差图后处理
两种立体匹配算法在算出初始视差图后会进行视差图后处理，包括中值滤波，连通域检测等。
其中中值滤波能够有效去除视差图中孤立的噪点，而连通域检测能够检测出视差图中因噪声引起小团块(blob)。
speckleWindowSize是指设置检测出的连通域中像素点个数，也就是连通域的大小。
speckleRange是指设置判断两个点是否属于同一个连通域的阈值条件。
大概流程如下：
判断当前像素点四邻域的邻域点与当前像素点的差值diff，
如果diff < speckRange，则表示该邻域点与当前像素点是一个连通域，设置一个标记。
然后再以该邻域点为中心判断其四邻域点，步骤同上。
直至某一像素点四邻域的点均不满足条件，则停止。
步骤1完成后，判断被标记的像素点个数count，
如果像素点个数count <= speckleWindowSize，
则说明该连通域是一个小团块(blob)，则将当前像素点值设置为newValue(表示错误的视差值，newValue一般设置为负数或者0值)。
否则，表示该连通域是个大团块，不做处理。
同时建立标记值与是否为小团块的关系表rtype[label]，
rtype[label]为0，表示label值对应的像素点属于小团块，为1则不属于小团块。
处理下一个像素点时，先判断其是否已经被标记：
如果已经被标记，则根据关系表rtype[label]判断是否为小团块（blob），
如果是，则直接将该像素值设置为newValue；如果不是，则不做处理。继续处理下一个像素。
如果没有被标记，则按照步骤1处理。
所有像素点处理后，满足条件的区域会被设置为newValue值，后续可以用空洞填充等方法重新估计其视差值。
*/

typedef cv::Point_<short> Point2s;
template <typename T> void filterSpecklesImpl(cv::Mat& img, int newVal, int maxSpeckleSize, int maxDiff, cv::Mat& _buf)
{
	using namespace cv;

	int width = img.cols, height = img.rows, npixels = width * height;
	size_t bufSize = npixels * (int)(sizeof(Point2s) + sizeof(int) + sizeof(uchar));
	if (!_buf.isContinuous() || _buf.empty() || _buf.cols * _buf.rows * _buf.elemSize() < bufSize)
		_buf.create(1, (int)bufSize, CV_8U);

	uchar * buf = _buf.ptr();
	int i, j, dstep = (int)(img.step / sizeof(T));
	int* labels = (int*)buf;
	buf += npixels * sizeof(labels[0]);
	Point2s * wbuf = (Point2s*)buf;
	buf += npixels * sizeof(wbuf[0]);
	uchar * rtype = (uchar*)buf;
	int curlabel = 0;

	// clear out label assignments
	memset(labels, 0, npixels * sizeof(labels[0]));

	for (i = 0; i < height; i++)
	{
		T* ds = img.ptr<T>(i);
		int* ls = labels + width * i;

		for (j = 0; j < width; j++)
		{
			if (ds[j] != newVal)   // not a bad disparity
			{
				if (ls[j])     // has a label, check for bad label
				{
					if (rtype[ls[j]]) // small region, zero out disparity
						ds[j] = (T)newVal;
				}
				// no label, assign and propagate
				else
				{
					Point2s* ws = wbuf; // initialize wavefront
					Point2s p((short)j, (short)i);  // current pixel
					curlabel++; // next label
					int count = 0;  // current region size
					ls[j] = curlabel;

					// wavefront propagation
					while (ws >= wbuf) // wavefront not empty
					{
						count++;
						// put neighbors onto wavefront
						T* dpp = &img.at<T>(p.y, p.x); //current pixel value
						T dp = *dpp;
						int* lpp = labels + width * p.y + p.x; //current label value

						//bot
						if (p.y < height - 1 && !lpp[+width] && dpp[+dstep] != newVal && std::abs(dp - dpp[+dstep]) <= maxDiff)
						{
							lpp[+width] = curlabel;
							*ws++ = Point2s(p.x, p.y + 1);
						}
						//top
						if (p.y > 0 && !lpp[-width] && dpp[-dstep] != newVal && std::abs(dp - dpp[-dstep]) <= maxDiff)
						{
							lpp[-width] = curlabel;
							*ws++ = Point2s(p.x, p.y - 1);
						}
						//right
						if (p.x < width - 1 && !lpp[+1] && dpp[+1] != newVal && std::abs(dp - dpp[+1]) <= maxDiff)
						{
							lpp[+1] = curlabel;
							*ws++ = Point2s(p.x + 1, p.y);
						}
						//left
						if (p.x > 0 && !lpp[-1] && dpp[-1] != newVal && std::abs(dp - dpp[-1]) <= maxDiff)
						{
							lpp[-1] = curlabel;
							*ws++ = Point2s(p.x - 1, p.y);
						}


						// pop most recent and propagate
						// NB: could try least recent, maybe better convergence
						p = *--ws;
					}

					// assign label type
					if (count <= maxSpeckleSize)   // speckle region
					{
						rtype[ls[j]] = 1;   // small region label
						ds[j] = (T)newVal;
					}
					else
						rtype[ls[j]] = 0;   // large region label
				}
			}
		}
	}
}


