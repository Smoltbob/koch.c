/*
 * Fichier : koch_main.c
 * Description : Trace de fractales geometriques - flocon de koch - programme principal
 */

#include <stdlib.h> 
#include "koch_fonctions.h"
#include "koch_ihm.h"
#include "create_image.h"

int main(int argc, char **argv)
{
    /* Lecture des paramètres */
    struct parameters parameters;
    printf("Utilisation : ./koch taille_seg nb_cotes");
    printf(" nb_iter couleur_fg couleur_bg nom_fichier all\n\n");
    init_parameters(&parameters, argc, argv);
    /* On affiche les paramètres pour les avoir en rappel (pratique pour
     * expérimenter */
    show_parameters(&parameters);
    uint32_t i = parameters.nb_iterations;
    /* Initialisation de deux listes : pic pour l'image et flocons pour les
     * points de la fractale */
    uint32_t taille = parameters.image_size;
    struct list *flocons = NULL; 
    uint32_t *pic = NULL;
    /* On génère les images en commençant par la plus itérée puis on remonte
     * jusqu'au niveau 1 ssi on a choisi "all".
     * Ca nous force à regénérer le flocon entier à chaque fois
     * TODO : changer ça ? */
    do{
        printf("Init pic\n");
        init_picture(&pic, taille, parameters.bg_color);
        printf("Init koch\n");
        /* Test d'affichage du triangle */
        init_koch(&flocons, taille, parameters.segment_length,
                parameters.nb_sides);
        printf("Generer\n");
        generer_koch(flocons, i - 1);
        printf("Render\n");
        render_image_bresenham(pic, flocons, taille, parameters.fg_color);
        /* Génération du fichier */
        char index[80];
        sprintf(index, "%s_%u.ppm", parameters.outfile, i - 1);
        printf("Create image\n");
        create_image_ppm(pic, taille, taille, index);
        printf("%u\n", i);
        i--;
        free_koch(flocons);
        free(pic);
    }while(i > 0 && parameters.all_images == true);
    return EXIT_SUCCESS;
}
