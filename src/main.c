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

    
    read_in_img(&im, "3x4.bin");
    print_img(im);

    calc_energy(im, &grad);

    print_grad(grad);
}



