/*
 * File : koch_fonctions.c
 * Description : traces geometric fractals - koch snowflake.
 * Computes the points and lines of the fractal.
 */

#include "koch_fonctions.h"
#include "create_image.h"

/* Initialization of the linked list corresponding to the first iteration
 * To do so we need to place the first point correctly in order to have a
 * centered image.
 * We need to know the internal angle of the polygon in order to perform the
 * correct rotations.*/
void init_koch(struct list **koch, uint32_t size, uint32_t segment_length,
        uint32_t nbsides)
{
    float angle = (nbsides - 2)*180/nbsides;
    /* First line : pt0 to pt1 */
    struct list *pt0 = malloc(sizeof(struct list)) ;
    /* x axis : we want the line to be centered */
    pt0->x = (size - segment_length)/2 ;
    /* y axis : as low as possible, minus room for the next iterations */
    pt0->y = (size/2 + (segment_length*cos(PI/nbsides)) / (2*sin(PI/nbsides)));
    struct list *fin = pt0;

    struct list *pt1 = malloc(sizeof(struct list));
    pt0->next = pt1;
    pt1->x = (size + segment_length)/2;
    pt1->y = pt0->y;

    /* Once the two first points have been placed we can create the (n-2) next
     * ones with a loop. To do so we perform rotations, placing the new points
     * counterclockwise. */
    struct list *pt2 = NULL;
    for (uint8_t i = 0; i < nbsides - 2; i++)
    {
        pt2 = malloc(sizeof(struct list));
        pt1->next = pt2;
        /* Rotation */
        pt2->x = ((pt0->x - pt1->x) * cos(angle*PI/180)
                - (pt0->y - pt1->y) * sin(angle*PI/180) + pt1->x);
        pt2->y = ((pt0->y - pt1->y) * cos(angle*PI/180)
                + (pt0->x - pt1->x) * sin(angle*PI/180) + pt1->y);
        /* Updating pointers */
        pt0 = pt1;
        pt1 = pt2;
    }
    /* We loop the circular list */
    pt2->next = fin;
    /* Returning pointer to the first point */
    *koch = fin;
}

/* Allocating the memory to store the image and setting the background color
 * The image is stored as an array. Each integer represents a pixel. */
void init_picture(uint32_t **picture, uint32_t size, uint32_t bg_color)
{
    /* Creating the image array and keeping a pointer to the first element */
    *picture = malloc(size*size*sizeof(uint32_t));
    uint32_t *pixel = *picture;
    /* Initialization of the pixels. Some of them will be overwritten by the
     * fractal later. */
    for (uint32_t i = 0; i < size * size; i++)
    {
        pixel[i] = bg_color;
    }
}

/* Computing the fractal for a given number of iterations. The result is added
 * to the linked list. At each iteration we add some triangles (regardless
 * of the original shape) */
void generer_koch(struct list *koch, uint32_t nb_iterations)
{
    /* It isn't yet possible to change the angle (keep it at 60) */
    uint8_t angle = 60;
    /* Lets add the 3 new points to form a triangle on each segment of the
     * current figure. They will be ptb, ptc and ptd. */
    for (uint16_t i=0; i < nb_iterations; i++)
    {
        /* pta and pte are the endpoints of the segment */
        struct list *pta = koch;
        struct list *pte = koch->next;
        do{
            struct list *ptb = malloc(sizeof(struct list));
            struct list *ptc = malloc(sizeof(struct list));
            struct list *ptd = malloc(sizeof(struct list));
            /* ptb and pbd are on the segment [pta, pte], respectively at the
             * third and two thirds of it length. */
            ptb->x = pta->x + 2 * (int16_t)(pte->x - pta->x)/3;
            ptb->y = pta->y + 2 * (int16_t)(pte->y - pta->y)/3;
            ptd->x = pta->x + (int16_t)(pte->x - pta->x)/3;
            ptd->y = pta->y + (int16_t)(pte->y - pta->y)/3;
            /* ptc is the apex of the triangle */
            ptc->x = ((int16_t)(ptb->x + ptd->x) * cos(angle * PI/180)
                    - (int16_t)(ptb->y - ptd->y) * sin(angle * PI/180));
            ptc->y = ((int16_t)(ptb->y + ptd->y) * cos(angle * PI/180)
                    + (int16_t)(ptb->x - ptd->x) * sin(angle * PI/180));
            /* Adding the points b,c,d in the linked list */
            pta->next = ptd;
            ptb->next = pte;
            ptc->next = ptb;
            ptd->next = ptc;
            /* Updating pointers for the next iteration */
            pta = pte;
            pte = pte->next;
        }while (pta != koch);
    }
}

/* To render the image we connect the points with lines by using the simplified
 * Bresenham algorithm. */
void render_image_bresenham(uint32_t *picture, struct list *koch, uint32_t size,
        uint32_t fg_color)
{
    /* Pointer to the array in which the pixels will be stored */
    uint32_t *pixel = picture;
    /* Initilization of a pointer from which we go through the list */
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

/* Freeing the memory allocated to the linked list */
void free_koch(struct list *koch)
{
    struct list *point = koch;
    do{
        struct list *suivant = point->next;
        free(point);
        point = suivant;
    }while(point != koch);
}
