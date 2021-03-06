#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <png.h>



void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    create_img(grad, im->height, im->width);


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

            int delta_x = ( deltas[0]*deltas[0] ) + (deltas[1]*deltas[1]) + (deltas[2]*deltas[2]);
            int delta_y = ( deltas[3]*deltas[3] ) + (deltas[4]*deltas[4]) + (deltas[5]*deltas[5]);

            /* delta_x *= delta_x; */
            /* delta_y *= delta_y; */

            /* printf("h: %d, w: %d, Rx: %d, Gx: %d, Bx: %d, Ry: %d, Gy: %d By: %d\n", */
                    /* h, w, deltas[0], deltas[1],deltas[2],deltas[3],deltas[4],deltas[5]); */

            uint8_t energy = (uint8_t) (sqrt((delta_x + delta_y))/10);

            /* printf("h: %d, w: %d, top_h: %d, bottom_h: %d, left_w: %d, right_w: %d sum: %d\n, energy: %d\n", */
                    /* h, w, top_h, bottom_h, left_w, right_w, delta_x + delta_y, energy); */
            /* printf("\n"); */

            set_pixel(*grad, h, w, energy, energy, energy);
        }
    }
}


void dynamic_seam(struct rgb_img *grad, double **best_arr){
   int height = grad->height;
   int width = grad->width;
   double *newarray = (double*) malloc(height * width * sizeof(double));
   // getting information from grad
   for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            newarray[i*width + j] = (double) get_pixel(grad, i, j, 0);
        }  }
   for (int i = 1; i < height; i++){
      for (int j = 0; j < width; j++){
         if (j == 0){
            newarray[i*width + j] += (newarray[(i-1)*width + j] < newarray[(i-1)*width + j+1] 
            ? newarray[(i-1)*width + j] : newarray[(i-1)*width + j+1]);
         }
         else if (j == (width - 1)){
            newarray[i*width + j] += (newarray[(i-1)*width + j-1] < newarray[(i-1)*width + j] 
            ? newarray[(i-1)*width + j-1] : newarray[(i-1)*width + j]);
         }
         else{
            int min = 100000000;
            for (int z = -1; z < 2; z++){
               if (newarray[(i-1)*width + j-z] < min){
                  min = newarray[(i-1)*width + j-z];
               }
            }
            newarray[i*width + j] += min;
         }
      }
   }
   *best_arr = newarray;
}





int threemin(double a, double b, double c) {
    double t[3] = {a, b, c};
    double min = a;
    int min_idx = 0;
    for (int i = 1; i < 3; i ++) {
        if (t[i] < min) {
            min_idx = i;
            min = t[i];
        }
    }
    return min_idx;
}

double double_get(double *arr, int width, int y, int x) {
    return (arr)[y*width+x];
}



void recover_path(double *best, int height, int width, int **path) {
    // get first min
    // then look at 3 or 2 (at edge) above for min there
    // build path

    /* int tpath[height]; */
    double min = double_get(best, width, height-1, 0);
    int min_idx = 0;
    int tmp_min_idx = 0;
    double a, b, c;
    int* tpath = (int*) malloc(height*sizeof(int));
    for (int w = 0; w < width; w++) {
        if (double_get(best, width, height-1, w) < min) {
            min = double_get(best, width, height-1, w);
            min_idx = w;
        }
    }

    /* printf("min_idx: %d\n", min_idx); */

    for (int h = height-1; h > 0; h--) {
        /* printf("outside_loop: h: %d\n", h); */
        tpath[h] = min_idx;
        /* printf("tpath[%d]: %d\n", h, tpath[h]); */
        if (min_idx == 0) {
            /* printf("0! "); */
            b = double_get(best, width, h-1, min_idx);
            c = double_get(best, width, h-1, min_idx+1);
            if (b < c) {
                tmp_min_idx = min_idx;
            } else {
                tmp_min_idx = min_idx + 1;
            }

        } else if (min_idx == width-1) {
            /* printf("width! "); */
            a = double_get(best, width, h-1, min_idx-1);
            b = double_get(best, width, h-1, min_idx);
            if (b < a) {
                tmp_min_idx = min_idx;
            } else {
                tmp_min_idx = min_idx + 1;
            }
        } else {
            /* printf("3! "); */
            a = double_get(best, width, h-1, min_idx-1);
            b = double_get(best, width, h-1, min_idx);
            c = double_get(best, width, h-1, min_idx+1);
            /* printf("a: %f, b: %f, c: %f\n", a, b, c); */
            tmp_min_idx = min_idx + threemin(a, b, c) -1; // (0, 1, 2) - 1 = (-1, 0, 1)
        }
        min_idx = tmp_min_idx;
    }

    if (min_idx == 0) {
        /* printf("0! "); */
        b = double_get(best, width, 0, min_idx);
        c = double_get(best, width, 0, min_idx+1);
        if (b < c) {
            tmp_min_idx = min_idx;
        } else {
            tmp_min_idx = min_idx + 1;
        }

    } else if (min_idx == width-1) {
        /* printf("width! "); */
        a = double_get(best, width, 0, min_idx-1);
        b = double_get(best, width, 0, min_idx);
        if (b < a) {
            tmp_min_idx = min_idx;
        } else {
            tmp_min_idx = min_idx + 1;
        }
    } else {
        /* printf("3! "); */
        a = double_get(best, width, 0, min_idx-1);
        b = double_get(best, width, 0, min_idx);
        c = double_get(best, width, 0, min_idx+1);
        /* printf("a: %f, b: %f, c: %f\n", a, b, c); */
        tmp_min_idx = min_idx + threemin(a, b, c) -1; // (0, 1, 2) - 1 = (-1, 0, 1)
    }
    tpath[0] = tmp_min_idx;

    /* for (int i = 0; i < 5; i++) { */
    /*     printf("%d, ", tpath[i]); */
    /* } */
    /* printf("\n"); */

    *path = tpath;


}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    int newheight = src->height;
    int newwidth = src->width - 1;
    int flag = 0;
    create_img(dest, newheight, newwidth);


    for (int i = 0; i < newheight; i++){
        flag = 0;
        for (int j = 0; j < newwidth + 1; j++)
            if (j!= path[i]){
                /* printf("(%d, %d), ", i, j); */
                int r = get_pixel(src, i, j, 0);
                int g = get_pixel(src, i, j, 1);
                int b = get_pixel(src, i, j, 2);
                set_pixel(*dest, i, j- flag, r, g, b);
            } else {
                flag ++;
            }
    /* printf("\n"); */
    }
}
