#include "detailwidget.h"

DetailWidget::DetailWidget(QWidget *parent) : QWidget(parent)
{
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){

        update();

    });
    timer->start(1);
}

void DetailWidget::setDrawPoint(const QPointF &point)
{
    drawPoint = point;
    update(); // 触发重绘事件
}

void DetailWidget::setFittingNum(int num)
{
    m_FittingNum=num;
}

void DetailWidget::setPoints(QVector<QVector<Complex> > &points)
{
    m_Points = points;
    for(int i=0;i<m_Points.size();++i){
        for(int j=0;j<m_Points[i].size();++j){
            m_Points[i][j].m_A*=20.0;
            m_Points[i][j].m_B*=20.0;
        }
    }
}

void DetailWidget::setPointArrow(QMap<int, Complex> &pointArrow)
{
    m_PointArrow = pointArrow;
    for(auto&val:m_PointArrow){
        val.m_A*=20.0;
        val.m_B*=20.0;

    }
}



void DetailWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QBrush brush;
    brush.setColor(QColor(0,0,0));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0,0,this->width(),this->height());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(255,255,255,75), 1));
    painter.setWindow(0, height(),  width(), -1 * height());
    int centerX = width() / 2;
    int centerY = height() / 2;
    painter.translate(centerX, centerY);
    painter.drawLine(-centerX, 0, centerX, 0);
    painter.drawLine(0, -centerY, 0, centerY);

    // 应用视图变换
    painter.translate(0 - drawPoint.x(), 0 - drawPoint.y());

    Complex ct;
    double r=0.0;
    ct.m_A=0;ct.m_B=0;
    int i=0;
    do
    {
        drawPoint=QPointF(ct.m_A+m_PointArrow[i].m_A,ct.m_B+m_PointArrow[i].m_B);
        drawLineWithArrow(painter,painter.pen(),QPointF(ct.m_A,ct.m_B),QPointF(ct.m_A+m_PointArrow[i].m_A,ct.m_B+m_PointArrow[i].m_B));
        r =Complex::toExpComplex(m_PointArrow[i]).m_A;
        painter.drawEllipse(QPointF(ct.m_A,ct.m_B),r,r);
        ct.m_A+=m_PointArrow[i].m_A;
        ct.m_B+=m_PointArrow[i].m_B;
        if(i!=0){
            if(i<0){i=-i;}
            else {i=-i;--i;}
        }
        else {--i;}
    }while(qAbs(i)<=m_FittingNum/2);
    painter.setPen(QPen(Qt::white, 1));
    for (int i = 0; i < m_Points.size(); ++i) {

        QPainterPath path;
        if (!m_Points[i].isEmpty()) {
            path.moveTo(QPointF(m_Points[i][0].m_A,m_Points[i][0].m_B));
            if(m_Points[i].size()<2){
            }
            else {
                for (int j = 0; j < m_Points[i].size() - 1; ++j) {
                    QPointF cp1, cp2;
                    cp1 = QPointF(m_Points[i][j].m_A,m_Points[i][j].m_B);
                    cp2 = QPointF(m_Points[i][j+1].m_A,m_Points[i][j+1].m_B);
                    path.cubicTo(cp1, cp2, cp2);
                }
            }
        }
        painter.strokePath(path, QPen(Qt::white, 2));
    }
}
void DetailWidget::drawLineWithArrow(QPainter& painter, QPen pen, QPointF start, QPointF end)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    qreal arrowSize = 10;
    painter.setPen(pen);
    painter.setBrush(pen.color());
    QLineF line(end, start);
    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + MY_PI / 3) * arrowSize,cos(angle + MY_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + MY_PI - MY_PI / 3) * arrowSize,cos(angle + MY_PI - MY_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;
    painter.drawLine(line);
    painter.drawPolygon(arrowHead);
    painter.setBrush(Qt::NoBrush);
}
