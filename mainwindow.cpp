#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwindow.h"
#include "QFileDialog"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_wprowadzbut_clicked()
{
    // kopiuje wartości z boxów do zmiennych
    rod.set_all_attributes(RodAttributes{
                               ui->total_length_box->value(),
                               ui->pole->value(),
                               ui->k->value(),
                               ui->Tp->value(),
                               ui->Tk->value(),
                               ui->Tot->value(),
                               static_cast<unsigned int>(ui->n->value()), 0}); //WARNING: can make problems

    if (rod.attributes_correct()){
        ui->Podzial->setEnabled(true);
        ui->wprowotbut->setDisabled(true);
        ui->doplikubut->setDisabled(true);

        ui->odcinekbar->setMaximum(ui->n->value());
        ui->sprawdzlab->setText("Dane prawidłowe");
    } else {
        ui->sprawdzlab->setText("Dane nieprawidłowe");
    }
}

void MainWindow::on_dodajbut_clicked(){

    if (rod.has_unalloc_pts()){ //Most of this should probably be done inside Rod class

        double section_length = ui->lodc->value();
        double heat_transfer_coeff = ui->alfa->value();
        int section_sim_points = static_cast<int>
                (round(section_length / rod.get_attributes().total_length * rod.get_attributes().sim_points_number));

        // there cannot be more section sim points then overall sim points number
        // but what if we get rid of this check? as long as there are no rounding errors, this shouldn't be required
//        if (section_sim_points > sim_points_number - allocated_points) {
//            section_sim_points = sim_points_number - allocated_points;
//        }

        //rod.get_attributes().aTk = 12; //would that work??
        rod.add_to_alloc_pts(section_sim_points);
        rod.new_segment(section_sim_points, heat_transfer_coeff);

        ui->odcinekbar->setValue(rod.get_alloc_pts()); // I want that function to be deprecated
    }

    // It has to be another check, not *else* statement, bc it's checked **after** first *if* statement
    if (!rod.has_unalloc_pts()){
        ui->wprowotbut->setEnabled(true);
        ui->doplikubut->setEnabled(true);
    }

}

void MainWindow::on_wprowotbut_clicked()
{
    auto* Tablica = new float [rod.get_attributes().sim_points_number];
    Tablica = rod.calculate_temperature_distribution();

    GraphWindow okno;
    okno.setModal(true);
    okno.przekaz(Tablica, rod.get_attributes().sim_points_number, rod.get_attributes().total_length);
    okno.exec();
}


void MainWindow::on_czyscbut_clicked()
{
    rod.clear_segments();

    ui->wprowotbut->setDisabled(true);
    ui->odcinekbar->setValue(0);
}


void MainWindow::on_doplikubut_clicked()
{
    QString q_name = QFileDialog::getSaveFileName(this,tr("Save File"), "", tr("Text files (*.txt)"));
    ofstream o_file;
    o_file.open(std::string{q_name.toUtf8().constData()}.c_str());

    if(o_file.is_open() == true){ // probably not needed, but it's ok

        o_file<<"Rod length = " << rod.get_attributes().total_length <<endl;
        o_file<<"Simulation points = " << rod.get_attributes().sim_points_number<<endl;
        o_file<<"Section area = " << rod.get_attributes().section_area<<endl;
        o_file<<"Thermal conductivity coeffitient = " << rod.get_attributes().thermal_conduct_coeff<<endl;
        o_file<<"Environment temperature = " << rod.get_attributes().env_temperature<<endl;
        o_file<<"Starting point temperature = " << rod.get_attributes().starting_temperature<<endl;
        o_file<<"End point temperature = " << rod.get_attributes().aTk<<endl;
        o_file<<"Segmentation (segment length)_(segment heat transfer coeffitient) = "<<endl;

        // those 2 functions can be merged into one with touple or vector of pairs or any other way.
        vector<double> lengths = rod.get_all_seg_lenghts();
        vector<double> ht_coeffs = rod.get_all_seg_ht_coeffs();

        for (unsigned long i = 0; i < lengths.size(); ++i) {
            o_file << lengths.at(i) << ' ';
            o_file << ht_coeffs.at(i) << ' ';
        }

        o_file.close();
    }
}


void MainWindow::on_wczytajbut_clicked()
{
    QString Qsciezka = QFileDialog::getOpenFileName(this,tr("wybierz plik") ,"", tr("Plik tekstowy (*.txt)"));
    std::string sciezka = Qsciezka.toUtf8().constData();
    if (sciezka == "") return;

    ifstream plik;
    plik.open(sciezka.c_str());
    if(!plik.is_open()){
        QMessageBox::warning(this, "Błąd","Nie można otworzyć pliku");
        return;
    }

    double section_length, alfa;
    int section_sim_points; //ilosc punktow

    // I probably need to specify default values in order to make it work
    // That was an earlier idea that attr will be clean. Now I copy everything from rod so nothing is lost
    // But I wonder whether I am allowed to copy them this way.
    // If that's OK, I might change all references to rod.getattributes to attr
    RodAttributes attr = rod.get_attributes();

    attr.allocated_points = 0;

    char temp;

    do{plik >> temp;} while (temp != '=');
    plik >> attr.total_length;

    do{plik >> temp;} while (temp != '=');
    plik >> attr.sim_points_number;

    do{plik >> temp;} while (temp != '=');
    plik >> attr.section_area;

    do{plik >> temp;} while (temp != '=');
    plik >> attr.thermal_conduct_coeff;

    do{plik >> temp;} while (temp != '=');
    plik >> attr.env_temperature;

    do{plik >> temp;} while (temp != '=');
    plik >> attr.starting_temperature;

    do{plik >> temp;} while (temp != '=');
    plik >> attr.aTk;

    do{plik >> temp;} while (temp != '=');


    rod.set_all_attributes(attr);
    rod.clear_segments();

    ui->odcinekbar->setValue(0);
    ui->wprowotbut->setDisabled(true);

    while(rod.has_unalloc_pts() && !plik.eof()){

        plik>>section_length;
        plik>>alfa;

        section_sim_points = int(round(section_length / rod.get_attributes().total_length * rod.get_attributes().sim_points_number));

        // Probably useful but for now it's disabled. Maybe the best way is to make sure that allocated points are not bigger than all points just before calculations?
        //if(section_sim_points>sim_points_number-suma) section_sim_points=sim_points_number-suma;

        rod.add_to_alloc_pts(section_sim_points);

        rod.new_segment(section_sim_points, alfa);
    }

    plik.close();

    ui->odcinekbar->setValue(rod.get_attributes().allocated_points);

    ui->total_length_box->setValue( rod.get_attributes().total_length);
    ui->n->setValue(                rod.get_attributes().sim_points_number);
    ui->pole->setValue(             rod.get_attributes().section_area);
    ui->k->setValue(                rod.get_attributes().thermal_conduct_coeff);
    ui->Tp->setValue(               rod.get_attributes().starting_temperature);
    ui->Tk->setValue(               rod.get_attributes().aTk);
    ui->Tot->setValue(              rod.get_attributes().env_temperature);

    if(rod.attributes_correct()){

        ui->Podzial->setEnabled(true);
        ui->wprowotbut->setDisabled(true);
        ui->doplikubut->setDisabled(true);

        ui->odcinekbar->setMaximum(rod.get_attributes().sim_points_number);
        ui->sprawdzlab->setText("Dane prawidłowe");
    }else{
        ui->sprawdzlab->setText("Dane nieprawidłowe");
    }

    if(!rod.has_unalloc_pts()){
        ui->wprowotbut->setEnabled(true);
        ui->doplikubut->setEnabled(true);
    }
}



