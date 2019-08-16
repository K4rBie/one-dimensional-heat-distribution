#include "rod.h"
#include "oknowynikow.h"

void zpliku(){
    ifstream plik;
    char pom;
    string nazwa;

    double l;//=0.5; //długość pręta +
    int n;//=100; // ilość podziałów +
    double A;//=0.0000785; //pole przekroju +
    double k;//=500; //wps przewodzenia +
    double Ta;// = 500; //+
    double Tb;// = 500; //+
    double Tot;// = 300; //+

    Segment *aktualny, *glowa, *ogon;
    double lp, alfa;
    int g; //ilosc punktow
    int suma=0;

    cout<<"Wprowadz nazwe pliku, ktory chcesz otworzyc: ";
    getline(cin, nazwa);
    nazwa+=".txt";

    plik.open(nazwa.c_str());
    if(plik.is_open() == true){
        do{
           plik>>pom;
        }while(pom!='=');
        plik>>l;

        do{
           plik>>pom;
        }while(pom!='=');
        plik>>n;

        do{
           plik>>pom;
        }while(pom!='=');
        plik>>A;

        do{
           plik>>pom;
        }while(pom!='=');
        plik>>k;

        do{
           plik>>pom;
        }while(pom!='=');
        plik>>Tot;

        do{
           plik>>pom;
        }while(pom!='=');
        plik>>Ta;

        do{
           plik>>pom;
        }while(pom!='=');
        plik>>Tb;

            glowa = NULL;
            aktualny = NULL;

            do{
               plik>>pom;
            }while(pom!='=');

            while(suma<n && plik.eof()==false){


                plik>>lp;
                plik>>alfa;

                g = lp/l*n;
                if(g>n-suma) g=n-suma;
                suma+=g;

                ogon=aktualny;
                aktualny=new Segment;
                aktualny->sim_points=g;
                aktualny->heat_transfer_coeff=alfa;
                aktualny->next=NULL;

                if(ogon==NULL){
                    glowa=aktualny;
                }else(ogon->next=aktualny);
            }

        obliczenia(l, n, A, k, Ta, Tb, Tot, glowa);
        plik.close();
    }

}

void dopliku(){
    //zapisuje tablicę do pliku
}

double * obliczenia(double l, int n, double A, double k, double Ta, double Tb, double Tot, Segment *pret){
/**/
                    /*ZMIENNE*/
    double dx = (l/n); //długość odcinka
    double wsp2;
    double wsplp = -k*A/dx;
    double wspc;
    double alfa;
    double *TabB = new double [n];
    double *TabC = new double [n];
    int nx=0;
    int ny=0;
    int i, j;

                /*WYPEŁNIANIE TABLIC A I B*/
    double **TabA = new double *[n];
    for(i=0;i<n;i++){
        TabA[i] = new double[n];
    }

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            TabA[i][j]=0;
        }
    }

    while(pret!=NULL){
        ny=nx;
        nx+=pret->sim_points;
        alfa=pret->heat_transfer_coeff;
        wspc = ((double) 2*k*A/dx+alfa*2*sqrt(M_PI*A)*dx);
        wsp2 = 2*alfa*sqrt(M_PI*A)*dx*Tot;

        for(i=ny; i<nx; i++){
            TabB[i]=wsp2;                   //TABLICA B
            TabA[i][i]=wspc;
            if(i+1<n){
                TabA[i][i+1]=wsplp;
            }
            if(i-1>=0){
                TabA[i][i-1]=wsplp;
            }
        }
        pret=pret->next;
    }

    TabB[0]=Ta;
    TabB[n-1]=Tb;

    TabA[0][0]=1;
    TabA[0][1]=0;

    TabA[n-1][n-1]=1;
    TabA[n-1][n-2]=0;

    TabA=odwrotna(TabA, n);

    /*MNOŻENIE MACIERZY A I B*/

    for(i=0;i<n;i++){
        TabC[i]=0;
        for(j=0;j<n;j++){
            TabC[i]+=TabA[i][j]*TabB[j];
        }
    }

    czysctab(TabA, n);
    delete []TabB;

    return TabC;
}

void drukuj(double **Tab, int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<Tab[i][j]<<" ";
        }
        cout<<endl;
    }
}

void czysctab(double **Tab, int n){
    for(int i=0;i<n;i++){
        delete [] Tab[i];
    }
    delete [] Tab;
}

double **odwrotna(double **Tab, int n){
    int i, j, k;
    double d;

    double **a = new double *[n];
    for(i=0;i<n;i++){
        a[i] = new double[2*n];
    }

    for (i=0; i<n; i++){      //zerowanie tablicy
        for(j=0; j<2*n; j++){
            if (j == i+n){
                a[i][j] = 1;
            }else{
                a[i][j] = 0;
            }
        }
    }

    for(i=0;i<n;i++){           //nadpisywanie na wiekszej tablicy
        for(j=0;j<n;j++){
            a[i][j]=Tab[i][j];
        }
    }

    for (i = n-1; i>0; i--){
        if (a[i-1][0] < a[i][0])
            for (j=0; j<n*2; j++){
                d = a[i][j];
                a[i][j] = a[i-1][j];
                a[i-1][j] = d;
            }
    }

    for (i=0; i<n; i++){
        for (j=0; j<n; j++)
            if (j!=i){
                d = a[j][i] / a[i][i];
                for (k=0; k<n*2; k++)
                    a[j][k] -= a[i][k] * d;
            }
    }

    for (i = 0; i < n; i++){
        d = a[i][i];
        for (j = 0; j < n * 2; j++)
            a[i][j] = a[i][j] / d;
    }

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            Tab[i][j]=a[i][j+n];
        }
    }

    czysctab(a,n);

    return Tab;
}

Rod::Rod(): last_tail(nullptr), head(nullptr), tail(nullptr) {
    attributes.allocated_points = 0;
}

void Rod::new_segment(int sim_points, double alpha)
{
    last_tail = tail;

    tail = new Segment;
    tail->sim_points = sim_points;
    tail->heat_transfer_coeff = alpha;
    tail->next = nullptr;

    if (last_tail == nullptr) {
        head = tail;
    } else {
        last_tail->next = tail;
    }
}

void Rod::clear_segments()
{
    Segment* temp;
    while(head != nullptr){
        temp = head;
        head = head->next;
        delete temp;
    }

    attributes.allocated_points = 0;
    last_tail = nullptr; //head is already a nullptr
    tail = nullptr; // tbh, I don't know if I really need to do that, but I guess I do
}

void Rod::set_all_attributes(RodAttributes new_attributes)
{
    attributes = new_attributes; // it's not good, bc user still needs to know how to create RodAttributes
}

RodAttributes Rod::get_attributes()
{
    return attributes;
}

int Rod::get_alloc_pts()
{
    return attributes.allocated_points;
}

void Rod::add_to_alloc_pts(int newly_allocated)
{
    attributes.allocated_points += newly_allocated;
}

Segment* Rod::give_head()
{
    return head;
}
