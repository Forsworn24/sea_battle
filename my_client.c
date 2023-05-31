#include "ships.h"

struct _NetworkContext {
  int sock;
  struct sockaddr_in server_addr;
};

typedef struct _NetworkContext NetworkContext;

NetworkContext *network_init(uint16_t port, in_addr_t server_addr)
{
  NetworkContext *ctx = malloc(sizeof(NetworkContext));

  if (!ctx) goto _error1;

  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    perror("socket creation failed");
    goto _error2;
  }
  ctx->sock = sock;

  memset(&ctx->server_addr, 0, sizeof(ctx->server_addr));

  ctx->server_addr.sin_family = AF_INET;
  ctx->server_addr.sin_port = htons(port);
  ctx->server_addr.sin_addr.s_addr = htonl(server_addr);

  return ctx;

_error2:
  free(ctx);
_error1:
  return NULL;
}


ssize_t network_send(const NetworkContext *network_context,
                     const char *buffer,
                     size_t buffer_size)
{
  ssize_t len = sendto(network_context->sock,
                       (const char *) buffer,
                       buffer_size,
                       0,
                       (const struct sockaddr *) &network_context->server_addr,
                       sizeof(network_context->server_addr));
  if (len != buffer_size) {
    perror("Can't send packet");
  }
  return len;
}

ssize_t network_recv(const NetworkContext *network_context,
                     char *buffer,
                     size_t buffer_size)
{
  struct sockaddr_in servaddr;
  socklen_t len = sizeof(servaddr);

  memcpy(&servaddr, &network_context->server_addr, sizeof(servaddr));

  ssize_t n = recvfrom(network_context->sock,
                       (char *) buffer,
                       buffer_size,
                       0,
                       (struct sockaddr *) &servaddr,
                       &len);

  return n;
}

int validate_ship(char *map, char coordinates[3], int type)
{
    // if ()
    return 1;
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

    while (count > 0) 
    {
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

int main() {
    char buffer[MAXLINE];

    NetworkContext *network_context = network_init(PORT, INADDR_LOOPBACK);

    if (!network_context) {
      printf("Ошибка инициализации сети\n");
      return -1;
    }

    int is_writer;
    printf("Для подключения напишите 1:\n");
    scanf("%d", &is_writer);

    // Сообщаем серверу о нашей роли
    uint8_t role_byte = !!is_writer;
    network_send(network_context, (char *) &role_byte, 1);

    char *map;
    while (1)
    {
        // блок инициализации подключений и начала игры
        int n = network_recv(network_context, buffer, MAXLINE - 1);
        if (n != -1)
        {
            buffer[n] = '\0';
            printf("Сервер: %s\n", buffer);
            char *cp_buffer;
            // Заполнение карты кораблями
            // cp_buffer = strdup(buffer);
            printf("Приступаем к заполнению!!!\n");
            // printf("CP_BUFFER = %s\n", cp_buffer);
            map = fill_map();
            // keep_map(cp_buffer);
            // char map_context[102] = {0};
            // map_context[0] = '#';
            // strcpy(map_context + 1, map);
            // network_send(network_context, map, 100);
            // print_map(map);
            // printf("%s\n", map_context);
            network_send(network_context, map, 100);
            break;
        }
    }

    while(1)
    {
        int n = network_recv(network_context, buffer, MAXLINE - 1);
        if (n != -1)
        {
            buffer[n] = '\0';
            printf("Сервер: %s\n", buffer);
            char *buf;
            buf = strdup(buffer);
            if (buf[0] == 'Y')
            {
                char coord[3];
                scanf("%s", coord);
                network_send(network_context, (char *) coord, 3);
            }
            // char *cp_buffer;
            // cp_buffer = strdup(buffer);
        }
    }

    // return map to server
    // network_send(network_context, map, 100);

    return 0;
}
// network_send(network_context, buffer, strlen(buffer));
