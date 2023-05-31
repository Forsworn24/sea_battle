#include "ships.h"

struct _NetworkContext {
  int sock;
  struct sockaddr_in addr;
};

typedef struct _NetworkContext NetworkContext;

NetworkContext *network_init(uint16_t port)
{
  NetworkContext *ctx = malloc(sizeof(NetworkContext));
  if (!ctx) goto _error1;

  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    perror("socket creation failed");
    goto _error2;
  }
  ctx->sock = sock;

  memset(&ctx->addr, 0, sizeof(ctx->addr));

  ctx->addr.sin_family = AF_INET;
  ctx->addr.sin_addr.s_addr = INADDR_ANY;
  ctx->addr.sin_port = htons(port);

  int bind_status = bind(ctx->sock,
                         (const struct sockaddr *) &ctx->addr,
                         sizeof(ctx->addr));
  if (bind_status < 0)
  {
    perror("bind failed");
    goto _error3;
  }

  return ctx;

_error3:
  close(ctx->sock);
_error2:
  free(ctx);
_error1:
  return NULL;
}

void network_fini(NetworkContext *ctx)
{
  if (ctx) {
    if (ctx->sock >= 0) close(ctx->sock);
    free(ctx);
  }
}

void print_map(char *map)
{
    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < 10; ++i) {
        printf("%d ", i);
        for (int j = 0; j < 10; ++j) {
            printf(" %c", map[10 * j + i]);
        }
        printf("\n");
    }
}

int valid_shot(char *buffer)
{
    return 1;
}

int check_shot(t_player *p, char *buffer)
{
    int x, y;

    x = buffer[0] - 'a';
    y = buffer[1] - '0';

    if (p->map[y][x] == '0')
        return 1;
    return 0;
}


char **make_shot(t_player *p, char *buffer)
{
    int x, y;

    x = buffer[0] - 'a';
    y = buffer[1] - '0';

    if (check_shot(p, buffer) == 1)
        p->map[y][x] = 'x';
    else
        p->map[y][x] = '*';
    return p->map;
}

