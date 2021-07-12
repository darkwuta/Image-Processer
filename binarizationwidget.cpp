#include "binarizationwidget.h"
#include "ui_binarizationwidget.h"

BinarizationWidget::BinarizationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinarizationWidget)
{
    ui->setupUi(this);


    curveChartView= new CurveChartView(ui->widget);

    //初始化表格
    curveChartView->init_curve_parameter(3);//2是灰度拉伸，1是灰度变换，3是二值化
    curveChartView->init_signals_and_slots();
    curveChartView->init_curve_chart_view();
    curveChartView->setVisible(true);
    curveChartView->resize(300,300);

    connect(curveChartView,&CurveChartView::ThresholdChange,this,&BinarizationWidget::ThresholdChange);

}


void BinarizationWidget::init(ImageProcessingFile *img)
{
    image = img;
}

BinarizationWidget::~BinarizationWidget()
{
    delete ui;
}

void BinarizationWidget::on_binThreshold_clicked()
{
    image->cvImage=image->originImage.clone();

    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();
    th = ui->Threshold->text().toInt();

    //只有灰度图像才能二值化
    if(channels==1)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(image->originImage.at<uchar>(i, j)>=th)//条件根据原图像进行判断，结果在显示图像中更改
                    image->cvImage.at<uchar>(i, j)=255;
                else
                    image->cvImage.at<uchar>(i, j)=0;
            }
        }
        emit imageChange();
    }

}

void BinarizationWidget::on_Otsu_clicked()
{
    image->cvImage=image->originImage.clone();
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();

    if(channels==1)
    {
        double w0=0, w1=0;
        double m0=0, m1=0;
        double max_sb = 0, sb = 0;
        int th = 0;
        for(int t= 0;t<255;t++)
        {
            w0=0;
            w1=0;
            m0=0;
            m1=0;
            for(int i= 0;i<height;i++)
            {
                for(int j =0;j<width;j++)
                {
                    int val = (int)(image->originImage.at<uchar>(i,j));

                    if(val<t)
                    {
                        w0++;
                        m0+=val;
                    }
                    else
                    {
                        w1++;
                        m1+=val;
                    }
                }
                m0/=w0;
                m1/=w1;

                w0/=(height*width);
                w1/=(height*width);

                sb=w0*w1*pow((m0-m1),2);

                if(sb>max_sb)
                {
                    max_sb=sb;
                    th=t;
                }

            }
        }
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(image->originImage.at<uchar>(i, j)>=th)
                    image->cvImage.at<uchar>(i, j)=255;
                else
                    image->cvImage.at<uchar>(i, j)=0;
            }
        }
        emit imageChange();
    }


}

void BinarizationWidget::on_AdaptiveThreshold_clicked()
{
    image->cvImage=image->originImage.clone();
    int kernelSize = 3;
    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();

    int pad = floor(kernelSize/2);

    double v =0;

    //只有灰度图像才能二值化
    if(channels==1)
    {
        cv::Mat t=cv::Mat::zeros(height,width,CV_8UC1);
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
                            v+=(int)image->originImage.at<uchar>(i+dy,j+dx);

                        }
                    }
                }
                v/=(kernelSize*kernelSize);
                t.at<uchar>(i, j)= (uchar)v;
            }
        }


        //二值化
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(image->originImage.at<uchar>(i, j)>=t.at<uchar>(i,j))
                    image->cvImage.at<uchar>(i, j)=255;
                else
                    image->cvImage.at<uchar>(i, j)=0;
            }
        }

        emit imageChange();
    }

}

void BinarizationWidget::on_Threshold_editingFinished()
{
    QPointF point={0,0};
    point.setX(ui->Threshold->text().toInt());
    point = curveChartView->chart()->mapToPosition(point);
    curveChartView->lineIndex1=1;
    curveChartView->lineIndex2=2;
    curveChartView->updateSeriesData(point);

}


void BinarizationWidget::ThresholdChange()
{
    th = curveChartView->threshold;
    ui->Threshold->setText(QString::number(th));
    on_binThreshold_clicked();
}

void BinarizationWidget::on_ShowOrigin_clicked()
{
    image->cvImage=image->originImage.clone();
    emit imageChange();
}

void BinarizationWidget::on_Apply_clicked()
{
    image->originImage=image->cvImage.clone();
}

void BinarizationWidget::on_halftone_clicked()
{
    image->cvImage=image->originImage.clone();
    int bayer[8][8]={{0,32,8,40,2,34,10,42},
                            {48,16,56,24,50,18,58,26},
                            {12,44,4,36,14,46,6,38},
                            {60,28,52,20,62,30,54,22},
                            {3,35,11,43,1,33,9,41},
                            {51,47,7,39,13,45,5,37},
                            {15,47,7,39,13,45,5,37},
                            {63,31,55,23,61,29,53,21}};


    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();
    if(channels==1)
    {
        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++)
            {
                if(image->cvImage.at<uchar>(y,x)>>2>bayer[y%8][x%8])
                    image->cvImage.at<uchar>(y,x)=255;
                else
                    image->cvImage.at<uchar>(y,x)=0;
            }
        }
    }
    imageChange();

}
