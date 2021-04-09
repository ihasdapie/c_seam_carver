#include "seamcarving.h"
#include "c_img.h"
#include "stdio.h"
#include "string.h"

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


void print_seams(struct rgb_img *im, double *best_arr){
    int height = im->height;
    int width = im->width;
    /* for(int i = 0; i < height; i++){ */
    /*     for(int j = 0; j < width; j++){ */
    /*         printf("%f ", best_arr[3 * (i*(im->width) + j) + 1]); */
    /*     } */
    /* printf("\n"); */    
    for (int i = 0; i < im->width * im->height; i++) {
        printf("%f, ", best_arr[i]);
    }
}

void print_img_dim(struct rgb_img* im) {
    printf("w: %d, h: %d\n", im->width, im->height);
}

int main (void) {
    struct rgb_img* grad;
    struct rgb_img* im;
    struct rgb_img* dest;
    double* best_arr;
    int* path;

    read_in_img(&im, "spirited_away.bin");

    for (int i = 0; i < 5; i ++ ) {
        print_img_dim(im);
        calc_energy(im, &grad);
        dynamic_seam(grad, &best_arr);
        recover_path(best_arr, im->height, im->width, &path);
        remove_seam(im, &dest, path);

        im->raster = (uint8_t* ) realloc(im->raster, (3*dest->height*dest->width));
        memcpy(im->raster, dest->raster, (dest->height*dest->width*3*sizeof(*dest->raster)));
        im->height = dest->height;  //alternatively we could just decrement width but this is clearer
        im->width = dest->width;

        destroy_image(dest);  // create_img always mallocs so we have to free it everytime
        destroy_image(grad);
        free(best_arr);
        free(path);
    }    

    write_img(im, "resize_6x5.bin");
    destroy_image(im);

    /* print_grad(grad); */

    /* printf("seam: \n"); */
    /* print_seams(im, best_arr); */

    /* printf("\n"); */
    /* for (int i = 0; i < im->height; i++) { */
    /*     printf("%d, ", (path)[i]); */
    /* } */
    /* printf("\n"); */
    /* print_img(dest); */



    /* write_img(dest, "6x5_new.bin"); */

}



