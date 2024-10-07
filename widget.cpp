#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    initConnect();


}

Widget::~Widget()
{
    delete ui;
}
void Widget::init()
{
    this->resize(1000,900);


    //放大视图
    m_DetailWidget=new DetailWidget();
    m_DetailWidget->resize(700,700);
    m_DetailWidget->show();
    m_DetailWidget->setFittingNum(this->m_FittingNum);
    m_DetailWidget->move(10,10);
    this->move(m_DetailWidget->width()+20,10);

    initPath(m_TexPath);

    initPoints();



}

void Widget::initConnect()
{

    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){
        static int t=0;t++;
        if(t>m_TimeNumM[m_Pos]){
            t=0;
            ++m_Pos;
            if(m_Pos>=m_Vvv.size()){
                timer->stop();
                return ;
            }
        }
        double timecoming=double(t)/double(m_TimeNumM[m_Pos]);
        Complex c;
        for(auto it=m_PointsMap[m_Pos].begin();it!=m_PointsMap[m_Pos].end();++it){
            Complex ct;
            ct.m_A=it.value().m_A*cos(it.value().m_B+double(it.key())*2.0*MY_PI*timecoming);
            ct.m_B=it.value().m_A*sin(it.value().m_B+double(it.key())*2.0*MY_PI*timecoming);
            c.m_A+=ct.m_A;
            c.m_B+=ct.m_B;
            m_PointArrow[it.key()]=ct;
        }
        m_Points[m_Pos].push_back(c);
        update();

        m_DetailWidget->setPoints(m_Points);
        m_DetailWidget->setPointArrow(m_PointArrow);


    });

    timer->start(10);
}

void Widget::initPoints()
{

    for(int i=0;i<m_Vvv.size();++i){
        m_Pos=i;
        for(int n=-m_FittingNum/2;n<=m_FittingNum/2;++n){
            Complex cn;
            for(int j=1;j<=m_CutNum;++j){
                double jek=double(j)/double(m_CutNum);
                Complex ct=Complex::toExpComplex(f(jek));
                cn.m_A+=(ct.m_A*cos(-2.0*MY_PI*double(n)*jek+ct.m_B))/double(m_CutNum);
                cn.m_B+=(ct.m_A*sin(-2.0*MY_PI*double(n)*jek+ct.m_B))/double(m_CutNum);
            }
            cn=Complex::toExpComplex(cn);
            m_PointsMap[m_Pos][n]=cn;

        }
        qDebug()<<m_Pos;
    }
    m_Pos=0;

}

void Widget::initPath(const QString &path)
{

    QFile file(path);
    int index=-1;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if(line.startsWith("(")){++index;}
            if(line.startsWith(" --") || line.startsWith("(")){
                QVector<QPointF> coordinates;
                QString strLine = line.remove(" ");
                QRegularExpression regex("\\((-?\\d+(\\.\\d+)?pt,\\s*-?\\d+(\\.\\d+)?pt)\\)");
                QRegularExpressionMatchIterator it = regex.globalMatch(strLine);
                while (it.hasNext()) {
                    QRegularExpressionMatch match = it.next();
                    QString coordinate = match.captured(1);
                    QStringList parts = coordinate.split(',');
                    if (parts.size() == 2) {
                        bool okX, okY;
                        qreal x = parts[0].remove("pt").toDouble(&okX);
                        qreal y = parts[1].remove("pt").toDouble(&okY);
                        if (okX && okY) {
                            coordinates.push_back(QPointF(x, y)/2-QPointF(+230,-230));//偏移量调节

                        }
                    }
                }
                m_Mvv[index].push_back(coordinates);
            }
        }
        file.close();
    }


    for(int i=0;i<m_Mvv.size();++i){
        QVector<QVector<QPointF>>vv;
        for(int j=0;j<m_Mvv[i].size();++j){
            if(m_Mvv[i][j].size()>0){
                vv.push_back(m_Mvv[i][j]);
            }
        }
        if(vv.size()>0){
            m_Vvv.push_back(vv);
        }
    }

    m_PointsMap.resize(m_Vvv.size());
    m_Points.resize(m_Vvv.size());


    for(int i=0;i<m_Vvv.size();++i){
        m_TimeNumM[i]=m_Vvv[i].size()*10;

    }

}


Complex Widget::f(const double &t)
{
    Complex c;

    int size=m_Vvv[m_Pos][int(double(m_Vvv[m_Pos].size()-1)*t)].size();
    int l=int(double(m_Vvv[m_Pos].size()-1)*t);
    double fl=double(m_Vvv[m_Pos].size()-1)*t;
    double nl= fl-double(l);
    //点(到下一点)
    if(size==1){

        if(m_Vvv[m_Pos].size()>l+1){
            QPointF c0,c1;
            c0=m_Vvv[m_Pos][l][0];
            c1=m_Vvv[m_Pos][l+1][0];

            c.m_A=c0.x()+(c1.x()-c0.x())*nl;
            c.m_B=c0.y()+(c1.y()-c0.y())*nl;
        }
        else {
            c.m_A=m_Vvv[m_Pos][0][0].x();
            c.m_B=m_Vvv[m_Pos][0][0].y();
        }
    }
    //线
    else if(size==2){
        QPointF c0,c1;
        c0=m_Vvv[m_Pos][l][0];
        c1=m_Vvv[m_Pos][l][1];
        c.m_A=c0.x()+(c1.x()-c0.x())*nl;
        c.m_B=c0.y()+(c1.y()-c0.y())*nl;
    }
    //三阶贝塞尔曲线
    if(size==4)
    {
        QPointF c0,c1,c2,c3;
        c0=m_Vvv[m_Pos][l][0];
        c1=m_Vvv[m_Pos][l][1];
        c2=m_Vvv[m_Pos][l][2];
        c3=m_Vvv[m_Pos][l][3];
        double x = pow(1.0 - nl, 3.0) * c0.x() + 3.0 * nl * pow(1.0 - nl, 2.0) * c1.x() + 3.0 * pow(nl, 2.0) * (1.0 - nl) * c2.x() + pow(nl, 3.0) * c3.x();
        double y = pow(1.0 - nl, 3.0) * c0.y() + 3.0 * nl * pow(1.0 - nl, 2.0) * c1.y() + 3.0 * pow(nl, 2.0) * (1.0 - nl) * c2.y() + pow(nl, 3.0) * c3.y();
        QPointF point(x, y);
        c.m_A=point.x();
        c.m_B=point.y();
    }
    return c;
}


void Widget::paintEvent(QPaintEvent *e)
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

    Complex ct;
    double r=0.0;
    ct.m_A=0;ct.m_B=0;
    int i=0;
    do
    {
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
        painter.strokePath(path, QPen(Qt::white, 2));  // 使用黑色线条，宽度为2
    }

}

void Widget::drawLineWithArrow(QPainter& painter, QPen pen, QPointF start, QPointF end)
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
