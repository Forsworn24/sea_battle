#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate_ship(char *map, char coordinates[3], int type)
{
    // if ()
    return 1;
}

void print_map(char *map)
{
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            printf("%c", map[10 * j + i]);
        }
        printf("\n");
    }
}

// b2v
void set_ship_to_map(char **map, char coordinates[3], int length)
{
    int x, y;
    char orientation;

    y = coordinates[0] - 'a'; // 2
    x = coordinates[1] - '0'; // 2
    orientation = coordinates[2];

    if (orientation == 'h') {
        while (length > 0) {
            (*map)[10 * (y + length - 1) + x] = '0';
            length--;
        }
    } else {
        while (length > 0) {
            (*map)[10 * y + (x + length - 1)] = '0';
            length--;
        }
    }
}

void fill_ship_type(char **map, int type, int count)
{
    if (type == 1)
        printf("Заполняем однопалубные корабли..\n");
    else if (type == 2)
        printf("Заполняем двухпалубные корабли..\n");
	else if (type == 3)
        printf("Заполняем трехпалубный корабли..\n");
	else if (type == 4)
        printf("Заполняем четырехпалубный корабли..\n");

	while (count > 0) {
        printf("Осталось заполнить %d кораблей.\n", count);
        printf("Введите данные о корабле:\n");

        char coordinates[4];

        // 0-9 a-j v/h
        scanf("%s", coordinates);
        if (validate_ship(*map, coordinates, type) == 1) {
            set_ship_to_map(map, coordinates, type);
            print_map(*map);
            count--;
        }
    }
}

char *fill_map(void)
{
	char *map;

	map = malloc(sizeof(char) * 100);
	memset(map, '.', 100);
	printf("Правила заполнения карты. \n");
	printf("Для постановки кораблей укажите следующее: \n");
	printf("Параметры корабля: (гориз/вертик лево/право клетка_постановки )\n");
	printf("Пример:		(x/y l/r A1)\n");


	// fill_ship_type(&map, 1, 4);
	// fill_ship_type(&map, 2, 3);
	// fill_ship_type(&map, 3, 2);
	fill_ship_type(&map, 4, 1);
	return map;
}
