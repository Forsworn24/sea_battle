#include "ships.h"

int check_ship(int c_ships)
{
    if (c_ships == 1)
        return 1;
    if (c_ships == 2)
        return 2;
    if (c_ships == 3)
        return 3;
    if (c_ships == 4)
        return 4;
}

char *input_ship(int count_ships)
{
    int i = 0;
    char *ship;
    char user_input[10];
    int count;

    ship = (char *)malloc(count_ships * SHIP_SIZE * sizeof(char) + 1);
    count = check_ship(count_ships);
    while (count_ships > 0)
    {
        scanf("%s", user_input);
        // printf("BEFORE_I = %d\n", i);
        while (i < count * 4)
        {
            ship[i] = user_input[i % 4];
            i++;
            printf("%d\n", i);
            if (i % 4 == 0)
                break;
        }
        // printf("AFTER_I = %d\n", i);
        // printf("CURRENT_SHIP = %s\n", ship);
        count_ships--;
    }
    ship[count * 4] = '\0';

    // if (count == 1)
    //     t->four = strdup(ship);
    // else if (count == 2)
    //     t->three = strdup(ship);
    // else if (count == 3)
    //     t->two = strdup(ship);
    // else if (count == 4)
    //     t->one = strdup(ship);
    // else
    //     printf("BAD INPUT");

    printf("SHIP = %s\n", ship);
    return ship;
}

void placeholder(char *buffer, t_ships *t)
{
    int four, three, two, one;
    // char user_input[10];

    four = 1;
    three = 2;
    two = 3;
    one = 4;
    // Заполняем айдишник
    printf("Айди игрока: %c\n", buffer[0]);
    t->id = buffer[0];

    // Четырёхпалубные корабли
    printf("Пример заполнения: (x/y l/r A1) - xrA1\n");
    printf("Заполняем четырехпалубные корабли:  %d\n", four);
    // while (four > 0)
    // {
    //     printf("Введите:\n");
    //     scanf("%s", user_input);
    //     // Проверка данных на кооректность
    //     // НАПИСАТЬ!!!
    //     // Условно берем так, что мы всегда ставим корабли правильно!
    //     strncpy(t->four, user_input, 4);
    //     t->four[4] = '\0';
        
    //     four--;
    // }
    t->four = strdup(input_ship(1));
    printf("Четырёхпалубник заполнен!\n");
    

    // Трёхпалубные корабли
    printf("Заполняем трехпалубные корабли: \n");
    t->three = strdup(input_ship(2));
    // int i = 0;
    // while (three > 0)
    // {
    //     scanf("%s", user_input);
    //     // Проверка данных на кооректность
    //     // НАПИСАТЬ!!!
    //     // Условно берем так, что мы всегда ставим корабли правильно!
    //     // t->three
    //     // _ _ _ _ _ _ _ _ _
    //     // x r A 1 _ _ _ _ _
    //     // x r B 5 _ _ _ _ _
    //     // x r A 1 x r B 5 \0
    //     // printf("CUR_USER_INPUT = %s\n", user_input);
    //     // printf("BEFORE_I = %d\n", i);
    //     while (i < 8)
    //     {
    //         t->three[i] = user_input[i % 4];
    //         i++;
    //         if (i % 4 == 0)
    //             break;
    //     }
    //     // printf("AFTER_I = %d\n", i );
    //     // printf("I_TREE = %s\n", input_three);
    //     // strncpy(t->three, user_input, 4);
    //     three--;
    // }
    // t->three[8] = '\0';
    // // printf("LEN THREE %ld\n", strlen(t->three));
    printf("Трехпалубники заполнены заполнены!\n");

    printf("Заполняем two корабли:\n");
    t->two = strdup(input_ship(3));
    printf("Two заполнен!\n");

    printf("Заполняем ont корабли:\n");
    t->one = strdup(input_ship(4));
    printf("One заполнен!\n");
}

void keep_map(char *buffer)
{
    t_ships *t;
    char *buf;
    // 1. Вывести правила заполнения на экран
    // 2. Заполнить нашу карту кораблями - заполнятор карты
    // 3. Обратно отправить серверу нашу заполненную карту ?
    printf("Выделение памяти для структуры кораблей...\n");

    // + где то нужно очистить от этого память
    t = init_ships();


    printf("Правила заполнения карты. \n");
    printf("Для постановки кораблей укажите следующее: \n");
    printf("Параметры корабля: (гориз/вертик лево/право клетка_постановки )\n");
    printf("Пример:		(x/y l/r A1)\n");

    buf = strdup(buffer);
    printf("OUR BUFFER = %s\n", buf);
    

    // buffer[1] - A 1    buffer[11] - B 1
    // buffer[2] - A 2    buffer[12] - B 2
    // buffer[3] - A 3    buffer[13] - B 3 
    // buffer[4] - A 4    buffer[14] - B 4
    // buffer[5] - A 5    buffer[15] - B 5
    // buffer[6] - A 6    buffer[16] - B 6
    // buffer[7] - A 7    buffer[17] - B 7
    // buffer[8] - A 8    buffer[18] - B 8
    // buffer[9] - A 9    buffer[19] - B 9
    // buffer[10] - A 10  buffer[20] - B 10
    
    //     A B C D E F G H I J
    //  1. . . . . . . . . . . 
    //  2. . . . . . . . . . .
    //  3. . . . . . . . . . .
    //  4. . . . . . . . . . .
    //  5. . . . . . . . . . .
    //  6. . . . . . . . . . .
    //  7. . . . . . . . . . .
    //  8. . . . . . . . . . .
    //  9. . . . . . . . . . .
    // 10. . . . . . . . . . .

    //  Как работает заполнятор карты - placeholder
    //  1. Заполняем последовательно: 4x - 1, 3x - 2, 2x - 3, 1x - 4
    //  2. Ждем ввода от пользователя и получаем его
    //  3. a - невозможно поставить корабль, б - ставим корабль и переходим
    // на следующее действие (зацикливаемся пока не расставим все корабли)
    //  4. Как расставили - сообщение об успехе и готовности.

    placeholder(buf, t);

    // Вывод на экран полученных данных
    printf("Итоговые результаты заполнения:\n");
    printf("_______________________\n");
    printf("ID игрока: %c\n", t->id);
    printf("Параметры четырехпалубника: %s\n", t->four);
    printf("Параметры трехпалубников: %s\n", t->three);
    printf("Параметры трехпалубников: %s\n", t->two);
    printf("Параметры трехпалубников: %s\n", t->one);

    // Write function of replacing dots on map
    // ...




}
