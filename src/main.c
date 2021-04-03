#include "seamcarving.h"
#include "c_img.h"
#include "stdio.h"

void print_img(struct rgb_img *im){
    int height = im->height;
    int width = im->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%d ", get_pixel(im, i, j, 0));
            printf("%d ", get_pixel(im, i, j, 1));
            printf("%d ", get_pixel(im, i, j, 2));
            printf("\t");
        }
    printf("\n");    
    }
}

int main (void) {
    struct rgb_img* grad;
    struct rgb_img* im;

    
    read_in_img(&im, "spirited_away.bin");
    /* print_img(im); */

    calc_energy(im, &grad);

    /* print_grad(grad); */

    write_img(grad, "sprited_away_energy.bin");


    double best_arr[] = {24.0, 22.0, 30.0, 15.0, 18.0, 19.0,
                        34.0, 45.0, 30.0, 38.0, 25.0, 33.0,
                        45.0, 43.0, 52.0, 38.0, 46.0, 39.0,
                        56.0, 58.0, 55.0, 66.0, 57.0, 60.0,
                        73.0, 72.0, 62.0, 82.0, 77.0, 76.0};


    int* path;

    recover_path(best_arr, 5, 6, &path);

    for (int i = 0; i < 5; i++) {
        printf("%d, ", (path)[i]);
    }
    printf("\n");


    









}



