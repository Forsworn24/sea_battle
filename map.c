#include "ships.h"

char *buffer_message(t_player *p)
{
    char *message;
    int i, j, k;

    message = (char *)malloc(MAXLINE * sizeof(char));
    i = 0;
    j = 0;
    k = 0;

    // message
    // map(100 символов) + id (1 символ)
    

    // message[i] = '\n';
    // i++;
    message[i] = p->id;
    i++;

    // Заполнение данных о карте
    while (j < MAP_SIZE)
    {
        k = 0;
        while (k < MAP_SIZE)
        {
            message[i] = p->map[j][k];
            k++;
            i++;
        }
        // message[i] = '\n';
        j++;
    }
    
    return message;
}

void print_player_info(t_player *p)
{
    int i;
    int j;


    printf("Данные об игроке: \n");
    printf("Уникальный номер:\n%d\n", p->id);
    printf("Карта игрока:\n");
    i = 0;
    j = 0;
    while (i < MAP_SIZE)
    {
        j = 0;
        while (j < MAP_SIZE)
        {
            printf("%c", p->map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
    printf("\n");
}

t_player *init_player()
{
    t_player *p;
    int i;

    i = 0;
    p = malloc(sizeof(t_player));
    p->map = (char**)malloc(MAP_SIZE * sizeof(char *));
    while (i < MAP_SIZE)
    {
        p->map[i] = (char*)malloc(MAP_SIZE * sizeof(char));
        i++;
    }

    return p;
}

void fill_data(t_player *p, char pid)
{
    int i;
    int j;

    i = 0;
    while (i < MAP_SIZE)
    {
        j = 0;
        while (j < MAP_SIZE)
        {
            p->map[i][j] = '.';
            j++;
        }
        i++;
    }

    p->id = pid;
}

void free_data(t_player *p)
{
    int i;

    i = 0;
    while(i < MAP_SIZE)
    {
        free(p->map[i]);
        i++;
    }

    free(p->map);
    free(p);
}
