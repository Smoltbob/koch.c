/*
 * Fichier : create_image.c
 * Description : Creation d'un fichier ppm a partir d'une image sous la forme
 *               d'un tableau uint32_t *
 */
#include "create_image.h"
#include <stdio.h>

struct couleur
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

struct couleur int32tocouleur(uint32_t entier)
{
    struct couleur res;
    /* On utilise des masques pour extraire les composantes RGB */
    res.R = (entier >> 16) & 0xFF;
    res.G = (entier >> 8) & 0xFF;
    res.B = entier & 0xFF;
    return res;
}

void create_image_ppm(uint32_t *pic, int32_t size_x, int32_t size_y, char *filename)
{
    /* Création du fichier */
    FILE *fic = fopen(filename, "w");
    /* Ecriture de l'en-tête ppm */
    fprintf(fic, "P6\n");                                                      
    fprintf(fic, "%d %d\n", size_x, size_y);                       
    fprintf(fic, "%i\n", 255); 
    /* Génération de l'image */
    uint32_t *i32pixel = pic;
    /* On boucle sur tous les pixels */
    for (int32_t i = 0; i < size_x * size_y; i++)
    {
        /* On récupère les couleurs et on écrit le pixel en binaire */
        struct couleur pixel = int32tocouleur(*i32pixel);
        uint8_t couleurs[] = {pixel.R, pixel.G, pixel.B};
        fwrite(couleurs, 1, 3, fic);
        i32pixel++;
    }
    fclose(fic);
}
