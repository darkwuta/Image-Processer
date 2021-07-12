#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include"ReadBmp.h"

#include <QMainWindow>
//灰度条
//#include <QBarSet>
//#include <QBarSeries>
#include <QtCharts>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//struct fourier_str {
//  std::complex<double> coef[height][width];
//};
typedef struct FourierStr{
  std::complex<double> **coef;
  FourierStr(int height,int width)
  {
      coef=new std::complex<double>*[height];
      for(int i=0;i<height;i++)
          coef[i]=new std::complex<double>[width];
  }
}FourierStr;

class ImageProcessingFile
{
public:
    ImageProcessingFile();
    cv::Mat cvImage;
    cv::Mat cvGray;
    cv::Mat cvFourier;
    QImage qImage;
    QPixmap qPixmap;
    cv::Mat originImage;


    void CanLuminanceChange(int i,int j,int n);

    QChart* showHistogram();
    bool load(QString filePath);

    cv::Mat mean_filter(int kernelSize);

     FourierStr *fourierStr;

    void RGB2GRAY();
    void dft();//离散傅里叶变换
    void idft();//离散傅里叶逆变换

private:
    int width;
    int height;



};


QImage Mat2QImage(cv::Mat& image);


#endif // IMAGEPROCESSING_H
