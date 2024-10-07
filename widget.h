#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include <QFile>
#include <QRegularExpression>
#include <QSlider>
#include <QPainterPath>

#include "complex.h"
#include "detailwidget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

private:
    QString     m_TexPath        ="C:/XXX/c.tex";   //tex路径
    int         m_FittingNum     =501;              //拟合用的函数数量
    int         m_CutNum         =500;              //近似积分切分区间的数量
    int         m_Pos            =0;                //pos坐标

    QMap<int,int>                       m_TimeNumM;
    DetailWidget*                       m_DetailWidget    =nullptr;
    QVector<QMap<int,Complex>>          m_PointsMap;
    QVector<QVector<Complex>>           m_Points;
    QMap<int,Complex>                   m_PointArrow;
    QMap<int,QVector<QVector<QPointF>>> m_Mvv;
    QVector<QVector<QVector<QPointF>>>  m_Vvv;



public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Complex f(const double& t);

    void init();

    void initConnect();

    void initPoints();

    void initPath(const QString& path);

    void paintEvent(QPaintEvent* e);

    void drawLineWithArrow(QPainter& painter, QPen pen, QPointF start, QPointF end);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
