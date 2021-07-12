#include "imageprocessing.h"

ImageProcessingFile::ImageProcessingFile()
{

}




//cv::Mat cvImage;
//QImage qImage;
//QPixmap qPixmap;

bool ImageProcessingFile::load(QString filePath)
{
    //底层读BMP
    //cvImage = cv::imread(filePath.toStdString());
    if(!filePath.isEmpty())
    {
        ReadBmp r(&cvImage,filePath.toStdString());//低层读取BMP图像,必须用指针
        originImage=cvImage.clone();
        qImage=Mat2QImage(cvImage);
        qPixmap=QPixmap::fromImage(qImage);
        height = cvImage.rows;
        width = cvImage.rows;
        fourierStr=new struct FourierStr(height,width);
        RGB2GRAY();
        //cvGray.convertTo(cvGray,CV_32FC1,1/255.0,0);
        //cvFourier = cv::Mat(height,width,CV_32FC1);
        //cv::dft(cvGray,cvFourier);
        //cv::imshow("a",cvFourier);
        //dft();
        return 1;
    }
    else return 0;

//    cvImage = cv::imread(filePath.toStdString());
//    qImage.load(filePath);
//    return qPixmap.load(filePath);
}

void ImageProcessingFile::CanLuminanceChange(int i,int j,int n)
{
    if(cvImage.channels()==3)
    {
        //1通道
        if(cvImage.at<cv::Vec3b>(i,j)[0]+n>255)
        {
            cvImage.at<cv::Vec3b>(i,j)[0]=255;
        }
        else if(cvImage.at<cv::Vec3b>(i,j)[0]+n<0)
        {
            cvImage.at<cv::Vec3b>(i,j)[0]=0;
        }
        else
        {
            cvImage.at<cv::Vec3b>(i,j)[0]+=n;
        }
        //2通道
        if(cvImage.at<cv::Vec3b>(i,j)[1]+n>255)
        {
            cvImage.at<cv::Vec3b>(i,j)[1]=255;
        }
        else if(cvImage.at<cv::Vec3b>(i,j)[1]+n<0)
        {
            cvImage.at<cv::Vec3b>(i,j)[1]=0;
        }
        else
        {
            cvImage.at<cv::Vec3b>(i,j)[1]+=n;
        }
        //3通道
        if(cvImage.at<cv::Vec3b>(i,j)[2]+n>255)
        {
            cvImage.at<cv::Vec3b>(i,j)[2]=255;
        }
        else if(cvImage.at<cv::Vec3b>(i,j)[2]+n<0)
        {
            cvImage.at<cv::Vec3b>(i,j)[2]=0;
        }
        else
        {
            cvImage.at<cv::Vec3b>(i,j)[2]+=n;
        }
    }
    else if(cvImage.channels()==1)
    {
        if(cvImage.at<uchar>(i,j)+n>255)
        {
            cvImage.at<uchar>(i,j)=255;
        }
        else if(cvImage.at<uchar>(i,j)+n<0)
        {
            cvImage.at<uchar>(i,j)=0;
        }
        else
        {
            cvImage.at<uchar>(i,j)+=n;
        }
    }

}



cv::Mat ImageProcessingFile::mean_filter(int kernelSize)
{
    int channels=cvImage.channels();

    int width = cvImage.cols;//列数
    int height = cvImage.rows;//行数
    int pad = floor(kernelSize/2);



    double v =0;

    if(channels==3)
    {
        cv::Mat out = cv::Mat::zeros(height,width,CV_8UC3);
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                for(int k=0;k<channels;k++)
                {
                    v=0;
                    for(int dy = -pad;dy<pad+1;dy++)
                    {
                        for(int dx=-pad;dx<pad+1;dx++)
                        {
                            if(((i+dy)>=0)&&((j+dx)>=0)&&((i+dy<height)&&((j+dx)<width)))
                            {
                                v+=(int)cvImage.at<cv::Vec3b>(i+dy,j+dx)[k];

                            }
                        }
                    }
                    v/=(kernelSize*kernelSize);
                    out.at<cv::Vec3b>(i, j)[k] = (uchar)v;
                }
            }
        }
        return out;
    }
    else if(channels==1)
    {
        cv::Mat out = cv::Mat::zeros(height,width,CV_8UC3);
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                v=0;
                for(int dy = -pad;dy<pad+1;dy++)
                {
                    for(int dx=-pad;dx<pad+1;dx++)
                    {
                        if(((i+dy)>=0)&&((j+dx)>=0)&&((i+dy<height)&&((j+dx)<width)))
                        {
                            v+=(int)cvImage.at<uchar>(i+dy,j+dx);

                        }
                    }
                }
                v/=(kernelSize*kernelSize);
                out.at<uchar>(i, j)= (uchar)v;
            }
        }
        return out;
    }



}

