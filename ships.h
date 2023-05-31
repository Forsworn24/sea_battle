# ifndef SHIPS_H
# define SHIPS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAP_SIZE 10
#define PORT	8080
#define MAXLINE 1024
#define SHIP_SIZE 4

enum Ships {SingleDeck = 4, TwoDeck = 3, ThreeDeck = 2, FourDeck = 1};

// Структура игрока
typedef struct s_player {
    unsigned short id;
    char **map;
} t_player;

// Структура кораблей
typedef struct s_ships {
    char id;
    char *one;
    char *two;
    char *three;
    char *four;
} t_ships;


// Функции работы с картой (заполнением данных)
void print_player_info(t_player *p);
void fill_data(t_player *p, char *buffer);
void free_data(t_player *p);
t_player *init_player();
char *buffer_message(t_player *p);

// Функции для работы с кораблями
t_ships *init_ships();
void free_ships_data(t_ships *t);



void keep_map(char *buffer);
void placeholder(char *buffer, t_ships *t);

#endif
