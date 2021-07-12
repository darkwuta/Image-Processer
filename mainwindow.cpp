#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Mat转QImage的代码
//    Mat image=imread("D:/code/qt/DigitalImageProcessing/imori.jpg",1);//一定要使用绝对路径，其他可以回报错
//    ///opencv显示
//    //namedWindow("Display window", WINDOW_AUTOSIZE );
//    //imshow("Display window", image );

//    QImage QImg = Mat2QImage(image);
//        //Mat转QImage
//    ui->label->setScaledContents(true);
//    ui->label->setPixmap(QPixmap::fromImage(QImg));
//    //ui->label->setAutoFillBackground(true);



    //设置可以拖拽
    setAcceptDrops(true);
    ui->graphicsView->setAcceptDrops(false);//设置QGraphicsView不接受拖放事件
    ui->graphicsView->setScene(&scene);//设置场景



    //设置所有按钮不能点击，读取图像后可以点击
    ui->binarization->setEnabled(false);
    ui->grayChange->setEnabled(false);



    //ui->label->resize(ui->label->pixmap()->size());

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)//拖动图片
{
    //拖拽事件
        QStringList FileTypes;
        FileTypes.append("jpg");
        FileTypes.append("png");
        FileTypes.append("bmp");

        if(event->mimeData()->hasUrls()&&event->mimeData()->urls().count()==1)
        {//对象是否可以返回URL列表，并且只有一个
            QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
            if(FileTypes.contains(file.suffix().toLower())){//在FileTypes查找文件后缀是否符合
                event->acceptProposedAction();
            }
        }
}

void MainWindow::dropEvent(QDropEvent *event)//显示图片
{
    //重置ui
    ui->binarization->setEnabled(false);
    ui->grayChange->setEnabled(false);
    //丢弃事件
    ui->graphicsView->resetTransform();//重置变换
    QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());//获取文件信息



    if(image.load(file.absoluteFilePath()))
    {//加载文件
        scene.clear();
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(image.qPixmap);//QGraphicsItem子类之一
        item->setFlags(QGraphicsItem::ItemIsMovable);//支持鼠标拖动
        scene.addItem(item);
        ui->graphicsView->fitInView(item,Qt::KeepAspectRatio);//缩放矩阵以适应View
        //显示图像信息
        ui->Width->setText("图像宽度："+image.qImage.width());
        ui->Height->setText("图像宽度："+image.qImage.height());


    }

    //设置ui信息
    ui->xAxis->setMaximum(image.cvImage.cols);
    ui->xAxis->setMinimum(-image.cvImage.cols);

    ui->yAxis->setMaximum(image.cvImage.rows);
    ui->yAxis->setMinimum(-image.cvImage.rows);

    //设置只有灰度图像才能点击的按钮
    if(image.cvImage.channels()==1)
    {
        ui->binarization->setEnabled(true);
        ui->grayChange->setEnabled(true);
    }
}
void MainWindow::resizeEvent(QResizeEvent *event)//重写界面
{
    //窗口大小变化事件
    Q_UNUSED(event);
    if(scene.items().count()!=0){
        QGraphicsPixmapItem* item = dynamic_cast<QGraphicsPixmapItem*>(scene.items().value(0));
        ui->graphicsView->fitInView(item,Qt::KeepAspectRatio);
    }

}



void MainWindow::on_readImage_clicked()
{
    //重置ui
    ui->binarization->setEnabled(false);
    ui->grayChange->setEnabled(false);
    QString fileName =QFileDialog::getOpenFileName(this, tr("open file"), "", tr("Allfile(*.*);"));
    if(image.load(fileName))
    {//加载文件
        scene.clear();
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(image.qPixmap);//QGraphicsItem子类之一
        item->setFlags(QGraphicsItem::ItemIsMovable);//支持鼠标拖动
        scene.addItem(item);
        ui->graphicsView->fitInView(item,Qt::KeepAspectRatio);//缩放矩阵以适应View

        //显示图像信息
        ui->Width->setText("图像宽度："+QString::number(image.qImage.width()));
        ui->Height->setText("图像高度："+QString::number(image.qImage.height()));
    }

    //设置ui信息
    ui->xAxis->setMaximum(image.cvImage.cols);
    ui->xAxis->setMinimum(-image.cvImage.cols);

    ui->yAxis->setMaximum(image.cvImage.rows);
    ui->yAxis->setMinimum(-image.cvImage.rows);


    //设置只有灰度图像才能点击的按钮
    if(image.cvImage.channels()==1)
    {
        ui->binarization->setEnabled(true);
        ui->grayChange->setEnabled(true);
    }
    //else if()

}

