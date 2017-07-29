/*
 * File : koch_main.c
 * Description : tracing geometric fractals - koch snowflake - main program
 */

#include <stdlib.h> 
#include "koch_fonctions.h"
#include "koch_ihm.h"
#include "create_image.h"

int main(int argc, char **argv)
{
    /* Persing parameters */
    struct parameters parameters;
    printf("Usage : ./koch <size> <number of sizes> <number of iterations> <foreground color> <background color> <file name> [all]\n\n");
    init_parameters(&parameters, argc, argv);
    show_parameters(&parameters);
    uint32_t i = parameters.nb_iterations;
    /* Two arrays : pic for the image and snowflake for the fractal points */
    uint32_t taille = parameters.image_size;
    struct list *snwoflake = NULL; 
    uint32_t *pic = NULL;
    /* We start by generating the image by starting at the highest iteration
     * and we go back to level 1 if the option "all" is enabled.
     * Makes the code smaller but forces us to regenerate the snowflake each
     * time. TODO change that ? */
    do{
        printf("Init pic\n");
        init_picture(&pic, taille, parameters.bg_color);
        printf("Init koch\n");
        /* Test d'affichage du triangle */
        init_koch(&snwoflake, taille, parameters.segment_length,
                parameters.nb_sides);
        printf("Genererate\n");
        generer_koch(snwoflake, i - 1);
        printf("Render\n");
        render_image_bresenham(pic, snwoflake, taille, parameters.fg_color);
        /* Génération du fichier */
        char index[80];
        sprintf(index, "%s_%u.ppm", parameters.outfile, i - 1);
        printf("Create image\n");
        create_image_ppm(pic, taille, taille, index);
        printf("%u\n", i);
        i--;
        free_koch(snwoflake);
        free(pic);
    }while(i > 0 && parameters.all_images == true);
    return EXIT_SUCCESS;
}
