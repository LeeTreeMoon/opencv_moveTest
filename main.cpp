#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/types_c.h"
#include <iostream>

using namespace cv;
using namespace std;

#define BACKGROUND_NUM 10

const int alphList[BACKGROUND_NUM] = {1,1,2,2,3,3,4,4,40,40};

static void creatWindow(const char *name, int x = 0, int y = 0, int width = 320, int height = 240);
static Mat creatBackGround(Mat *imageList, int num);
static void updateQueue(Mat *ImageList, Mat newImage, int len);
static void computerRectangle(Mat &image, Point &p1, Point &p2);

int main(void)
{
	VideoCapture capture(0);   //打开摄像头
	capture.set(CAP_PROP_FRAME_WIDTH, 320);
	capture.set(CAP_PROP_FRAME_HEIGHT, 240);
	creatWindow("捕捉窗口");

    //读取25帧图像用于计算背景模型
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
//	imshow("捕捉窗口", backGround);

	Mat frame;
	Mat grayFrame;
	Mat diff;
	Point p1 = Point(20, 20);
	Point p2 = Point(200, 200);
	while(1)
	{
		capture >>frame;
		if(frame.empty())	
			return -1;
		blur(frame, frame, Size(3, 3), Point(-1, -1));   //均值滤波
		cvtColor(frame, grayFrame, CV_BGR2GRAY);  //转换成灰度图像
		diff = grayFrame - backGround;
		threshold(diff, diff, 10, 255, THRESH_BINARY);  //对差值图像进行二值化
		computerRectangle(diff, p1, p2);                //计算检测矩形
		rectangle( frame, p1, p2, Scalar(0,0,255), 3, 1 ,0 );  //将矩形添加进图像中
		updateQueue(backGroundList, grayFrame, BACKGROUND_NUM);
		backGround = creatBackGround(backGroundList, BACKGROUND_NUM);
		imshow("捕捉窗口", frame);
		imshow("差值窗口", diff);
		waitKey(1);
	}
	delete []backGroundList;
	return 0;
}

/*
 * 函数名称：
 * 函数功能：
 * 输入参数：
 * 输出参数：
 */
static void creatWindow(const char *name, int x, int y, int width, int height)
{
	namedWindow(name, 0);
	moveWindow(name, x, y);
	resizeWindow(name, width, height);
}

/*
 * 函数名称：
 * 函数功能：
 * 输入参数：
 * 输出参数：
 */
static Mat creatBackGround(Mat *imageList, int num)
{   
	Mat _backGround = Mat::zeros(imageList[0].size(), CV_8U);
	for (int x = 0; x < imageList[0].rows; x++)
	{
		for (int y = 0; y < imageList[0].cols; y++)
		{
			int pixel_sum = 0;
			for (int n = 0; n < num; n++)   //计算25帧对应像素的总值
			{
				pixel_sum += imageList[n].at<uchar>(x, y) * alphList[n];  //乘以权重
			}
			_backGround.at<uchar>(x, y) = pixel_sum / 100;
		}
	}
	return _backGround;
}

/*
 * 函数名称：
 * 函数功能：
 * 输入参数：
 * 输出参数：
 */
static void updateQueue(Mat *ImageList, Mat newImage, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		ImageList[i] = ImageList[i+1];
	}
	ImageList[len-1] = newImage;
}

/*
 * 函数名称：
 * 函数功能：
 * 输入参数：
 * 输出参数：
 */
static void computerRectangle(Mat &image, Point &p1, Point &p2)
{
	p1 = Point(0, 0);
	p2 = Point(0, 0);
	int *x_shadowArray = new int[image.cols];  //存放水平投影序列
	int *y_shadowArray = new int[image.rows];  //存放垂直投影序列
	memset(x_shadowArray, 0, image.cols);
	memset(y_shadowArray, 0, image.rows);

	for (int x = 0; x < image.cols; x++)    //进行水平垂直投影
	{
		for (int y = 0; y < image.rows; y++)
		{
			if ( image.at<uchar>(x, y) != 0 )
			{
				x_shadowArray[x] += 1; 
				y_shadowArray[y] += 1;
			}
		}
	}

    /* 寻找最左x坐标 */
	for (int x = 0; x < image.cols - 1; x++)    
	{
		if ( x_shadowArray[x] == 0 && x_shadowArray[x+1] != 0 )
		{
			p1.x = x+1;
			break;
		}
	}

    /* 寻找最右x坐标 */
	for (int x = image.cols - 1; x > 0; x--)
	{
		if ( x_shadowArray[x] == 0 && x_shadowArray[x-1] != 0 )
		{
			p2.x = x-1;
			break;
		}
	}

    /* 寻找最上y坐标 */
	for (int y = 0; y < image.rows - 1; y++)
	{
		if ( y_shadowArray[y] == 0 && y_shadowArray[y-1] != 0 )
		{
			p1.y = y-1;
			break;
		}
	}

	/* 寻找最右x坐标 */
	for (int y = image.rows - 1; y > 0; y--)
	{
		if ( y_shadowArray[y] == 0 && y_shadowArray[y-1] != 0 )
		{
			p2.y = y-1;
			break;
		}
	}	
	delete []x_shadowArray;
	delete []y_shadowArray;
}
