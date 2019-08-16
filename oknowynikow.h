#ifndef OKNOWYNIKOW_H
#define OKNOWYNIKOW_H

#include <QDialog>
#include <QPainter>


namespace Ui {
class OknoWynikow;
}

class OknoWynikow : public QDialog
{
    Q_OBJECT

public:
    explicit OknoWynikow(QWidget *parent = nullptr);
    ~OknoWynikow();

    void paintEvent(QPaintEvent*);
    void przekaz(double *Tablica, int n, double l);
    int bn;
    int bl;
    double *Btab;
    QPointF *Wykres;
    double minw, maxw;


private slots:


private:
    Ui::OknoWynikow *ui;

};

#endif // OKNOWYNIKOW_H
