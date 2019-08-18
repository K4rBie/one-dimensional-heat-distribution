#ifndef ROD_H
#define ROD_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;


struct Segment{
    int sim_points;
    double heat_transfer_coeff;
    Segment *next;
};

struct RodAttributes {
    double total_length;
    double section_area;
    double thermal_conduct_coeff;
    double starting_temperature;
    double aTk;
    double env_temperature;

    int sim_points_number;
    int allocated_points;
};

class Rod {
public:
    Rod();
    void new_segment(int sim_points, double alpha);
    void clear_segments();
    void set_all_attributes(const RodAttributes& new_attributes);
    RodAttributes& get_attributes();
    int& get_alloc_pts(); // I hope I can deprecate that function at some point
    void add_to_alloc_pts(const int& newly_allocated);
    Segment* give_head(); // to be deprecated
    bool attributes_correct();
    bool has_unalloc_pts();
    std::vector<double> get_all_seg_lenghts();
    std::vector<double> get_all_seg_ht_coeffs();


private:
    Segment* last_tail;
    Segment* head;
    Segment* tail;

    RodAttributes attributes;
};

Segment *pret(double l, int n);

double * obliczenia(double l, int n, double A, double k, double Ta, double Tb, double Tot, Segment *test);
void dopliku();
void zpliku();
void drukuj(double **Tab, int n);
double **odwrotna(double **Tab, int n);
void czysctab(double **Tab, int n);


#endif // OBLICZENIA_H

