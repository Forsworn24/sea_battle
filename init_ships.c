#include "ships.h"

t_ships *init_ships()
{
    t_ships *t;
    int i;
    i = 0;

    t = malloc(sizeof(t_ships));
    t->one = (char *)malloc(SingleDeck * SHIP_SIZE * sizeof(char) + 1);
    t->two = (char *)malloc(TwoDeck * SHIP_SIZE * sizeof(char) + 1);
    t->three = (char *)malloc(ThreeDeck * SHIP_SIZE * sizeof(char) + 1);
    t->four = (char *)malloc(FourDeck * SHIP_SIZE * sizeof(char) + 1);

    return t;
}

void free_ships_data(t_ships *t)
{
    free(t->one);
    free(t->two);
    free(t->three);
    free(t->four);
    free(t);
}
