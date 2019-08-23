#include "rod.h"
#include "graphwindow.h"

void Rod::delete_matrix(float **Tab, unsigned int n){
    for (unsigned int i = 0; i < n; i++){
        delete [] Tab[i];
    }
    delete [] Tab;
}

void Rod::invert_matrix(float **input_matrix, unsigned int size){ //or get_inverse_matrix?
    unsigned int i, j, k;
    float temp;

    // creating matrix twice the size of input. WHY?
    auto **a = new float *[size];
    auto **inversed = new float *[size];
    for (i = 0; i < size; i++){
        a[i] = new float[size];
        inversed[i] = new float[size];
    }

    // 1
    for (i = 0; i < size; i++){      //identity matrix
        for(j = 0; j < size; j++){
            if (j == i){
                inversed[i][j] = 1;
            } else {
                inversed[i][j] = 0;
            }
        }
    }


    // TBH, it should fail to sort all the rows, but does it even suppose to sort?? Can't remember now
    for (i = size-1; i>0; i--){
        if (input_matrix[i-1][0] < input_matrix[i][0])
            for (j=0; j<size; j++){ // switch places of rows. ALL rows, left and right half.
                temp = input_matrix[i][j];
                input_matrix[i][j] = input_matrix[i-1][j];
                input_matrix[i-1][j] = temp;

                temp = inversed[i][j];
                inversed[i][j] = inversed[i-1][j];
                inversed[i-1][j] = temp;
            }
    }

    // 4
    for (i=0; i<size; i++){
        for (j=0; j<size; j++) {
            if (j != i){
                temp = input_matrix[j][i] / input_matrix[i][i];
                for (k=0; k<size; k++) { // now it's all
                    input_matrix[j][k] -= input_matrix[i][k] * temp;
                    inversed[j][k] -= inversed[i][k] * temp;
                }
            }
        }
    }

    // 5
    for (i = 0; i < size; i++){ // podziel tak, że masz jedynki po przekątnej po lewej
        temp = input_matrix[i][i];
        for (j = 0; j < size; j++) {
            input_matrix[i][j] = input_matrix[i][j] / temp;
            inversed[i][j] = inversed[i][j] / temp;
        }
    }

    // 6 //that doesn't clean memory right way. maybe I should clear first dimension, too. Or use memcpy?
    for (i = 0; i<size; i++){
        delete [] input_matrix[i];
        input_matrix[i] = inversed[i];

//        for (j=0; j<size; j++){
//            input_matrix[i][j] = inversed[i][j];
//        }
    }

    //float **temp_matrix = input_matrix;
    //input_matrix = inversed; // would it work?
    //delete [] temp_matrix;

    //delete_matrix(temp_matrix, size); // even if so, this would need to change
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

void Rod::set_all_attributes(const RodAttributes &new_attributes)
{
    attributes = new_attributes; // it's not good, bc user still needs to know how to create RodAttributes
}

RodAttributes &Rod::get_attributes()
{
    return attributes;
}

unsigned int &Rod::get_alloc_pts()
{
    return attributes.allocated_points;
}

void Rod::add_to_alloc_pts(const int& newly_allocated) // does const ref have any sense on int??
{
    attributes.allocated_points += newly_allocated;
}

Segment* Rod::give_head()
{
    return head;
}

bool Rod::attributes_correct()
{
    return attributes.total_length > 0 &&
           attributes.sim_points_number > 0 &&
           attributes.section_area > 0 &&
           attributes.thermal_conduct_coeff > 0;
}

bool Rod::has_unalloc_pts()
{
    return attributes.sim_points_number > attributes.allocated_points;
}

std::vector<double> Rod::get_all_seg_lenghts()
{
    Segment* temp = head;
    std::vector<double> lengths;

    while(temp != nullptr){
        double length = temp->sim_points * attributes.total_length / attributes.sim_points_number;
        lengths.push_back(length);
        temp = temp->next;
    }

    return  lengths;
}

std::vector<double> Rod::get_all_seg_ht_coeffs()
{
    Segment* temp = head;
    std::vector<double> ht_coeffs;

    while(temp != nullptr){
        ht_coeffs.push_back(temp->heat_transfer_coeff);
        temp = temp->next;
    }

    return  ht_coeffs;
}

float *Rod::calculate_temperature_distribution()
{
    const unsigned int& sim_points = attributes.sim_points_number;
        /*ZMIENNE*/
    double dx = attributes.total_length / sim_points; // smallest calculation length
    float coeff_ht;
    float coeff_dx = static_cast<float>(- attributes.thermal_conduct_coeff * attributes.section_area / dx);
    float coeff_en;
    float *TabB = new float [sim_points]; // this can be a problem if over 34000 I guess
    float *TabC = new float [sim_points];
    unsigned int nx = 0;
    unsigned int ny = 0;
    unsigned int i, j;
    Segment* iter = give_head();

    /*WYPEŁNIANIE TABLIC A I B*/
    float **TabA = new float* [sim_points];
    for (i = 0; i < sim_points; i++){
        TabA[i] = new float[sim_points];
    }

    for (i = 0; i < sim_points; i++){
        for (j = 0; j < sim_points; j++){
            TabA[i][j] = 0;
        }
    }

    while (iter != nullptr){
        ny = nx;
        nx += iter->sim_points;

        coeff_en = static_cast<float>(
                    2 * attributes.thermal_conduct_coeff * attributes.section_area / dx +
                    2 * iter->heat_transfer_coeff * sqrt(M_PI * attributes.section_area) * dx);
        coeff_ht = static_cast<float>(
                    2 * iter->heat_transfer_coeff * sqrt(M_PI * attributes.section_area) * dx *
                    attributes.env_temperature);

        for (i = ny; i < nx; i++){
            TabB[i] = coeff_ht;                   //TABLICA B
            TabA[i][i] = coeff_en;
            if (i+1 < sim_points){
                TabA[i][i+1] = coeff_dx;
            }
            if(i != 0){
                TabA[i][i-1] = coeff_dx;
            }
        }
        iter = iter->next;
    }

    TabB[0] = static_cast<float>(attributes.starting_temperature);
    TabB[sim_points - 1] = static_cast<float>(attributes.aTk);


    TabA[0][0] = 1;
    TabA[0][1] = 0 ;

    TabA[sim_points - 1][sim_points - 1] = 1;
    TabA[sim_points - 1][sim_points - 2] = 0;

    invert_matrix(TabA, sim_points);

    /*MNOŻENIE MACIERZY A I B*/

    for (i=0; i<sim_points; i++){
        TabC[i] = 0;
        for(j = 0; j < attributes.sim_points_number; j++){
            TabC[i]+=TabA[i][j]*TabB[j];
        }
    }

    delete_matrix(TabA, sim_points);
    delete []TabB;

    return TabC;
}
