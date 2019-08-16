#include "oknowynikow.h"
#include "ui_oknowynikow.h"
#include "mainwindow.h"
#include "sstream"
#include "iomanip"

OknoWynikow::OknoWynikow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoWynikow)
{
    ui->setupUi(this);
}

OknoWynikow::~OknoWynikow()
{
    delete ui;
}

void OknoWynikow::przekaz(double *Tablica, int n, double l){
    Wykres = new QPointF [n];
    bn=n;
    bl=l;
    double a, b;
    double min = Tablica[0];
    double max = Tablica[0];

    a=530;
    for(int j=0; j<n; j++){
        if(min>Tablica[j]){
            min=Tablica[j];
        }
        if(max<Tablica[j]){
            max=Tablica[j];
        }
    }
    b=300/(max-min);

    maxw = max;
    //minw = min;
    minw = (max-min)*b;


    for(int i=0; i<n; i++){
        Wykres[i] += QPointF(50+a*i/(n-1), 30+b*(max-Tablica[i]));
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


void OknoWynikow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);


    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::gray, 0));
    painter.drawRect(50,20,530,340);

    /*LINIE PIONOWE*/

    painter.setPen(QColor(210, 210, 210));
    painter.drawLine(315, 20, 315, 360);
    //painter.drawLine(320, 20, 320, 360);
    //painter.drawLine(70, 20, 70, 360);

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
