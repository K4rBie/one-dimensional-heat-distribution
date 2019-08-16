#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rod.h"
#include <qfilesystemmodel.h>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Segment *last_segment, *head, *tail; // deklaracja zmiennych wewntąrz listy. przerób w klasę
    int allocated_points;

    double total_length;
    int sim_points_number;
    double section_area;
    double thermal_conduct_coeff;
    double starting_temperature;
    double aTk;
    double env_temperature;

    Rod rod;

private slots:
    void on_wprowadzbut_clicked();
    void on_dodajbut_clicked();
    void on_wprowotbut_clicked();
    void on_czyscbut_clicked();
    void on_doplikubut_clicked();
    void on_wczytajbut_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *przegl;
};

#endif // MAINWINDOW_H