void MainWindow::on_writeImage_clicked()
{
    QString fileName =QFileDialog::getSaveFileName(this, tr("save file"), "", tr("Allfile(*.*);"));

    if(!fileName.isEmpty())
        cv::imwrite(fileName.toStdString(),image.cvImage);

}

void MainWindow::on_ImageInfo_clicked()
{
    int channels=image.cvImage.channels();
    int width = image.cvImage.cols;//列数
    int height = image.cvImage.rows;//行数
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            for(int k=0;k<channels;k++)
            {
                std::cout<<int(image.cvImage.at<cv::Vec3b>(i,j)[k])<<" ";
            }
            std::cout<<"/";
        }
        std::cout<<endl;
    }
}


void MainWindow::GetHistogram()
{
    int channels=image.cvImage.channels();
    int width = image.cvImage.cols;//列数
    int height = image.cvImage.rows;//行数

    histogram = new QChart;

    if(channels==3)//三通道的
    {
        int data[256][3];
        //直方图初始化
        for(int i=0;i<256;i++)
            for(int j=0;j<3;j++)
                data[i][j]=0;
        //获得直方图
        int yRange = 0;
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                for(int k=0;k<3;k++)
                {
                    int c=image.cvImage.at<cv::Vec3b>(i,j)[k];
                    data[c][k]++;
                }
            }
        }

        //归一化
//        int s = width*height;
//        int t=50;//标准化因子
//        for(int i=0;i<256;i++)
//        {
//            for(int k=0;k<3;k++)
//                data[i][k]=int(float(data[i][k])/float(s)*t);
//        }

        //三个柱状图
        QBarSet *red = new QBarSet("R");
        QBarSet *green = new QBarSet("G");
        QBarSet *blue = new QBarSet("B");
        //设置柱状图颜色
        red->setLabelColor(QColor(255,0,0));//红色
        green->setLabelColor(QColor(0,255,0));//红色
        blue->setLabelColor(QColor(0,0,255));//红色

        //设置3个柱状图的数值
        for(int i=0;i<256;i++)
        {
            int k = 0;
            red->append(data[i][0]);
            if(yRange<data[i][k])
                yRange = data[i][k];
        }
        for(int i=0;i<256;i++)
        {
            int k = 0;
            green->append(data[i][1]);
            if(yRange<data[i][k])
                yRange = data[i][k];
        }
        for(int i=0;i<256;i++)
        {
            int k = 0;
            blue->append(data[i][2]);
            if(yRange<data[i][k])
                yRange = data[i][k];
        }

        //显示柱状图
        QBarSeries *series = new QBarSeries();
        series->append(red);
        series->append(green);
        series->append(blue);
        series->setBarWidth(1.2);

        //设置横坐标，这里看不懂
        QStringList categories;
        for(int i=0;i<256;i++)
        {
            categories.push_back("");
        }
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);

        //设置纵坐标
        QValueAxis *axisY =new QValueAxis;
        axisY->setLabelFormat("%d");
        axisY->setRange(0,yRange);

        histogram->addSeries(series);
        histogram->createDefaultAxes();
        histogram->setAxisX(axisX);
        histogram->setAxisY(axisY);
        histogram->setTitle("灰度直方图");
        histogram->setAnimationOptions(QChart::SeriesAnimations);
        histogram->legend()->setVisible(false);
        return;
    }
    else if(channels==1)//灰度图
    {
        int data[256];
        //直方图初始化
        for(int i=0;i<256;i++)
            data[i]=0;
        //获得直方图
        int yRange = 0;
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                int c=image.cvImage.at<uchar>(i,j);
                data[c]++;
            }
        }
        //柱状图
        QBarSet *gray = new QBarSet("灰度");

        //设置柱状图颜色
        gray->setLabelColor(QColor(0,0,0));//红色

