#include "CurveChartView.h"

CurveChartView::CurveChartView(QWidget *parent)
    : QChartView(parent)
{
    forceCorrectnessFlag = false;
    dragPointFlag = false;
    pointIndex = 0;
    minValue = 0;
    maxValue = 0;
    this->setMouseTracking(true);
}

CurveChartView::~CurveChartView()
{
}

void CurveChartView::mousePressEvent(QMouseEvent *event)
{
    QCursor currentCusor = this->cursor();
    if (event->button() == Qt::LeftButton)
    {
        if (currentCusor.shape() == Qt::CrossCursor)
        {
            //this->setDragMode(QGraphicsView::RubberBandDrag);
            beginPoint = event->pos();
        }
        else
        {
            //this->setDragMode(QGraphicsView::NoDrag);
            dragPointFlag = true;
        }
    }
    QChartView::mousePressEvent(event);
}


void CurveChartView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    emit signalMouseMovePoint(point);
    if (dragPointFlag)
    {
        this->setCursor(Qt::SizeHorCursor);
        updateSeriesData(point);
    }
    else
    {
        this->setCursor(Qt::CrossCursor);
        detectDragPoint(point);
    }

    QChartView::mouseMoveEvent(event);
}


void CurveChartView::mouseReleaseEvent(QMouseEvent *event)
{
    QCursor currentCusor = this->cursor();
    if (event->button() == Qt::LeftButton)
    {
        if (currentCusor.shape() == Qt::CrossCursor)
        {
            endPoint = event->pos();
            QRectF rectF;
            rectF.setTopLeft(this->beginPoint);
            rectF.setBottomRight(this->endPoint);
            this->chart()->zoomIn(rectF);
        }
        else
        {
            dragPointFlag = false;
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        this->chart()->zoomReset();
    }
    QChartView::mouseReleaseEvent(event);
}


void CurveChartView::setForceCorrectnessOnDrag(bool status)
{
    forceCorrectnessFlag = status;
}

void CurveChartView::setYRange(int minVal, int maxVal)
{
    minValue = minVal;
    maxValue = maxVal;
}

qreal CurveChartView::distance(const QPointF &p1, const QPointF &p2)
{
    return qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x())
        + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

qreal CurveChartView::distance(const QPointF &p1,const QPointF &p2,const QPointF &c)
{
    float cross = (p2.x() - p1.x()) * (c.x() - p1.x()) + (p2.y() - p1.y()) * (c.y() - p1.y());
    if (cross <= 0) return qSqrt((c.x() - p1.x()) * (c.x() - p1.x()) + (c.y() - p1.y()) * (c.y() - p1.y()));

    float d2 = (p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y());
    if (cross >= d2) return qSqrt((c.x() - p2.x()) * (c.x() - p2.x()) + (c.y() - p2.y()) * (c.y() - p2.y()));

    float r = cross / d2;
    float px = p1.x() + (p2.x() - p1.x()) * r;
    float py = p1.y() + (p2.y() - p1.y()) * r;
    return qSqrt((c.x() - px) * (c.x() - px) + (py - c.y()) * (py - c.y()));

}

void CurveChartView::detectDragPoint(const QPointF &point)//拖拽时候的函数
{
    // detect drag point
    QPointF curPoint = this->chart()->mapToValue(point);//鼠标的点
//    QScatterSeries *curSeries = scatterList.at(seriesIndex);
    QVector<QPointF> seriesData = scatter->pointsVector();

    //二值化计算线
    if(is_binaryation)
    {
        for (int i = 0; i < seriesData.count()-1; i++)
        {
            if (distance(seriesData.at(i),seriesData.at(i+1), curPoint) <= 2)//检测鼠标是不是在线上
            {
                lineIndex1 = i;
                lineIndex2 = i+1;
                if(lineIndex1==1||lineIndex2==2)
                {
                    this->setCursor(Qt::SizeHorCursor);
                }
                //emit signalCurrentDragPoint(seriesData.at(i));
                break;
            }
        }
    }

    //灰度化计算点
    else if(is_grayChange)
    {
        for (int i = 0; i < seriesData.count(); i++)
        {
            if (distance(curPoint, seriesData.at(i)) <= 3)//检测鼠标是不是在点上
            {
                pointIndex = i;
                if(pointIndex==2||pointIndex==3)
                {
                    this->setCursor(Qt::SizeHorCursor);
                }
                //emit signalCurrentDragPoint(seriesData.at(i));
                break;
            }
        }
    }

   else if(is_grayStretch)
    {
        for (int i = 0; i < seriesData.count(); i++)
        {
            if (distance(curPoint, seriesData.at(i)) <= 3)//检测鼠标是不是在点上
            {
                pointIndex = i;
                if(pointIndex==1||pointIndex==2)
                {
                    this->setCursor(Qt::SizeHorCursor);
                }
                //emit signalCurrentDragPoint(seriesData.at(i));
                break;
            }
        }
    }
    else//返回函数
    {
        for (int i = 0; i < seriesData.count(); i++)
        {
            if (distance(curPoint, seriesData.at(i)) <= 3)//检测鼠标是不是在点上
            {
                pointIndex = i;
                this->setCursor(Qt::SizeHorCursor);
                //emit signalCurrentDragPoint(seriesData.at(i));
                break;
            }
        }
    }



    //detect drag line
//    QLineSeries *lin = line;
//    QVector<QPointF> seriesData = line->pointsVector();



}


void CurveChartView::updateSeriesData(const QPointF &point)
{
    //二值化共4个点
    if(is_binaryation)
    {
        QPointF original1 = line->at(lineIndex1);
        QPointF original2 = line->at(lineIndex2);

        QPointF target1 = this->chart()->mapToValue(point);//鼠标位置
        QPointF target2 = this->chart()->mapToValue(point);//鼠标位置


        target1.setY(original1.y());
        target2.setY(original2.y());
        if(target1.x()<=0)
        {
            target1.setX(0);
            target2.setX(0);
        }
        else if(target1.x()>=255)
        {
            target1.setX(255);
            target2.setX(255);
        }
        scatter->replace(original1, target1);
        scatter->replace(original2, target2);
        line->replace(original1, target1);
        line->replace(original2, target2);

        threshold = target1.x();//改变阈值
        emit ThresholdChange();
    }
    //灰度拉伸4个点
    else if(is_grayStretch)
    {
        QPointF original = line->at(pointIndex);

        QPointF target = this->chart()->mapToValue(point);//鼠标位置

        if(target.x()<=1)
        {
            target.setX(1);
        }
        else if(target.x()>=254)
        {
            target.setX(254);
        }
        if(target.y()<=1)
        {
            target.setY(1);
        }
        else if(target.y()>=254)
        {
            target.setY(254);
        }
        scatter->replace(original, target);
        line->replace(original, target);

        if(pointIndex==1)
        {
            X1x = target.x();//改变阈值
            X1y = target.y();
            emit X1StretchChange();
        }
        else if(pointIndex==2)
        {
            X2x = target.x();//改变阈值
            X2y = target.y();
            emit X2StretchChange();
        }

    }
    //灰度化共6个点
    else if(is_grayChange)
    {
        QPointF original = scatter->at(pointIndex);
        QPointF target = this->chart()->mapToValue(point);//鼠标位置
        if(pointIndex==2)
        {
            QPointF former = scatter->at(pointIndex-1);
            QPointF later = scatter->at(pointIndex+1);
            QPointF origin_former = scatter->at(pointIndex-1);

            target.setX(target.y());
            former.setX(target.x());

            //控制边界条件
            if(target.x()<=0)
            {
                target.setX(0);
                target.setY(0);
                former.setX(0);
            }
            else if(target.x()>=255)
            {
                target.setX(255);
                target.setY(255);
                former.setX(255);
            }
            if(target.x()>=later.x()-1)
            {
                target.setX(later.x()-1);
                target.setY(later.y()-1);
                former.setX(later.x()-1);
            }
            scatter->replace(original, target);
            scatter->replace(origin_former,former);
            line->replace(original, target);
            line->replace(origin_former,former);
            lowerLimit = target.x();
            emit LowerLimitChange();
        }
        else if(pointIndex==3)
        {
            QPointF former = scatter->at(pointIndex-1);
            QPointF later = scatter->at(pointIndex+1);
            QPointF origin_later = scatter->at(pointIndex+1);

            target.setX(target.y());
            later.setX(target.x());

            //控制边界条件
            if(target.x()<=0)
            {
                target.setX(0);
                target.setY(0);
                later.setX(0);
            }
            else if(target.x()>=255)
            {
                target.setX(255);
                target.setY(255);
                later.setX(255);
            }
            if(target.x()<=former.x()+1)
            {
                target.setX(former.x()+1);
                target.setY(former.y()+1);
                later.setX(former.x()+1);
            }



            scatter->replace(original, target);
            scatter->replace(origin_later,later);
            line->replace(original, target);
            line->replace(origin_later,later);
            upperLimit = target.x();
            emit UpperLimitChange();
        }
    }

    else
    {
        QPointF original = scatter->at(pointIndex);
        QPointF target = this->chart()->mapToValue(point);//鼠标位置

        //控制边界条件
        if(target.x()<=1)
        {
            target.setX(1);
        }
        else if(target.x()>=254)
        {
            target.setX(254);
        }
        if(target.y()>=254)
        {
            target.setY(254);
        }
        else if(target.y()<=1)
        {
            target.setY(1);
        }

        if(pointIndex==scatter->count()-1)
        {
            target.setX(255);
        }
        else if(pointIndex==0)
        {
             target.setX(0);
        }

        scatter->replace(original, target);
        line->replace(original, target);




        //通过插值获得每个点的值

        int x0=0;
        int y0=scatter->at(0).y();
        int x1=scatter->at(1).x();
        int y1=scatter->at(1).y();
        int x2=scatter->at(2).x();
        int y2=scatter->at(2).y();
        int x3=255;
        int y3=scatter->at(3).y();
        func[x0]=y0;
        func[x1]=y1;
        func[x2]=y2;
        func[x3]=y3;
        for(int i=0;i<x1;i++)
        {
            double tk =double(y1-y0)/double(x1-x0);
            double tb =y0;
            func[i]=int(tk*(i-x0)+tb);

        }
        for(int i=x1+1;i<x2;i++)
        {
            double tk =double(y2-y1)/double(x2-x1);
            double tb = y1;
            k=tk;
            b=tb-tk*x1;
            func[i]=int(tk*(i-x1)+tb);
        }
        k=double(y2-y1)/double(x2-x1);
        b=y1-k*x1;
        for(int i=x2+1;i<x3;i++)
        {
            double tk =double(y3-y2)/double(x3-x2);
            double tb = y2;
            func[i]=int(tk*(i-x2)+tb);
        }


//        for(int i=0;i<scatter->count();i++)
//        {
//            for(int j=0;j<=255/scatter->count();j++)
//            {
//                int index = 255*i/scatter->count()+j;
//                if(i==scatter->count()-1||i==0)
//                {
//                    func[index]=scatter->at(i).y();
//                }
//                else
//                {
//                     double k =double(scatter->at(i+1).y()-scatter->at(i).y())/double(scatter->at(i+1).x()-scatter->at(i).x());
//                     double b = scatter->at(i).y();
//                    func[index]=floor(k*(index-scatter->at(i).x())+b);
//                }
//            }
//        }
//        for(int i=0;i<255;i++)
//        {
//            double index=double(i)/255.0*4;
//            if(qAbs(index-0)<0.000001||qAbs(index-1)<0.000001||qAbs(index-2)<0.000001||qAbs(index-3)<0.00001||qAbs(index-4)<0.00001)//恰好在点上
//            {
//                func[i]=int(scatter->at((int)index).y());
//            }
//            else
//            {
//                //double y1=scatter->at(floor(index)).y();
//                //double y2=scatter->at(floor(index+1)).y();
//                //double x1=scatter->at(floor(index)).x();
//                //double x2=scatter->at(floor(index+1)).x();
//                double k=(scatter->at(floor(index+1)).y()-scatter->at(floor(index)).y())/(scatter->at(floor(index+1)).x()-scatter->at(floor(index)).x());
//                double b = scatter->at((int)index).y();
//                func[i]=floor(k*(i-scatter->at(floor(index+1)).x())+b);
//            }

//        }

        emit FuncChange();
    }





    //emit signalCurrentDragPoint(target1);
}


void CurveChartView::init_curve_parameter(int model)
{
    minX = 0;
    maxX = 255;
    minY = 0;
    maxY = 255;
    curveIndex = 0;
    int map[2] = { 0x696969, 0x1E90FF };        //设置点和线的颜色
    memcpy(color_map, map, sizeof(int)* 2);

    for(int i=0;i<256;i++)
        func[i]=i;

    if(model==1)//灰度变换
    {
        is_grayStretch=false;
        is_binaryation=false;
        is_grayChange=true;
    }
    else if(model==2)//灰度拉伸
    {
        is_grayStretch=true;
        is_grayChange=false;
        is_binaryation=false;
    }
    else if(model==3)//二值化
    {
        is_grayStretch=false;
        is_grayChange=false;
        is_binaryation=true;
    }
    else
    {
        is_grayStretch=false;
        is_grayChange=false;
        is_binaryation=false;
    }

    //添加灰度变化的点

    //二值化
    if(is_binaryation)
    {
        scatter = new QScatterSeries();
        line = new QLineSeries();


        scatter->append(minX,minY);
        scatter->append(maxX/2,0);
        scatter->append(maxX/2,maxY);
        scatter->append(maxX,maxY);

        line->append(minX,minY);
        line->append(maxX/2,0);
        line->append(maxX/2,maxY);
        line->append(maxX,maxY);

    }
    //灰度化
    else if(is_grayChange)
    {
        scatter = new QScatterSeries();
        line = new QLineSeries();


        scatter->append(0,0);
        scatter->append(50,0);
        scatter->append(50,50);
        scatter->append(100,100);
        scatter->append(100,255);
        scatter->append(255,255);

        line->append(0,0);
        line->append(50,0);
        line->append(50,50);
        line->append(100,100);
        line->append(100,255);
        line->append(255,255);
    }
    else if(is_grayStretch)
    {
        scatter = new QScatterSeries();
        line = new QLineSeries();


        scatter->append(0,0);
        scatter->append(50,50);
        scatter->append(200,200);
        scatter->append(255,255);

        line->append(0,0);
        line->append(50,50);
        line->append(200,200);
        line->append(255,255);
    }
    else
    {
        scatter = new QScatterSeries();
        line = new QLineSeries();

        for(int i=0;i<4;i++)
        {
            scatter->append(i*255/3,i*255/3);
            line->append(i*255/3,i*255/3);
        }
    }




    //这里不知道干啥用
    //    QScatterSeries *scatter2 = new QScatterSeries();
    //    QLineSeries *spline2 = new QLineSeries();
    //    // y = x * x
    //    for (int i = 0; i < 11; i++)
    //    {
    //        scatter2->append(i, i * i);
    //        spline2->append(i, i * i);
    //    }
    //    scatterList.append(scatter2);
    //    lineList.append(spline2);
}

void CurveChartView::init_signals_and_slots()
{
//    connect(this, SIGNAL(signalMouseMovePoint(QPoint)),
//        this, SLOT(slotMouseMovePoint(QPoint)));	// mouse drag event
//    connect(this, SIGNAL(signalCurrentDragPoint(QPointF)),
//        this, SLOT(slotCurrentDragPoint(QPointF)));	// drag point event
}



void CurveChartView::init_curve_chart_view()
{
    QChart *chart = new QChart();

    // add series


    QPen pen;
    pen.setColor(color_map[0]);
    pen.setWidth(2);
    line->setPen(pen);

    scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatter->setBorderColor(color_map[0]);
    scatter->setBrush(QBrush(color_map[0]));
    scatter->setMarkerSize(8);

    chart->addSeries(line);
    chart->addSeries(scatter);


    // chart setting
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->axisY()->setRange(minY, maxY);
    chart->axisY()->setTitleText("y");
    chart->axisX()->setRange(minX, maxX);
    chart->axisX()->setTitleText("x");
    chart->setMargins(QMargins(1, 1, 1, 1));


    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);
    //this->setSeriesIndex(0);
    this->setForceCorrectnessOnDrag(false);
    this->setYRange(minY, maxY);
}

