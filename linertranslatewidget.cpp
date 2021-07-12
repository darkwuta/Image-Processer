#include "linertranslatewidget.h"
#include "ui_linertranslatewidget.h"

LinerTranslateWidget::LinerTranslateWidget(FunctionalWidget *parent) :
    FunctionalWidget(parent),
    ui(new Ui::LinerTranslateWidget)
{
    ui->setupUi(this);

    //delete curveChartView;
    curveChartView= new CurveChartView(ui->widget);

    //初始化表格
    curveChartView->init_curve_parameter(4);//2是灰度拉伸，1是灰度变换，3是二值化，4是线性变换
    curveChartView->init_signals_and_slots();
    curveChartView->init_curve_chart_view();
    curveChartView->setVisible(true);
    curveChartView->resize(300,300);


    connect(curveChartView,&CurveChartView::FuncChange,this,&LinerTranslateWidget::FuncChange);
    //FuncChange();

}

LinerTranslateWidget::~LinerTranslateWidget()
{
    delete ui;
}

void  LinerTranslateWidget::FuncChange()
{
    for(int i=0;i<256;i++)
    {
        if(curveChartView->func[i]>255)
            func[i]=255;
        else if(curveChartView->func[i]<0)
            func[i]=0;
        else
            func[i]=curveChartView->func[i];
    }
    k=curveChartView->k;
    b=curveChartView->b;
    ui->slope->setText(QString::number(k));
    ui->intercept->setText(QString::number(b));
    on_linerTranslate_clicked();
}

void LinerTranslateWidget::on_linerTranslate_clicked()
{
    image->cvImage=image->originImage.clone();

    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();

    //只有灰度图像才能灰度拉伸
    if(channels==1)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                image->cvImage.at<uchar>(i,j) = func[(int)image->originImage.at<uchar>(i,j)];
            }
        }
        emit imageChange();
    }
    else if(channels ==3)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                for(int c =0;c<channels;c++)
                {
                     image->cvImage.at<cv::Vec3b>(i,j)[c] = func[(int)image->originImage.at<cv::Vec3b>(i,j)[c]];
                }

            }
        }
        emit imageChange();
    }
}

void LinerTranslateWidget::on_slope_editingFinished()
{

//    QPointF point1={0,0};
//    QPointF point2={0,0};
//    //求函数和边界的交点
//    int x;
//    int y;
//    //x=0
//    x=0;
//    y=k*x+b;
//    if(y>=0&&y<=255)
//    {

//    }
//    //x=255
//    x=255;
//    y=k*x+b;
//    if(y>=0&&y<=255)
//    {

//    }


//    int x1= 0;
//    int y1 = k*x1+b;
//    int x2=255;
//    int y2=k*x2+b;

//    point1.setX(x1);
//    point1.setY(y1);
//    point2.setX(x2);
//    point2.setY(y2);
//    point1 = curveChartView->chart()->mapToPosition(point1);
//    curveChartView->pointIndex=1;
//    curveChartView->updateSeriesData(point1);

//    point2 = curveChartView->chart()->mapToPosition(point2);
//    curveChartView->pointIndex=2;
//    curveChartView->updateSeriesData(point2);
}
