#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/types_c.h"
#include <iostream>

using namespace cv;
using namespace std;

#define BACKGROUND_NUM 25
/*
 * 函数名称：
 * 函数功能：
 * 输入参数：
 * 输出参数：
 */
static void creatWindow(const char *name, int x = 0, int y = 0, int height = 640, int width = 480)
{
	namedWindow(name, 0);
	moveWindow(name, x, y);
	resizeWindow(name, height, width);
}

static Mat creatBackGround(Mat imageList[], int num)
{
	int pixel_sum = 0;
	Mat _backGround;
	for (int x = 0; x < imageList[0].rows; x++)
	{
		for (int y = 0; y < imageList[0].cols; y++)
		{
			for (int n = 0; n < num; n++)
			{
				pixel_sum += imageList[n].at<uchar>(x, y) * 0.25 + 1;
			}
			_backGround.at<uchar>(x, y) = pixel_sum/100;
		}
	}
	return _backGround;
}

int main()
{
	VideoCapture capture(0);   //打开摄像头
	creatWindow("捕捉窗口");

    //读取24帧图像用于计算背景模型
	Mat *backGroundList = new Mat[BACKGROUND_NUM];
	for (int i = 0; i < BACKGROUND_NUM; i++)
	{
		Mat frame;
		capture >>frame;
		if(frame.empty())	
			return -1;
		cvtColor(frame, backGroundList[i], CV_BGR2GRAY);
	}

	Mat backGround = creatBackGround(backGroundList, BACKGROUND_NUM);
	imshow("背景模型", backGround);
	waitKey(1);

/*	while (1)
	{
		Mat frame;
		capture >> frame;
		if(frame.empty())
			break;
		imshow("捕捉窗口", frame);
		waitKey(1);
	}*/
	delete []backGroundList;
	return 0;
}
