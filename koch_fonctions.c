/*
 * Fichier : koch_fonctions.c
 * Description : Trace de fractales geometriques - flocon de koch - generation des points et rendu des lignes
 */

#include "koch_fonctions.h"
#include "create_image.h"

/* Initialisation de la liste chainee koch correspondant au triangle
   de Koch initial */
void init_koch(struct list **koch, uint32_t size, uint32_t segment_length,
        uint32_t nbsides)
{
    /* Calcul de l'angle interne du polugone régulier */
    float angle = (nbsides - 2)*180/nbsides;
    /* Création du premier point : le point gauche de la base du polygone */
    struct list *pt0 = malloc(sizeof(struct list)) ;
    /* Abscisse : taille de l'image / 2 (=le centre) moins taille segment/2 */
    pt0->x = (size - segment_length)/2 ;
    /* Ordonnée : le plus bas possible car base du polygone moins une petite
     * marge pour le triangle qui sera ajouté après */
    pt0->y = (size/2 + (segment_length*cos(PI/nbsides)) / (2*sin(PI/nbsides)));
    struct list *fin = pt0;
    /* Création du second point */
    struct list *pt1 = malloc(sizeof(struct list));
    /* Chaînage de la liste */
    pt0->next = pt1;
    /* Abscisse du point : même raisonnement (autre extrémité de la base) */
    pt1->x = (size + segment_length)/2;
    /* On veut positionner le polygone "à plat" sur l'axe des abscisses donc 
     * le segment [pt0, pt1] est horizontal */
    pt1->y = pt0->y;
        
    /* Une fois les deux premiers points crées nous pouvons créer les 
     * suivants de manière itérative. Pour cela on efectue des rotations et
     * on crée de nouveaux points dans le sens inverse des aiguilles d'une
     * montre. */
    struct list *pt2 = NULL;
    /* On itère pour les n - 2 points qu'il reste à calculer */
    for (uint8_t i = 0; i < nbsides - 2; i++)
    { 
        /* Création du troisième point et mise à jour de suiv */
        pt2 = malloc(sizeof(struct list));
        pt1->next = pt2;
        // Rotation du point 0 autour du point 1 selon l'angle en deg
        pt2->x = ((pt0->x - pt1->x) * cos(angle*PI/180)
                - (pt0->y - pt1->y) * sin(angle*PI/180) + pt1->x);
        pt2->y = ((pt0->y - pt1->y) * cos(angle*PI/180)
                + (pt0->x - pt1->x) * sin(angle*PI/180) + pt1->y);
        /* Mise à jour des pointeurs pour la prochaine itération */
        pt0 = pt1;
        pt1 = pt2;
    }
    /* On boucle la liste cyclique */
    pt2->next = fin;
    /* Retour de l'adresse vers le premier point */
    *koch = fin;
}

/* Initialisation de l'image avec la couleur de fond definie dans les
   arametres */
void init_picture(uint32_t **picture, uint32_t size, uint32_t bg_color)
{
    /* Allocation d'une zone mémoire qui correspond à un tableau d'entiers.
     * Chaque entier représente un pixel de l'image. */
    *picture = malloc(size*size*sizeof(uint32_t));
    /* Pointeur sur la première case du tableau */
    uint32_t *pixel = *picture;
    /* Itération et initialisation de tous les pixels */
    for (uint32_t i = 0; i < size * size; i++)
    {
        pixel[i] = bg_color;
    }
}

/* Calcul de la fractale de Koch apres un nombre d'iterations donne ;
   generation de la liste chainee koch correspondante */
void generer_koch(struct list *koch, uint32_t nb_iterations)
{
    /* A l'avenir on pourra changer l'angle (pas fonctionnel, laisser 60) */
    uint8_t angle = 60;
    /* Nous allons calculer les coordonnées des points intermédiaires.
     * Pour cela nous itérons sur chaque segment de la liste. */
    for (uint16_t i=0; i < nb_iterations; i++)
    {
        /* pta et pte sont les extrémités du segment */
        struct list *pta = koch;
        struct list *pte = koch->next;
        do{
            /* Calcul des 3 nouveaux points */
            /* Allocation de la mémoire nécessaire pour chaque point */
            struct list *ptb = malloc(sizeof(struct list));
            struct list *ptc = malloc(sizeof(struct list));
            struct list *ptd = malloc(sizeof(struct list));
            /* Calcul des coordonnées des points */
            /* ptb et pbd sont sur le segment [pta, pte], respectivement au
             * tiers et aux deux tiers de la longueur. */
            ptb->x = pta->x + 2 * (int16_t)(pte->x - pta->x)/3;
            ptb->y = pta->y + 2 * (int16_t)(pte->y - pta->y)/3;
            ptd->x = pta->x + (int16_t)(pte->x - pta->x)/3;
            ptd->y = pta->y + (int16_t)(pte->y - pta->y)/3;
            /* ptc est le sommet du nouveau plus petit triangle qui a été
             * ajouté */
            ptc->x = ((int16_t)(ptb->x + ptd->x) * cos(angle * PI/180) 
                    - (int16_t)(ptb->y - ptd->y) * sin(angle * PI/180));
            ptc->y = ((int16_t)(ptb->y + ptd->y) * cos(angle * PI/180)
                    + (int16_t)(ptb->x - ptd->x) * sin(angle * PI/180));
            /* Ajout des points b,c,d dans la liste chaînée */
            pta->next = ptd;
            ptb->next = pte;
            ptc->next = ptb;
            ptd->next = ptc;
            /* Mise à jour des pointeurs du segment pour iteration suivante */
            pta = pte;
            pte = pte->next;
            /* Quand on bouclé sur notre liste circulaire on a fini */
        }while (pta != koch);
    }
}

/* Rendu image via algorithme bresehem - version generalisee
   simplifiee */
void render_image_bresenham(uint32_t *picture, struct list *koch, uint32_t size,
        uint32_t fg_color)
{
    /* Pointeur vers le tableau dans lequel seront stockés les pixels */
    uint32_t *pixel = picture;
    /* On initialise le pointeur à partir duquel on parcourt la liste */
    struct list *point = koch;
    do{
        uint16_t x0 = point->x;
        uint16_t y0 = point->y;
        uint16_t x1 = point->next->x;
        uint16_t y1 = point->next->y;

        uint16_t dx = abs(x1 - x0);
        uint16_t dy = abs(y1 - y0);
        
        int16_t err = dx - dy;
        int16_t err2 = 0;

        int8_t sx = x0 < x1 ? 1 : -1;
        int8_t sy = y0 < y1 ? 1 : -1;

        while ( (x0 != x1) || (y0 != y1))
        {
            /* Tracé du pixel (x0, y0) */
            pixel[x0 + y0*size] = fg_color;
            err2 = err * 2;
            if (err2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }
            if (err2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        }
        point = point->next;
    }while(point != koch);
}

/* Liberation de la memoire allouee a la liste chainee */
void free_koch(struct list *koch)
{
    // Itérer sur les points pour les libérer un à un
    struct list *point = koch;
    do{
        struct list *suivant = point->next;
        free(point);
        point = suivant;
    }while(point != koch);
}

