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
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            printf("%c", map[10 * j + i]);
        }
        printf("\n");
    }
}

int main() {
    char buffer[MAXLINE];
    char start_game[MAXLINE] = "Игра началась!\nВаши данные:\n";
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
            fill_data(p1, '1');
            buf_params1 = buffer_message(p1);
            print_player_info(p1);
        }
        else if (connect_players == 2)
        {
            memcpy(&reader_addr2, &cliaddr, sizeof(cliaddr));
            printf("Заполнена память для игрока с портом: %d\n", cliaddr.sin_port);
            fill_data(p2, '2');
            buf_params2 = buffer_message(p2);
            print_player_info(p2);
        }

        if (connect_players == 2)
        {
            printf("Кол-во подключений: %d\n", connect_players);
            printf("Отправка сообщений о начале игры!\n");
            printf("Отправляем следующее сообщение: %s\n", start_game);

            // Сообщения о начале игры - a
            // sendto(network_context->sock,
            //     (const char *)start_game,
            //     strlen(start_game),
            //     0,
            //     (const struct sockaddr *) &reader_addr1,
            //     sizeof(reader_addr1));

            // sendto(network_context->sock,
            //     (const char *)start_game,
            //     strlen(start_game),
            //     0,
            //     (const struct sockaddr *) &reader_addr2,
            //     sizeof(reader_addr2));

            // Сообщения о параметрах игроков - b
            sendto(network_context->sock,
                (const char *)buf_params1,
                strlen(buf_params1),
                0,
                (const struct sockaddr *) &reader_addr1,
                sizeof(reader_addr1));
            sendto(network_context->sock,
                (const char *)buf_params2,
                strlen(buf_params2),
                0,
                (const struct sockaddr *) &reader_addr2,
                sizeof(reader_addr2));
    }

    free_data(p1);
    free_data(p2);


    return 0;
}
