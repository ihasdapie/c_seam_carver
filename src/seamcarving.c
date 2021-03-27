#include "c_img.h"
#include "seamcarving.h"



void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

}
void dynamic_seam(struct rgb_img *grad, double **best_arr) {
     // not simply enough to pick min element, but the path also has to be 
     // continous! i.e. a feasible path.
}


void recover_path(double *best, int height, int width, int **path);


void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path);


