#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDialog>
#include <QPainter>


namespace Ui {
class GraphWindow;
}

class GraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

    void paintEvent(QPaintEvent*);
    void przekaz(double *Tablica, int n, double l);
    int bn;
    int bl;
    double *Btab;
    QPointF *Wykres;
    double minw, maxw;


private slots:


private:
    Ui::GraphWindow *ui;

};

#endif // OKNOWYNIKOW_H
