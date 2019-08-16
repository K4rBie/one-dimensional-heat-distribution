#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oknowynikow.h"
#include "QFileDialog"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    last_segment(nullptr), head(nullptr), tail(nullptr), allocated_points(0), // w przyszłości zbędne
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
    // set_all()
    total_length = ui->total_length_box->value();
    sim_points_number = ui->n->value();
    section_area = ui->pole->value();
    thermal_conduct_coeff = ui->k->value();
    starting_temperature = ui->Tp->value(); // temperatura początku i końca powinna być liczona jako następna i poprzednia
    aTk=ui->Tk->value();
    env_temperature=ui->Tot->value();

    // simple check
    if(total_length>0 && sim_points_number>0 && section_area>0 && thermal_conduct_coeff>0){

        ui->Podzial->setEnabled(true);
        ui->wprowotbut->setDisabled(true);
        ui->doplikubut->setDisabled(true);

        ui->odcinekbar->setMaximum(sim_points_number);
        ui->sprawdzlab->setText("Dane prawidłowe");
    }else{
        ui->sprawdzlab->setText("Dane nieprawidłowe");
    }
}

void MainWindow::on_dodajbut_clicked(){

    if (allocated_points < sim_points_number){

        double section_length = ui->lodc->value();
        double heat_transfer_coeff = ui->alfa->value();
        int section_sim_points = static_cast<int>(round(section_length / total_length * sim_points_number));

        // there cannot be more section sim points then overall sim points number
        if (section_sim_points > sim_points_number - allocated_points) {
            section_sim_points = sim_points_number - allocated_points;
        }

        allocated_points += section_sim_points;

        ui->odcinekbar->setValue(allocated_points);

        // Rod.new_segment (section_sim_points, heat_transfer_coeff)
        tail = last_segment;
        last_segment = new Segment;
        last_segment->sim_points = section_sim_points;
        last_segment->heat_transfer_coeff = heat_transfer_coeff;
        last_segment->next=nullptr;

        // to powinna być część konstruktora Roda
        if(tail == nullptr){ //true if last segment was nullptr at the start of function.
            head = last_segment; // there's only one element now.
        } else (tail->next = last_segment);
    }
    if (allocated_points >= sim_points_number){ // to nie może być else bo on musi sprawdzić to po dodaniu punktów do sumy
        ui->wprowotbut->setEnabled(true);
        ui->doplikubut->setEnabled(true);
    }

}

void MainWindow::on_wprowotbut_clicked()
{
    double *Tablica;
    Tablica = new double [sim_points_number];
    Tablica = obliczenia(total_length, sim_points_number, section_area, thermal_conduct_coeff, starting_temperature, aTk, env_temperature, head);

    OknoWynikow okno;
    okno.setModal(true);
    okno.przekaz(Tablica, sim_points_number, total_length);
    okno.exec();
}

void MainWindow::on_czyscbut_clicked()
{
    //Rod.clear()
    Segment* temp;
    while(head != nullptr){
        temp = head;
        head=head->next;
        delete temp;
    }

    allocated_points=0;
    head=nullptr;
    last_segment=nullptr;
    tail=nullptr;

    ui->wprowotbut->setDisabled(true);
    ui->odcinekbar->setValue(0);
}


void MainWindow::on_doplikubut_clicked()
{
    QString Qnazwa= QFileDialog::getSaveFileName(this,tr("wybierz plik") ,"", tr("Plik tekstowy (*.txt)"));
    ofstream o_file;
    std::string nazwa = Qnazwa.toUtf8().constData();
    o_file.open(nazwa.c_str());
    if(o_file.is_open() == true){

        double lp;
        double segment_sim_points;
        double alfa;

        o_file<<"Długość pręta = "<<total_length <<endl;

        o_file<<"Ilość punktów obliczeń = "<<sim_points_number<<endl;

        o_file<<"Pole przekroju pręta = "<<section_area<<endl;

        o_file<<"Współczynnik przewodzenia ciepła = "<<thermal_conduct_coeff<<endl;

        o_file<<"Temperatura otoczenia = "<<env_temperature<<endl;

        o_file<<"Temperatura pierwszego końca = "<<starting_temperature<<endl;

        o_file<<"Temperatura drugiego końca = "<<aTk<<endl;

        o_file<<"podział (długość fragmentu drutu)_(współczynnik przejmowania ciepła) = "<<endl;

        //Rod.give_head()
        while(head!=nullptr){

            segment_sim_points = head->sim_points;
            lp = segment_sim_points * total_length / sim_points_number;
            o_file<<lp<<' ';

            alfa=head->heat_transfer_coeff;
            o_file<<alfa<<' '<<endl;

            head=head->next;
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

    char pom;

    double lp, alfa;
    int g; //ilosc punktow
    int suma=0;


    do{plik>>pom;}while(pom!='=');
    plik>>total_length;

    do{plik>>pom;}while(pom!='=');
    plik>>sim_points_number;

    do{plik>>pom;}while(pom!='=');
    plik>>section_area;

    do{plik>>pom;}while(pom!='=');
    plik>>thermal_conduct_coeff;

    do{plik>>pom;}while(pom!='=');
    plik>>env_temperature;

    do{plik>>pom;}while(pom!='=');
    plik>>starting_temperature;

    do{plik>>pom;}while(pom!='=');
    plik>>aTk;

    do{plik>>pom;}while(pom!='=');

    Segment *tmp; //Rod.clear()
    while(head!=nullptr){
        tmp=head;
        head=head->next;
        delete tmp;
    }

    suma=0;
    head=nullptr;
    last_segment=nullptr;
    tail=nullptr;

    ui->odcinekbar->setValue(0);
    ui->wprowotbut->setDisabled(true);

    while(suma<sim_points_number && plik.eof()==false){

        plik>>lp;
        plik>>alfa;

        g = int(round(lp/total_length*double(sim_points_number)));
        if(g>sim_points_number-suma) g=sim_points_number-suma;
        suma+=g;

        //Rod.new_segment();
        tail=last_segment;
        last_segment=new Segment;
        last_segment->sim_points=g;
        last_segment->heat_transfer_coeff=alfa;
        last_segment->next=nullptr;

        if(tail==nullptr){
            head=last_segment;
        }else(tail->next=last_segment);
    }

    ui->odcinekbar->setValue(suma);

    ui->total_length_box->setValue(total_length);
    ui->n->setValue(sim_points_number);
    ui->pole->setValue(section_area);
    ui->k->setValue(thermal_conduct_coeff);
    ui->Tp->setValue(starting_temperature);
    ui->Tk->setValue(aTk);
    ui->Tot->setValue(env_temperature);

    if(total_length>0 && sim_points_number>0 && section_area>0 && thermal_conduct_coeff>0){

        ui->Podzial->setEnabled(true);
        ui->wprowotbut->setDisabled(true);
        ui->doplikubut->setDisabled(true);

        ui->odcinekbar->setMaximum(sim_points_number);
        ui->sprawdzlab->setText("Dane prawidłowe");
    }else{
        ui->sprawdzlab->setText("Dane nieprawidłowe");
    }

    if(suma>=sim_points_number){
        ui->wprowotbut->setEnabled(true);
        ui->doplikubut->setEnabled(true);
    }

    plik.close();
}



