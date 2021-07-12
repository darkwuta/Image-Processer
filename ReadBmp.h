#ifndef READBMP_H
#define READBMP_H

#include<iostream>
#include<windows.h>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<iomanip>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <QMainWindow>

#include <QDebug>

using namespace std;

class ReadBmp
{
public:
    ReadBmp(cv::Mat *cvImage,string filename);



private:
    unsigned char *pBmpBuf;//读入图像数据的指针

    BITMAPINFOHEADER head;//bmp头目录

    int bmpWidth;//图像的宽
    int bmpHeight;//图像的高
    RGBQUAD *pColorTable;//颜色表指针

    int biBitCount;//图像类型，每像素位数

};

#endif // READBMP_H
