#include "graphwindow.h"
#include "ui_graphwindow.h"
#include "mainwindow.h"
#include "sstream"
#include "iomanip"

GraphWindow::GraphWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::przekaz(float *Tablica, unsigned int n, double l){
    Wykres = new QPointF [n];
    bn=n;
    bl=l;
    float a, b;
    float min = Tablica[0];
    float max = Tablica[0];

    a=530;
    for(unsigned int j=0; j<n; j++){
        if(min>Tablica[j]){
            min=Tablica[j];
        }
        if(max<Tablica[j]){
            max=Tablica[j];
        }
    }
    b=300/(max-min);

    maxw = max;
    minw = (max-min)*b;


    for(unsigned int i=0; i<n; i++){
        Wykres[i] += QPointF(static_cast<double>(50+a*i/(n-1)),
                             static_cast<double>(30+b*(max-Tablica[i])));
    }


    /*OPISY POZIOME*/

    std::ostringstream strs;

    strs <<fixed << setprecision(2) << max;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);

    ui->max->setText(qstr);
    ui->max->setGeometry(10,25,35,13);
    //pomin stringa double->qstring


    strs.str("");
    strs << min;
    str = strs.str();
    qstr = QString::fromStdString(str);

    ui->min->setText(qstr);
    ui->min->setGeometry(10,minw+25,35,13);


    strs.str("");
    strs << (max-min)/3*2+min;
    str = strs.str();
    qstr = QString::fromStdString(str);

    ui->m13->setText(qstr);
    ui->m13->setGeometry(10,minw/3+25,35,13);


    strs.str("");
    strs << (max-min)/3+min;
    str = strs.str();
    qstr = QString::fromStdString(str);

    ui->m23->setText(qstr);
    ui->m23->setGeometry(10,2*minw/3+25,35,13);


    /*OPISY PIONOWE*/

    strs.str("");
    strs << l;
    str = strs.str();
    qstr = QString::fromStdString(str);
    ui->n1->setText(qstr);

    strs.str("");
    strs << l/2;
    str = strs.str();
    qstr = QString::fromStdString(str);

    ui->n05->setText(qstr);

    delete [] Tablica;

}


void GraphWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::gray, 0));
    painter.drawRect(50,20,530,340);

    /*LINIE PIONOWE*/

    painter.setPen(QColor(210, 210, 210));
    painter.drawLine(315, 20, 315, 360);

    /*LINIE POZIOME*/

    painter.setPen(QPen(Qt::gray, 0));
    painter.drawLine(50,30, 580,30);
    painter.drawLine(50,minw+30, 580,minw+30);
    painter.drawLine(50, minw/3+30, 580, minw/3+30);
    painter.drawLine(50, 2*minw/3+30, 580, 2*minw/3+30);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::blue, 0));
    painter.drawPolyline(Wykres, bn);

}
