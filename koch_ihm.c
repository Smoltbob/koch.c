/*
 * File : koch_ihm.c
 * Description : parsing and initialization of parameters - koch snowflake
 */
#include "koch_fonctions.h"
/* TODO : sécuriser les entrées, module SVG */

/* Gets or asks the paramaters in command line and saves them in a structured
 * type.
 * Arguments by order :
 * 1) Size of the first segment. We deduce the size of the image from it.
 * 2) Number of segments for the polygon (3 = triangle, etc)
 * 3) Numver of iterations = "levels" of fractal
 * 4) Foreground color in 0x**RRGGBB hex format
 * 5) Background color in 0x**RRGGBB hex format
 * 6) Output svg file name
 * 7) Wether or not we generate one image per iteration (usefull for gifs)
 *    default : false.
 * 
 * If the arguments aren't properly provided we have to ask for them.
 * */
void init_parameters(struct parameters *parameters, int argc, char *argv[])
{
    /* We must have 7 or 8 arguments to continue (= everything +/- -"all" */
    if (argc == 7 || argc == 8)
    {
        parameters->segment_length = atoi(argv[1]);
        parameters->nb_sides = atoi(argv[2]);
        /* La taille de l'image vaut le rayon du polygone + la hauteur des
         * triangles ajoutés à l'itération 1.
         * Diametre d'un polyone régulier = tailleseg/sin(Pi/nbcote)
         * Hauteur du triangle isocèle de côté c = sqrt(3(c^2/4)), ici
         * on divise par 2 pour séparer un peu la figure du bord */
        parameters->image_size = (atoi(argv[1])/sin(PI/atoi(argv[2]))
                + sqrt(pow(atoi(argv[1]), 2) * 3/4));
        parameters->nb_iterations = atoi(argv[3]);
        parameters->fg_color = strtol(argv[4], NULL, 16);
        parameters->bg_color = strtol(argv[5], NULL, 16);
        parameters->outfile = argv[6];
        parameters->all_images = ((argc == 8 
                    && strcmp(argv[7],"all\n") != 0) ? true : false);
    }
    else
    {
        /* Initialisation d'un tampon pour la lecture de stdin. On fait un
         * malloc pour qu'il soit lisible même en dehors de la fonction 
         * TODO : changer ce fonctionnement ? */
        char *tmp = malloc(40 * sizeof(char));
        /* Initialisation de la variable utilisée pour l'option all */
        char choix = 0;
        printf("All the parameters haven't been specified\n");
        printf("Length of segment ?\n");
        scanf("%"PRIu32, &parameters->segment_length);
        printf("Number of segments\n");
        scanf("%"PRIu32, &parameters->nb_sides);
        parameters->image_size = (
                parameters->segment_length/sin(PI/parameters->nb_sides)
                + sqrt(pow(parameters->segment_length,2) * 3/4));
        printf("Number of iterations ?\n");
        scanf("%"PRIu32, &parameters->nb_iterations);
        printf("Line color ?\n");
        scanf("%s", tmp);
        parameters->fg_color = strtol(tmp, NULL, 16);
        printf("Background color ?\n");
        scanf("%s", tmp);
        parameters->bg_color = strtol(tmp, NULL, 16);
        printf("File name ?\n");
        scanf("%s", tmp);
        parameters->outfile = tmp;
        printf("Save all the steps [y/N] ?\n");
        scanf(" %c", &choix);
        /* 121 = 'y' ascii code */
        parameters->all_images = choix == 121 ? true : false;
    }
}

/* Prints the linked list */
void show_koch_list(struct list *koch)
{
    /* Notre pointeur sur la liste */
    struct list *point = koch;
    /* Index permet de compter les points */
    uint64_t index = 0;
    /* On boucle sur la liste simplement chaînée circulaire */
    do{
        printf("Point %" PRIu64 ": x = %f, y = %f\n",
                index, point->x, point->y);
        point = point->next;
        index++;
    }while(point != koch);
}

/* Shows the saved parameters */
void show_parameters(const struct parameters *parameters)
{
    printf("Size of segment : %d\n", parameters->segment_length);
    printf("Number of segmentsi : %d\n", parameters->nb_sides);
    printf("Image size : %d\n", parameters->image_size);
    printf("Number of iterations : %d\n", parameters->nb_iterations);
    printf("Line color : %d\n", parameters->fg_color);
    printf("Background color : %d\n", parameters->bg_color);
    printf("File name : %s\n", parameters->outfile);
}

/* Frees a struct parameters variable */
void free_parameters(struct parameters *parameters)
{
    free(parameters->outfile);
    free(parameters);
}
