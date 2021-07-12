#ifndef CURVECHARTVIEW_H
#define CURVECHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCore/QtMath>

QT_CHARTS_USE_NAMESPACE

class CurveChartView : public QChartView
{
    Q_OBJECT

public:
    CurveChartView(QWidget *parent = Q_NULLPTR);
    ~CurveChartView();

    void setForceCorrectnessOnDrag(bool status);			// 限制拖动点范围
    void setSeriesIndex(int index);							// 更改curve type类型
    void setCurrentPointValue(const QPointF &value);		// 修改当前坐标数据
    void setSeriesList(QList<QScatterSeries *> sList, QList<QLineSeries *> lList);						// 设置曲线数据
    void setYRange(int minVal, int maxVal);					// Y轴范围

    void init_curve_chart_view();           //设置chart
    void init_curve_parameter(int model);   //model=1是灰度变换，2是灰度拉伸
    void init_signals_and_slots();

    int threshold;

    int func[256];
    double k;
    double b;

    int upperLimit;
    int lowerLimit;

    int lineIndex1;
    int lineIndex2;
    int pointIndex;

    int X1x;
    int X1y;
    int X2x;
    int X2y;

    void updateSeriesData(const QPointF &point);			// 更新数据点数据

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool forceCorrectnessFlag;		// 拖动范围限制标志
    bool dragPointFlag;				// 拖动数据点标志
    QPoint beginPoint;				// 选择矩形区域的起点
    QPoint endPoint;				// 选择矩形区域的重点

    int dataCount;					// 数据点个数
    int minValue;					// Y轴最小值
    int maxValue;					// Y轴最大值

    QScatterSeries *scatter;	// 散点序列集合
    QLineSeries *line;		// 曲线序列集合

    int color_map[2]= { 0x696969, 0x1E90FF };                       //颜色
    int curveIndex;
    int minX;
    int maxX;
    int minY;
    int maxY;




    bool is_grayStretch;
    bool is_grayChange;
    bool is_binaryation;



    qreal distance(const QPointF &p1, const QPointF &p2);	// 计算距离
    qreal distance(const QPointF &p1,const QPointF &p2,const QPointF &c);
    void detectDragPoint(const QPointF &point);				// 捕获待拖动的数据点


signals:
    void signalMouseMovePoint(QPoint point);		// 鼠标移动信号
    void signalCurrentDragPoint(QPointF point);		// 数据点拖动信号
    void ThresholdChange();

    void UpperLimitChange();
    void LowerLimitChange();


    void X1StretchChange();
    void X2StretchChange();

    void FuncChange();


};

#endif
