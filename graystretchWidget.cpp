#include "graystretchwidget.h"
#include "ui_graystretchWidget.h"

GrayStretchWidget::GrayStretchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GrayStretchWidget)
{
    ui->setupUi(this);


    curveChartView= new CurveChartView(ui->widget);

    //初始化表格
    curveChartView->init_curve_parameter(2);//2是灰度拉伸，1是灰度变换，3是二值化
    curveChartView->init_signals_and_slots();
    curveChartView->init_curve_chart_view();
    curveChartView->setVisible(true);
    curveChartView->resize(300,300);


    connect(curveChartView,&CurveChartView::X1StretchChange,this,&GrayStretchWidget::X1stretchChange);
    connect(curveChartView,&CurveChartView::X2StretchChange,this,&GrayStretchWidget::X2stretchChange);

}

GrayStretchWidget::~GrayStretchWidget()
{
    delete ui;
}

void GrayStretchWidget::on_grayStretch_clicked()
{
    image->cvImage=image->originImage.clone();

    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();

    int X1x = ui->X1xAxis->text().toInt();
    int X1y = ui->X1yAxis->text().toInt();
    int X2x = ui->X2xAxis->text().toInt();
    int X2y = ui->X2yAxis->text().toInt();


    //只有灰度图像才能灰度拉伸
    if(channels==1)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(image->originImage.at<uchar>(i, j)>=X2x)//条件根据原图像进行判断，结果在显示图像中更改
                {
                    int x = image->cvImage.at<uchar>(i, j);
                    image->cvImage.at<uchar>(i, j) = (255-X2y)/(255-X2x)*(x-X2x)+X2y;
                }

                else if(image->originImage.at<uchar>(i, j)<X2x&&image->originImage.at<uchar>(i, j)>X1x)
                {
                    int x = image->cvImage.at<uchar>(i, j);
                    image->cvImage.at<uchar>(i, j) = (X2y-X1y)/(X2x-X1x)*(x-X1x)+X1y;
                }
                else
                {
                    int x = image->cvImage.at<uchar>(i, j);
                    image->cvImage.at<uchar>(i, j) = X1y/X1x*x;
                }
                if(image->cvImage.at<uchar>(i, j)>255)
                    image->cvImage.at<uchar>(i, j)=255;
                else if(image->cvImage.at<uchar>(i, j)<0)
                    image->cvImage.at<uchar>(i, j)=0;

            }
        }
        emit imageChange();
    }
}

void GrayStretchWidget::init(ImageProcessingFile *img)
{
    image=img;
}
void GrayStretchWidget::X1stretchChange()
{
    X1x=curveChartView->X1x;
    X1y = curveChartView->X1y;
    ui->X1xAxis->setText(QString::number(X1x));
    ui->X1yAxis->setText(QString::number(X1y));
}
void GrayStretchWidget::X2stretchChange()
{
    X2x=curveChartView->X2x;
    X2y = curveChartView->X2y;
    ui->X2xAxis->setText(QString::number(X2x));
    ui->X2yAxis->setText(QString::number(X2y));
}

void GrayStretchWidget::on_apply_clicked()
{
     image->originImage=image->cvImage.clone();
}

void GrayStretchWidget::on_X1xAxis_editingFinished()
{
    if(ui->X1xAxis->text().toInt()<=1)
        ui->X1xAxis->setText("1");
    else if(ui->X1xAxis->text().toInt()>=254)//最高254
        ui->X1xAxis->setText("254");
    QPointF point={0,0};
    point.setX(ui->X1xAxis->text().toInt());
    point.setY(ui->X1yAxis->text().toInt());
    point = curveChartView->chart()->mapToPosition(point);
    curveChartView->pointIndex=1;
    curveChartView->updateSeriesData(point);
}

void GrayStretchWidget::on_X1yAxis_editingFinished()
{
    if(ui->X1yAxis->text().toInt()<=1)
        ui->X1yAxis->setText("1");
    else if(ui->X1yAxis->text().toInt()>=254)//最高254
        ui->X1yAxis->setText("254");
    QPointF point={0,0};
    point.setX(ui->X1xAxis->text().toInt());
    point.setY(ui->X1yAxis->text().toInt());
    point = curveChartView->chart()->mapToPosition(point);
    curveChartView->pointIndex=1;
    curveChartView->updateSeriesData(point);
}

void GrayStretchWidget::on_X2xAxis_editingFinished()
{
    if(ui->X2xAxis->text().toInt()<=1)
        ui->X2xAxis->setText("1");
    else if(ui->X2xAxis->text().toInt()>=254)//最高254
        ui->X2xAxis->setText("254");
    QPointF point={0,0};
    point.setX(ui->X2xAxis->text().toInt());
    point.setY(ui->X2xAxis->text().toInt());
    point = curveChartView->chart()->mapToPosition(point);
    curveChartView->pointIndex=1;
    curveChartView->updateSeriesData(point);
}

void GrayStretchWidget::on_X2yAxis_editingFinished()
{
    if(ui->X2yAxis->text().toInt()<=1)
        ui->X2yAxis->setText("1");
    else if(ui->X2yAxis->text().toInt()>=254)//最高254
        ui->X2yAxis->setText("254");
    QPointF point={0,0};
    point.setX(ui->X2yAxis->text().toInt());
    point.setY(ui->X2yAxis->text().toInt());
    point = curveChartView->chart()->mapToPosition(point);
    curveChartView->pointIndex=1;
    curveChartView->updateSeriesData(point);
}
