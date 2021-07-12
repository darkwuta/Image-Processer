#include "graychangewidget.h"
#include "ui_graychangewidget.h"

GrayChangeWidget::GrayChangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GrayChangeWidget)
{
    ui->setupUi(this);

    curveChartView= new CurveChartView(ui->widget);

    //初始化表格
    curveChartView->init_curve_parameter(1);//2是灰度拉伸，1是灰度变换，3是二值化
    curveChartView->init_signals_and_slots();
    curveChartView->init_curve_chart_view();
    curveChartView->setVisible(true);
    curveChartView->resize(300,300);

    connect(curveChartView,&CurveChartView::UpperLimitChange,this,&GrayChangeWidget::upperLimitChange);
    connect(curveChartView,&CurveChartView::LowerLimitChange,this,&GrayChangeWidget::lowerLimitChange);

}

void GrayChangeWidget::init(ImageProcessingFile *img)
{
    image = img;
}

GrayChangeWidget::~GrayChangeWidget()
{
    delete ui;
}

void GrayChangeWidget::on_upperLimit_editingFinished()
{
    if(ui->upperLimit->text().toInt()>=255)
        ui->upperLimit->setText("255");
    QPointF point={0,0};
    point.setX(ui->upperLimit->text().toInt());
    point.setY(ui->upperLimit->text().toInt());
    point = curveChartView->chart()->mapToPosition(point);
    curveChartView->pointIndex=3;
    curveChartView->updateSeriesData(point);
}

void GrayChangeWidget::on_lowerLimit_editingFinished()
{
    if(ui->lowerLimit->text().toInt()<=0)
        ui->lowerLimit->setText("0");
    QPointF point={0,0};
    point.setX(ui->lowerLimit->text().toInt());
    point.setY(ui->lowerLimit->text().toInt());
    point = curveChartView->chart()->mapToPosition(point);
    curveChartView->pointIndex=2;
    curveChartView->updateSeriesData(point);
}

void GrayChangeWidget::on_grayChange_clicked()
{
    image->cvImage=image->originImage.clone();

    int height=image->originImage.rows;
    int width = image->originImage.cols;
    int channels = image->originImage.channels();

    int up_limit = ui->upperLimit->text().toInt();
    int low_limit = ui->lowerLimit->text().toInt();

    //只有灰度图像才能二值化
    if(channels==1)
    {
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(image->originImage.at<uchar>(i, j)>=up_limit)//条件根据原图像进行判断，结果在显示图像中更改
                    image->cvImage.at<uchar>(i, j)=255;
                else if(image->originImage.at<uchar>(i, j)<low_limit)
                    image->cvImage.at<uchar>(i, j)=0;
            }
        }
        emit imageChange();
    }
}

void GrayChangeWidget::upperLimitChange()
{
    up = curveChartView->upperLimit;
    ui->upperLimit->setText(QString::number(up));
}
void GrayChangeWidget::lowerLimitChange()
{
    low = curveChartView->lowerLimit;
    ui->lowerLimit->setText(QString::number(low));
}

void GrayChangeWidget::on_apply_clicked()
{
    image->originImage=image->cvImage.clone();
}
