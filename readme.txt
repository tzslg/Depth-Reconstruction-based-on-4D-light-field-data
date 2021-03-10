#include"pch.h"
#include <iostream>
#include <opencv2\opencv.hpp>

/*
filterSpeckles�����Ӳ�ͼ����
��������ƥ���㷨�������ʼ�Ӳ�ͼ�������Ӳ�ͼ����������ֵ�˲�����ͨ����ȡ�
������ֵ�˲��ܹ���Чȥ���Ӳ�ͼ�й�������㣬����ͨ�����ܹ������Ӳ�ͼ������������С�ſ�(blob)��
speckleWindowSize��ָ���ü�������ͨ�������ص������Ҳ������ͨ��Ĵ�С��
speckleRange��ָ�����ж��������Ƿ�����ͬһ����ͨ�����ֵ������
����������£�
�жϵ�ǰ���ص��������������뵱ǰ���ص�Ĳ�ֵdiff��
���diff < speckRange�����ʾ��������뵱ǰ���ص���һ����ͨ������һ����ǡ�
Ȼ�����Ը������Ϊ�����ж���������㣬����ͬ�ϡ�
ֱ��ĳһ���ص�������ĵ����������������ֹͣ��
����1��ɺ��жϱ���ǵ����ص����count��
������ص����count <= speckleWindowSize��
��˵������ͨ����һ��С�ſ�(blob)���򽫵�ǰ���ص�ֵ����ΪnewValue(��ʾ������Ӳ�ֵ��newValueһ������Ϊ��������0ֵ)��
���򣬱�ʾ����ͨ���Ǹ����ſ飬��������
ͬʱ�������ֵ���Ƿ�ΪС�ſ�Ĺ�ϵ��rtype[label]��
rtype[label]Ϊ0����ʾlabelֵ��Ӧ�����ص�����С�ſ飬Ϊ1������С�ſ顣
������һ�����ص�ʱ�����ж����Ƿ��Ѿ�����ǣ�
����Ѿ�����ǣ�����ݹ�ϵ��rtype[label]�ж��Ƿ�ΪС�ſ飨blob����
����ǣ���ֱ�ӽ�������ֵ����ΪnewValue��������ǣ�������������������һ�����ء�
���û�б���ǣ����ղ���1����
�������ص㴦�����������������ᱻ����ΪnewValueֵ�����������ÿն����ȷ������¹������Ӳ�ֵ��
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


