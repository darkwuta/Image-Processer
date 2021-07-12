#include "ReadBmp.h"

ReadBmp::ReadBmp(cv::Mat *cvImage,string filename)
{
    char *bmpName=(char*)filename.data();


    FILE *fp=fopen(bmpName,"rb");//二进制读方式打开指定的图像文件

        if(fp==0)
            return;

        //跳过位图文件头结构BITMAPFILEHEADER

        fseek(fp, sizeof(BITMAPFILEHEADER),0);

        //定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中


        fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); //获取图像宽、高、每像素所占位数等信息

        bmpWidth = head.biWidth;

        bmpHeight = head.biHeight;

        biBitCount = head.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）

        int lineByte=(bmpWidth * biBitCount/8+3)/4*4;//灰度图像有颜色表，且颜色表表项为256

        if(biBitCount==8)
        {

            //申请颜色表所需要的空间，读颜色表进内存

            pColorTable=new RGBQUAD[256];

            for(int i=0;i<256;i++)
            {
                std::cout<<i<<" R:"<<int(pColorTable[i].rgbRed)<<"G:"<<int(pColorTable[i].rgbGreen)<<"B:"<<int(pColorTable[i].rgbBlue)<<endl;
            }


            fread(pColorTable,sizeof(RGBQUAD),256,fp);


            if(pColorTable[10].rgbRed==pColorTable[10].rgbBlue&&pColorTable[10].rgbRed==pColorTable[10].rgbGreen)//灰度图
                *cvImage = cv::Mat::zeros(bmpHeight,bmpWidth,CV_8UC1);
            else //伪彩色
                *cvImage = cv::Mat::zeros(bmpHeight,bmpWidth,CV_8UC3);
        }
        else if(biBitCount==24)
        {
            *cvImage = cv::Mat::zeros(bmpHeight,bmpWidth,CV_8UC3);
        }

        //申请位图数据所需要的空间，读位图数据进内存

        pBmpBuf=new unsigned char[lineByte * bmpHeight];

        int a=sizeof(pBmpBuf);

        fread(pBmpBuf,1,lineByte * bmpHeight,fp);

//        for(int i=0;i<lineByte;i++)
//            for(int j=0;j<bmpHeight;j++)
//                qDebug()<<pBmpBuf[i][j];

        if(biBitCount==8)
        {
            //8bit只有一个通道
            unsigned int **out;
            out = new unsigned int *[bmpHeight];        //开辟指针数组
            for (int i = 0; i<bmpHeight; i++)
                out[i] = new unsigned int[bmpWidth];
            //初始化数组


            for (int i = 0; i<bmpHeight; i++)
            {
                for (int j = 0; j<bmpWidth; j++)
                {
                    out[i][j] = pBmpBuf[j + lineByte*i];//每行像素必须是4的倍数，这里读取宽度不是4的倍数的图片时会出问题，所以每行要用lineByte而不是cols
                }
            }


            //导入cvImage中
            if(pColorTable[10].rgbRed==pColorTable[10].rgbBlue&&pColorTable[10].rgbRed==pColorTable[10].rgbGreen)//灰度图
            {
                for (int i = 0; i<bmpHeight; i++)
                {
                    for (int j = 0; j<bmpWidth; j++)
                    {
                        cvImage->at<uchar>(bmpHeight - 1 - i, j) = out[i][j];//bmp中的像素是从下到上的

                    }
                }

            }

            else//伪彩色
            {
                for (int i = 0; i<bmpHeight; i++)
                {
                    for (int j = 0; j<bmpWidth; j++)
                    {
                        cvImage->at<cv::Vec3b>(bmpHeight - 1 - i, j)[0]= pColorTable[out[i][j]].rgbBlue;
                        cvImage->at<cv::Vec3b>(bmpHeight - 1 - i, j)[1]= pColorTable[out[i][j]].rgbGreen;
                        cvImage->at<cv::Vec3b>(bmpHeight - 1 - i, j)[2]= pColorTable[out[i][j]].rgbRed;
                    }
                }
            }




        }


        else if(biBitCount==24)
        {
            unsigned int **out_r;
            unsigned int **out_g;
            unsigned int **out_b;


            out_r = new unsigned int *[bmpHeight];        //开辟指针数组
            for (int i = 0; i<bmpHeight; i++)
                out_r[i] = new unsigned int[bmpWidth];

            out_g = new unsigned int *[bmpHeight];        //开辟指针数组
            for (int i = 0; i<bmpHeight; i++)
                out_g[i] = new unsigned int[bmpWidth];

            out_b = new unsigned int *[bmpHeight];        //开辟指针数组
            for (int i = 0; i<bmpHeight; i++)
                out_b[i] = new unsigned int[bmpWidth];


            for (int i = 0; i<bmpHeight; i++)
            {
                for (int j = 0; j<bmpWidth; j++)
                {
                    out_r[bmpHeight - 1 - i][j] = pBmpBuf[j * 3 + 2 +  lineByte*i];
                    out_g[bmpHeight - 1 - i][j] = pBmpBuf[j * 3 + 1 +  lineByte *i];
                    out_b[bmpHeight - 1 - i][j] = pBmpBuf[j * 3 +  lineByte *i];
                }
            }

            for (int i = 0; i<bmpHeight; i++){
                for (int j = 0; j<bmpWidth; j++){
                    cvImage->at<cv::Vec3b>(i, j)[0] = out_r[i][j];
                    cvImage->at<cv::Vec3b>(i, j)[1]= out_g[i][j];
                    cvImage->at<cv::Vec3b>(i, j)[2]= out_b[i][j];
                }
            }

            //qDebug()<<cvImage->channels();
            //cv::namedWindow("lunpan");
            //cv::imshow("lunpan",cvImage);
            //cv::imshow("a",cvImage);

        }


        fclose(fp);//关闭文件
        //读取文件成功
}