//        //归一化
//        int s = width*height;
//        float t=50.;//标准化因子
//        for(int i=0;i<256;i++)
//        {
//            data[i]=int(float(data[i])/float(s)*t);
//        }

        //设置3个柱状图的数值
        for(int i=0;i<256;i++)
        {
            int k = 0;
            gray->append(data[i]);
            if(yRange<data[i])
                yRange = data[i];
        }



        //显示柱状图
        QBarSeries *series = new QBarSeries();
        series->append(gray);
        series->setBarWidth(1.0);

        //设置横坐标，这里看不懂
        QStringList categories;
        for(int i=0;i<256;i++)
        {
            categories.push_back("");
        }
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);

        //设置纵坐标
        QValueAxis *axisY =new QValueAxis;
        axisY->setLabelFormat("%d");
        axisY->setRange(0,yRange);

        histogram->addSeries(series);
        histogram->createDefaultAxes();
        histogram->setAxisX(axisX);
        histogram->setAxisY(axisY);
        histogram->setTitle("灰度直方图");
        histogram->setAnimationOptions(QChart::SeriesAnimations);
        histogram->legend()->setVisible(false);
        return;
    }

}

void MainWindow::on_Histogram_clicked()
{
    //QWidget histogram;
    //histogram

    chartview = new QChartView;
    GetHistogram();
    Histogram = new QWidget();
    chartview->setParent(Histogram);
    chartview->setChart(histogram);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setVisible(true);
    QPushButton *equalization=new QPushButton(Histogram);
    equalization->setText("灰度均衡化");

    connect(equalization,SIGNAL(clicked()),this,SLOT(on_equalization_clicked()));

    Histogram->show();
    //histogram.setVisible(chartview);
    //-+histogram.show();

}

//cv::Mat cvImage;
//QImage qImage;
//QPixmap qPixmap;


void MainWindow::on_Inversion_clicked()
{

    //类更改
    for(int i=0;i<image.cvImage.rows;i++)
    {
        for(int j=0;j<image.cvImage.cols;j++)
        {
            image.cvImage.at<cv::Vec3b>(i,j)[0]=255-image.cvImage.at<cv::Vec3b>(i,j)[0];
            image.cvImage.at<cv::Vec3b>(i,j)[1]=255-image.cvImage.at<cv::Vec3b>(i,j)[1];
            image.cvImage.at<cv::Vec3b>(i,j)[2]=255-image.cvImage.at<cv::Vec3b>(i,j)[2];
        }
    }

    ImageChange();
}

void MainWindow::on_graying_clicked()
{
    int height=image.cvImage.rows;
    int width = image.cvImage.cols;
    cv::Mat gray = cv::Mat::zeros(height,width,CV_8UC1);
    if(image.cvImage.channels()==1)
        return;
    else if(image.cvImage.channels()==3)
    {

        for (int i =0;i<height;i++)
        {
            for (int j=0;j<width;j++)
            {
                gray.at<uchar>(i, j) = 0.2126 * (float)image.cvImage.at<cv::Vec3b>(i, j)[2]
                                + 0.7152 * (float)image.cvImage.at<cv::Vec3b>(i, j)[1]
                                + 0.0722 * (float)image.cvImage.at<cv::Vec3b>(i, j)[0];


            }
        }
        image.cvImage=gray;
//        cv::namedWindow("lunpan");
//        cv::imshow("lunpan",image.cvImage);
    }

    ImageChange();

}

void MainWindow::on_pseudoColor_clicked()
{
    //只有bmp能够转化
//    if(image.cvImage.channels()==3)//只有真彩色才能转化
//    {
//        int height=image.cvImage.rows;
//        int width = image.cvImage.cols;
//        int min=0;
//        for(int i=0;i<height;i++)
//        {
//            for(int j=0;j<width;j++)
//            {

//            }
//        }
//    }
//    else
//        return;
}