void ImageProcessingFile::RGB2GRAY()
{
    cvGray = cv::Mat::zeros(height,width,CV_8UC1);
    if(cvImage.channels()==1)
        cvGray=cvImage.clone();
    else if(cvImage.channels()==3)
    {

        for (int i =0;i<height;i++)
        {
            for (int j=0;j<width;j++)
            {
                cvGray.at<uchar>(i, j) = 0.2126 * (float)cvImage.at<cv::Vec3b>(i, j)[2]
                                + 0.7152 * (float)cvImage.at<cv::Vec3b>(i, j)[1]
                                + 0.0722 * (float)cvImage.at<cv::Vec3b>(i, j)[0];
            }
        }
    }
}

void ImageProcessingFile::dft()
{
    double i;
    double theta;
    std::complex<double> val;

    for ( int l = 0; l < height; l ++){
      for ( int k = 0; k < width; k ++){
        val.real(0);
        val.imag(0);
        for ( int y = 0; y < height; y ++){
          for ( int x = 0; x < width; x ++){
            i = (double)cvGray.at<uchar>(y, x);
            theta = -2 * M_PI * ((double)k * (double)x / (double)width + (double)l * (double)y / (double)height);
            val += std::complex<double>(cos(theta), sin(theta)) * i;
          }
        }
        val /= sqrt(height * width);
        fourierStr->coef[l][k]=val;
      }
    }
}

void ImageProcessingFile::idft()
{
    double theta;
    double g;
    std::complex<double> G;
    std::complex<double> val;

    for ( int y = 0; y < height; y ++){
      for ( int x = 0; x < width; x ++){
        val.real(0);
        val.imag(0);
        for ( int l = 0; l < height; l ++){
          for ( int k = 0; k < width; k ++){
            G = fourierStr->coef[l][k];
            theta = 2 * M_PI * ((double)k * (double)x / (double)width + (double)l * (double)y / (double)height);
            val += std::complex<double>(cos(theta), sin(theta)) * G;
          }
        }
        g = std::abs(val) / sqrt(height * width);
        g = fmin(fmax(g, 0), 255);
        cvGray.at<uchar>(y, x) = (uchar)g;
      }
    }
}


//QChart* ImageProcessingFile::showHistogram()
//{
//    int channels=cvImage.channels();
//    int data[256][channels];
//    int width = cvImage.cols;//列数
//    int height = cvImage.rows;//行数



//    int yRange = 0;
//    for(int i=0;i<hight;i++)
//    {
//        for(int j=0;j<width;j++)
//        {
//            for(int k=0;k<channels;k++)
//            {
//                int c=cvImage.at<cv::Vec3b>(i,j)[k];
//                data[c][k]++;
//            }
//        }
//    }

//    QBarSet *set = new QBarSet("灰度值");

//    for(int i=0;i<256;i++)
//    {
//        for(int k=0;k<channels;k++)
//        {
//            set->append(data[i][k]);
//            if(yRange<data[i][k])
//                yRange = data[i][k];
//        }

//    }
//    set->setColor(QColor::Rgb);//设置颜色
//    //灰度条宽度
//    QBarSeries *series = new QBarSeries();
//    series->append(set);
//    series->setVisible(true);
//    series->setLabelsVisible(true);

//    //设置横坐标，这里看不懂
//    QStringList categories;
//    for(int i=0;i<256;i++)
//    {
//        categories.push_back("");
//    }
//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->append(categories);

//    //设置纵坐标
//    QValueAxis *axisY =new QValueAxis;
//    axisY->setLabelFormat("%d");
//    axisY->setRange(0,yRange);

//    QChart *chart =new QChart();
//    chart->addSeries(series);
//    chart->createDefaultAxes();
//    chart->setAxisX(axisX);
//    chart->setAxisY(axisY);
//    chart->setTitle("灰度直方图");
//    chart->setAnimationOptions(QChart::SeriesAnimations);
//    chart->legend()->setVisible(false);


//    return chart;
//}

QImage Mat2QImage(cv::Mat& image)
{
    QImage img;

    if (image.channels() == 3) {//三通道rgb
        cvtColor(image, image, cv::COLOR_BGR2RGB);
        img = QImage((const unsigned char*)(image.data), image.cols, image.rows,
            image.cols * image.channels(), QImage::Format_BGR888);//mat里是BGR，这里有个小问题，bmp，mat，qimage的通道是不一样的
    }
    else if (image.channels() == 1) {//单通道灰度图
        img = QImage((const unsigned char*)(image.data), image.cols, image.rows,
            image.cols * image.channels(), QImage::Format_Grayscale8);//图像是使用一个8位灰度格式存储;
    }
    else {
        img = QImage((const unsigned char*)(image.data), image.cols, image.rows,
            image.cols * image.channels(), QImage::Format_BGR888);
    }

    return img;
}