int main() {
    char buffer[MAXLINE];
    char start_game[MAXLINE] = "Игра началась!\nВаши данные:\n";
    char your_turn[MAXLINE] = "Your turn!";
    char wait_message[MAXLINE] = "Wait...";
    int len, n;
    int connect_players = 0;
    struct sockaddr_in cliaddr, reader_addr1, reader_addr2;
    NetworkContext *network_context = network_init(PORT);

    t_player *p1, *p2;

    p1 = init_player();
    p2 = init_player();
    char *buf_params1;
    char *buf_params2;

    if (!network_context) {
      printf("Ошибка инициализации сети\n");
      return -1;
    }

    memset(&cliaddr, 0, sizeof(cliaddr));

    printf("Ждем подключения игроков...\n");

    // Add players with filling their data
    while (1)
    {
        len = sizeof(cliaddr);

        n = recvfrom(network_context->sock,
                   (char *) buffer,
                   MAXLINE,
                   MSG_WAITALL,
                   (struct sockaddr *) &cliaddr,
                   &len);

        if (n == 1)
        {
            printf("Игрок 1 подключен, ждем подключения Игрока 2!\n");
            printf("Параметры первого игрока:\n");
            printf("%d \n", cliaddr.sin_port);
            connect_players += 1;
        }

        if (connect_players == 1)
        {
            memcpy(&reader_addr1, &cliaddr, sizeof(cliaddr));
            printf("Заполнена память для игрока с портом: %d\n", cliaddr.sin_port);
            p1->id = cliaddr.sin_port;
            printf("Player1 id = %d\n", p1->id);
            // fill_data(p1, '1');
            // buf_params1 = buffer_message(p1);
            // print_player_info(p1);
        }
        else if (connect_players == 2)
        {
            memcpy(&reader_addr2, &cliaddr, sizeof(cliaddr));
            printf("Заполнена память для игрока с портом: %d\n", cliaddr.sin_port);
            p2->id = cliaddr.sin_port;
            printf("Player2 id = %d\n", p2->id);
            // fill_data(p2, '2');
            // buf_params2 = buffer_message(p2);
            // print_player_info(p2);
        }

        if (connect_players == 2)
        {
            printf("Кол-во подключений: %d\n", connect_players);
            printf("Отправка сообщений о начале игры!\n");
            printf("Отправляем следующее сообщение: %s\n", start_game);

            // Сообщения о начале игры - a
            sendto(network_context->sock,
                (const char *)start_game,
                strlen(start_game),
                0,
                (const struct sockaddr *) &reader_addr1,
                sizeof(reader_addr1));

            sendto(network_context->sock,
                (const char *)start_game,
                strlen(start_game),
                0,
                (const struct sockaddr *) &reader_addr2,
                sizeof(reader_addr2));

            // Сообщения о параметрах игроков - b
            // sendto(network_context->sock,
            //     (const char *)buf_params1,
            //     strlen(buf_params1),
            //     0,
            //     (const struct sockaddr *) &reader_addr1,
            //     sizeof(reader_addr1));
            // sendto(network_context->sock,
            //     (const char *)buf_params2,
            //     strlen(buf_params2),
            //     0,
            //     (const struct sockaddr *) &reader_addr2,
            //     sizeof(reader_addr2));
            break;
        }
    }

    int count_fill_players = 0;
    while(1)
    {
        len = sizeof(cliaddr);

        n = recvfrom(network_context->sock,
                   (char *) buffer,
                   MAXLINE,
                   MSG_WAITALL,
                   (struct sockaddr *) &cliaddr,
                   &len);

        if (n != -1)
        {
            printf("Client: %s\n", buffer);
            print_map(buffer);
            char *bf;
            bf = strdup(buffer);
            if (p1->id == cliaddr.sin_port)
            {
                fill_data(p1, bf);
                print_player_info(p1);
                printf("Player 1 was filled\n");
                count_fill_players++;
            }
            else
            {
                fill_data(p2, bf);
                print_player_info(p2);
                printf("Player 2 was filled\n");
                count_fill_players++;
            }

            if (count_fill_players == 2)
              break;
        }
    }

    // Game start
    // p1, p2
    int check_turn = 0;
    while (1)
    {
        // printf("check_turn = %d\n", check_turn);
        if (check_turn == 0 || check_turn == 1)
        {
            sendto(network_context->sock,
                    (const char *)your_turn,
                    strlen(your_turn),
                    0,
                    (const struct sockaddr *) &reader_addr1,
                    sizeof(reader_addr1));

            sendto(network_context->sock,
                    (const char *)wait_message,
                    strlen(wait_message),
                    0,
                    (const struct sockaddr *) &reader_addr2,
                    sizeof(reader_addr2));
        }
        else if (check_turn == 2)
        {
            sendto(network_context->sock,
                    (const char *)your_turn,
                    strlen(your_turn),
                    0,
                    (const struct sockaddr *) &reader_addr2,
                    sizeof(reader_addr2));
            sendto(network_context->sock,
                    (const char *)wait_message,
                    strlen(wait_message),
                    0,
                    (const struct sockaddr *) &reader_addr1,
                    sizeof(reader_addr1));
        }

        while (1)
        {
            // printf("wait message ...\n");
            len = sizeof(cliaddr);

            n = recvfrom(network_context->sock,
                      (char *) buffer,
                      MAXLINE,
                      MSG_WAITALL,
                      (struct sockaddr *) &cliaddr,
                      &len);

            char *buff;
            int check_sh = -1;
            buff = strdup(buffer);
            if (n != -1 && cliaddr.sin_port == p1->id)
            {
                check_sh = check_shot(p2, buff);
                printf("Client1: %s\n", buffer);
                if (valid_shot(buff) == 1)
                {
                    p2->map = make_shot(p2, buff);
                    print_player_info(p2);
                }
                if (check_sh != 1)
                  check_turn = 2;
            }
            else if (n != -1 && cliaddr.sin_port == p2->id)
            {
                check_sh = check_shot(p1, buff);
                printf("Client2: %s\n", buffer);
                if (valid_shot(buff) == 1)
                {
                    p1->map = make_shot(p1, buffer);
                    print_player_info(p1);
                }
                if (check_sh != 1)
                  check_turn = 1;
            }

            if (check_turn != 0)
              break;
        }
    }
    printf("Game was finished ...\n");
    printf("Winner = player id.. \n");
    free_data(p1);
    free_data(p2);


    return 0;
}
