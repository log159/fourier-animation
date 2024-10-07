#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

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

class DetailWidget : public QWidget
{
    Q_OBJECT
private:
    QPointF drawPoint;
    int m_FittingNum=0;
    QVector<QVector<Complex>>           m_Points;
    QMap<int,Complex>                   m_PointArrow;
public:
    DetailWidget(QWidget *parent = nullptr);
    void setDrawPoint(const QPointF &point);

    void setFittingNum(int num);

    void setPoints(QVector<QVector<Complex>>& points);

    void setPointArrow(QMap<int,Complex>& pointArrow);

    void paintEvent(QPaintEvent*e);

    void drawLineWithArrow(QPainter& painter, QPen pen, QPointF start, QPointF end);
signals:

public slots:
};

#endif // DETAILWIDGET_H