void MainWindow::on_brightnessChange_sliderMoved(int position)
{

    image.cvImage=image.originImage.clone();
    int height=image.cvImage.rows;
    int width = image.cvImage.cols;

    for (int i =0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            image.CanLuminanceChange(i,j,position);
        }
    }

    ImageChange();

}

void MainWindow::on_normalization_clicked()
{
    int channels = image.cvImage.channels();
    int height=image.cvImage.rows;
    int width = image.cvImage.cols;




    for (int i =0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            for(int k=0;k<channels;k++)
            {

            }
        }
    }

}

void MainWindow::on_colorTable_clicked()
{

}

void MainWindow::on_graying_2_clicked()
{



}

void MainWindow::on_binarization_clicked()
{

    binarizationWidget = new BinarizationWidget();

    binarizationWidget->init(&image);
    connect(binarizationWidget,&BinarizationWidget::imageChange,this,&MainWindow::ImageChange);//连接成功，一定要写到报告
    binarizationWidget->show();


}


void MainWindow::ImageChange()
{
    image.qImage=Mat2QImage(image.cvImage);
    image.qPixmap=QPixmap::fromImage(image.qImage);

    //重新显示
    scene.clear();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(image.qPixmap);//QGraphicsItem子类之一
    item->setFlags(QGraphicsItem::ItemIsMovable);//支持鼠标拖动
    scene.addItem(item);
}

void MainWindow::on_equalization_clicked()
{
    image.cvImage=image.originImage.clone();
    int height=image.cvImage.rows;
    int width = image.cvImage.cols;
    int channels = image.cvImage.channels();

    double sum=double(height*width);

    //灰度概率
    double gray_p[256];
    double gray_e[256];//均衡化后概率

    for(int i=0;i<256;i++)
        gray_p[i]=0;

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            int index=image.cvImage.at<uchar>(i, j);
            gray_p[index]+=1.0;
        }
    }
    for(int i=0;i<256;i++)
        gray_p[i]/=sum;
    for(int i=0;i<256;i++)
    {
        double t=0;
        for(int j =0;j<i; j++)
        {
            t+=gray_p[j];
        }
        gray_e[i]=t;
    }


    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            int index=image.cvImage.at<uchar>(i, j);
            image.cvImage.at<uchar>(i, j)=255*gray_e[index];
        }
    }
    Histogram->close();
    on_Histogram_clicked();
    ImageChange();


}


void MainWindow::on_grayStretch_clicked()
{



//    QWidget *CureveChart = new QWidget();
//    curveChartView= new CurveChartView(CureveChart);

//    curveChartView->init_curve_parameter(2);//2是灰度拉伸，1是灰度变换
//    curveChartView->init_signals_and_slots();
//    curveChartView->init_curve_chart_view();
//    curveChartView->resize(CureveChart->size());
//    curveChartView->setVisible(true);


//    CureveChart->show();
    grayStretchWidget = new GrayStretchWidget();
    connect(grayStretchWidget,&GrayStretchWidget::imageChange,this,&MainWindow::ImageChange);//连接成功，一定要写到报告
    grayStretchWidget->init(&image);
    grayStretchWidget->show();

}

void MainWindow::on_Fliter_clicked()
{
    fliterWeight = new FliterWeight();

    connect(fliterWeight,&FliterWeight::imageChange,this,&MainWindow::ImageChange);//连接成功，一定要写到报告
    fliterWeight->init(&image);
//    //fliterWeight->GaussianFliter(1.3,3);
//    //fliterWeight->imageChange();
    fliterWeight->show();


}

