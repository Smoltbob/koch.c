/*
 * File : create_image.c
 * Description : creation of a ppm file from an uint32_t array representing
 * an image.
 */
#include "create_image.h"
#include <stdio.h>

struct color
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

/* Takes a color coded as xxRRGGBB in an uint32_t and returns the individual
 * components */
struct color int32tocolor(uint32_t number)
{
    struct color res;
    /* We use masks to extract RGB components */
    res.R = (number >> 16) & 0xFF;
    res.G = (number >> 8) & 0xFF;
    res.B = number & 0xFF;
    return res;
}

/* Generates a ppm (P6: color, binary) image of size x*y from an array */
void create_image_ppm(uint32_t *pic, int32_t size_x, int32_t size_y, char *filename)
{
    /* Creating file */
    FILE *fic = fopen(filename, "w");
    /* Writing ppm header */
    fprintf(fic, "P6\n");
    fprintf(fic, "%d %d\n", size_x, size_y);
    fprintf(fic, "%i\n", 255);
    /* Generating image */
    uint32_t *i32pixel = pic;
    /* We loop through all the pixels */
    for (int32_t i = 0; i < size_x * size_y; i++)
    {
        struct color pixel = int32tocolor(*i32pixel);
        uint8_t colors[] = {pixel.R, pixel.G, pixel.B};
        fwrite(colors, 1, 3, fic);
        i32pixel++;
    }
    fclose(fic);
}
