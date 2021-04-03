#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    create_img(grad, im->height, im->width);

    int height = im->height;

    for (int h = 0; h < im->height; h++){
        for (int w = 0; w < im->width; w++) {
            int right_w = w+1;
            int left_w = w-1;
            int top_h = h-1;
            int bottom_h = h+1;

            // perform wraparounds
            if (h == 0) {
                top_h = im->height-1;
            } if (w == 0) {
                left_w = im->width-1;
            } 

            if (h == im->height-1) {
                bottom_h = 0;
            } if (w == im->width-1) {
                right_w = 0;
            }

            int deltas[6]; // [Rx, Gx, Bx, Ry, Gy, By]

            for (int i = 0; i < 3; i++) {
                deltas[i] = (int) get_pixel(im, h, left_w, i) - (int) get_pixel(im, h, right_w, i);
            }

            for (int i = 3; i < 6; i++) { 
                deltas[i] = (int) get_pixel(im, top_h, w, i-3) - (int) get_pixel(im, bottom_h, w, i-3);
            }

            int delta_x = deltas[0] + deltas[1] + deltas[2];
            int delta_y = deltas[3] + deltas[4] + deltas[5];

            delta_x *= delta_x;
            delta_y *= delta_y;

            /* printf("h: %d, w: %d, Rx: %d, Gx: %d, Bx: %d, Ry: %d, Gy: %d By: %d\n", */
            /*         h, w, deltas[0], deltas[1],deltas[2],deltas[3],deltas[4],deltas[5]); */

            uint8_t energy = (uint8_t) (sqrt((delta_x + delta_y))/10);

            /* printf("h: %d, w: %d, top_h: %d, bottom_h: %d, left_w: %d, right_w: %d sum: %d\n", */
            /*         h, w, top_h, bottom_h, left_w, right_w, delta_x + delta_y); */
            /* printf("\n"); */




            set_pixel(*grad, h, w, energy, energy, energy);
        }
    }
}


void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    // not simply enough to pick min element, but the path also has to be 
    // continous! i.e. a feasible path.
}


void recover_path(double *best, int height, int width, int **path) {

}



















void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path);