void MainWindow::on_grayChange_clicked()
{
    grayChangeWiget = new GrayChangeWidget();
    connect(grayChangeWiget,&GrayChangeWidget::imageChange,this,&MainWindow::ImageChange);//连接成功，一定要写到报告
    grayChangeWiget->init(&image);
    grayChangeWiget->show();


//    QWidget *CureveChart = new QWidget();
//    curveChartView= new CurveChartView(CureveChart);

//    //初始化表格
//    curveChartView->init_curve_parameter(1);//2是灰度拉伸，1是灰度变换
//    curveChartView->init_signals_and_slots();
//    curveChartView->init_curve_chart_view();
//    curveChartView->resize(CureveChart->size());
//    curveChartView->setVisible(true);


//    CureveChart->show();

//    binarizationWidget = new BinarizationWidget();

//    binarizationWidget->init(&image);
//    connect(binarizationWidget,&BinarizationWidget::imageChange,this,&MainWindow::ImageChange);//连接成功，一定要写到报告
//    binarizationWidget->show();


}

void MainWindow::on_rotate_sliderMoved(int position)
{
    //qt的转换
//    QMatrix matrix;

//    matrix.rotate(position);
//    QTransform qtransForm(matrix);

//    scene.clear();
//    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(image.qPixmap);//QGraphicsItem子类之一
//    item->setFlags(QGraphicsItem::ItemIsMovable);//支持鼠标拖动

//    item->setTransform(qtransForm);
//    scene.addItem(item);



    rotate();



}

void MainWindow::on_rotate_valueChanged(int value)
{
    rotate();
}

void MainWindow::on_xAxis_sliderMoved(int position)
{
    translate();
}


void MainWindow::rotate()
{
    image.cvImage=image.originImage.clone();
    int height=image.cvImage.rows;
    int width = image.cvImage.cols;
    int channels = image.cvImage.channels();

    double a = 1;
    double b = 0;
    double c = 0;
    double d = 1;
    double tx = 0;
    double ty = 0;

    // get detriment
    double det = a * d - b * c;

    int theta=ui->rotate->value();

    if(theta!=0)
    {
        double rad = theta / 180. * M_PI;
        a = std::cos(rad);
        b = - std::sin(rad);
        c = std::sin(rad);
        d = std::cos(rad);

        // get detriment
        det = a * d - b * c;
        double cx = width / 2.;
        double cy = height / 2.;
        double new_cx = (d * cx - b * cy) / det;
        double new_cy = (- c * cx + a * cy) / det;
        tx = new_cx - cx;
        ty = new_cy - cy;
    }
    int resized_width = (int)width;
    int resized_height = (int)height;

    // other parameters
    int x_before, y_before;
    if(channels==3)
        image.cvImage=cv::Mat::zeros(resized_height,resized_width,CV_8UC3);
    else if(channels==1)
        image.cvImage=cv::Mat::zeros(resized_height,resized_width,CV_8UC1);
    for (int y = 0; y < resized_height; y++){
       for (int x = 0; x < resized_width; x++){

         // get original position x
         x_before = (int)((d * x - b * y) / det - tx);

         if ((x_before < 0) || (x_before >= width)){
           continue;
         }

         // get original position y
         y_before = (int)((-c * x + a * y) / det - ty);

         if ((y_before < 0) || (y_before >= height)){
           continue;
         }

         // assign pixel to new position
         for (int c = 0; c < channels; c++){
             if(channels==3)
                image.cvImage.at<cv::Vec3b>(y, x)[c] = image.originImage.at<cv::Vec3b>(y_before, x_before)[c];
             else if(channels==1)
                image.cvImage.at<uchar>(y, x) = image.originImage.at<uchar>(y_before, x_before);

         }
       }
     }

    ImageChange();
}
void MainWindow::translate()
{
    image.cvImage=image.originImage.clone();
    int height=image.cvImage.rows;
    int width = image.cvImage.cols;
    int channels = image.cvImage.channels();


    double a = 1;
    double b = 0;
    double c = 0;
    double d = 1;
    double tx = ui->xAxis->value();
    double ty = ui->yAxis->value();

    // get detriment
    double det = a * d - b * c;

    // Resize width and height
    int resized_width = (int)(width * a);
    int resized_height = (int)(height * d);

    // other parameters
    int x_before, y_before;

    if(channels==3)
        image.cvImage=cv::Mat::zeros(resized_height,resized_width,CV_8UC3);
    else if(channels==1)
        image.cvImage=cv::Mat::zeros(resized_height,resized_width,CV_8UC1);
      // Affine transformation
      for (int y = 0; y < resized_height; y++){
        for (int x = 0; x < resized_width; x++){

          // get original position x
          x_before = (int)((d * x - b * y) / det - tx);

          if ((x_before < 0) || (x_before >= width)){
            continue;
          }

          // get original position y
          y_before = (int)((-c * x + a * y) / det - ty);

          if ((y_before < 0) || (y_before >= height)){
            continue;
          }

          // assign pixel to new position
          for (int c = 0; c < channels; c++){
              if(channels==3)
                 image.cvImage.at<cv::Vec3b>(y, x)[c] = image.originImage.at<cv::Vec3b>(y_before, x_before)[c];
              else if(channels==1)
                 image.cvImage.at<uchar>(y, x) = image.originImage.at<uchar>(y_before, x_before);

          }
        }
      }
      ImageChange();
}
void MainWindow::scale()
{
    image.cvImage=image.originImage.clone();
    int height=image.cvImage.rows;
    int width = image.cvImage.cols;
    int channels = image.cvImage.channels();


    double a = ui->Xscale->value();
    double b = 0;
    double c = 0;
    double d = ui->Yscale->value();
    double tx = 0;
    double ty = 0;

    // get detriment
    double det = a * d - b * c;

    // Resize width and height
    int resized_width = (int)(width * a);
    int resized_height = (int)(height * d);

    // other parameters
    int x_before, y_before;

    if(channels==3)
        image.cvImage=cv::Mat::zeros(resized_height,resized_width,CV_8UC3);
    else if(channels==1)
        image.cvImage=cv::Mat::zeros(resized_height,resized_width,CV_8UC1);

    for (int y = 0; y < resized_height; y++){
        for (int x = 0; x < resized_width; x++){

          // get original position x
          x_before = (int)((d * x - b * y) / det - tx);

          if ((x_before < 0) || (x_before >= width)){
            continue;
          }

          // get original position y
          y_before = (int)((-c * x + a * y) / det - ty);

          if ((y_before < 0) || (y_before >= height)){
            continue;
          }

          // assign pixel to new position
          for (int c = 0; c < channels; c++){
              if(channels==3)
                 image.cvImage.at<cv::Vec3b>(y, x)[c] = image.originImage.at<cv::Vec3b>(y_before, x_before)[c];
              else if(channels==1)
                 image.cvImage.at<uchar>(y, x) = image.originImage.at<uchar>(y_before, x_before);

          }
        }
      }

    ImageChange();

}

