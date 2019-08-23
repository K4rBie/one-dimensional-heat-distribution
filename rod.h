#ifndef ROD_H
#define ROD_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;


struct Segment{
    unsigned int sim_points;
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

    unsigned int sim_points_number;
    unsigned int allocated_points;
};

class Rod {
public:
    Rod();
    void new_segment(int sim_points, double alpha);
    void clear_segments();
    void set_all_attributes(const RodAttributes& new_attributes);
    RodAttributes& get_attributes();
    unsigned int& get_alloc_pts(); // I hope I can deprecate that function at some point
    void add_to_alloc_pts(const int& newly_allocated);
    Segment* give_head(); // to be deprecated
    bool attributes_correct();
    bool has_unalloc_pts();
    std::vector<double> get_all_seg_lenghts();
    std::vector<double> get_all_seg_ht_coeffs();
    float *calculate_temperature_distribution(); // probably should be a vector


private:
    Segment* last_tail;
    Segment* head;
    Segment* tail;

    RodAttributes attributes;

    void invert_matrix(float **Tab, unsigned int n);
    void delete_matrix(float **Tab, unsigned int n);

};

Segment *pret(double l, int n); //??


#endif // OBLICZENIA_H