void MainWindow::on_xAxis_valueChanged(int value)
{
    translate();
}

void MainWindow::on_yAxis_sliderMoved(int position)
{
    translate();
}

void MainWindow::on_yAxis_valueChanged(int value)
{
    translate();
}

void MainWindow::on_Xscale_sliderMoved(int position)
{
    scale();
}

void MainWindow::on_Xscale_valueChanged(int value)
{
    scale();
}

void MainWindow::on_Yscale_sliderMoved(int position)
{
    scale();
}

void MainWindow::on_Yscale_valueChanged(int value)
{
    scale();
}

void MainWindow::on_apply_clicked()
{
    image.originImage=image.cvImage.clone();
    ImageChange();
}

void MainWindow::on_showOrigin_clicked()
{
    image.cvImage=image.originImage.clone();
    ImageChange();
}

void MainWindow::on_morphology_clicked()
{

    morphologyWidget = new MorphologyWidget();
    connect(morphologyWidget,&MorphologyWidget::imageChange,this,&MainWindow::ImageChange);//连接成功，一定要写到报告
    morphologyWidget->init(&image);
    morphologyWidget->show();
}

void MainWindow::on_linerTranslate_clicked()
{
    linerTranslateWidget = new LinerTranslateWidget();
    connect(linerTranslateWidget,&LinerTranslateWidget::imageChange,this,&MainWindow::ImageChange);//连接成功，一定要写到报告
    linerTranslateWidget->init(&image);
    linerTranslateWidget->show();
}
