CC			=		cc
FLAGS		=		 #-Wall -Werror -Wextra
RM			=		rm -f

CLIENT =			client
SERVER =			server

HEADER =			-I ../ships.h

UTILS_DIR = utils/
UTILS_OBJ_DIR = $(UTILS_DIR)obj/

CLIENT_DIR =		client_src/
CLIENT_OBJ_DIR =	$(CLIENT_DIR)obj/

SERVER_DIR =		server_src/
SERVER_OBJ_DIR =	$(SERVER_DIR)obj/

## Сюда исходники добавлять

SRC_CLIENT_FILES =	client
SRC_SERVER_FILES =	server map
SRC_UTILS_FILES =	test_utils

SRCCL = $(addprefix $(CLIENT_DIR), $(addsuffix .c, $(SRC_CLIENT_FILES)))
OBJCL = $(addprefix $(CLIENT_OBJ_DIR), $(addsuffix .o, $(SRC_CLIENT_FILES)))

SRCSV = $(addprefix $(SERVER_DIR), $(addsuffix .c, $(SRC_SERVER_FILES)))
OBJSV = $(addprefix $(SERVER_OBJ_DIR), $(addsuffix .o, $(SRC_SERVER_FILES)))

SRCUTILS = $(addprefix $(UTILS_DIR), $(addsuffix .c, $(SRC_UTILS_FILES)))
OBJUTILS = $(addprefix $(UTILS_OBJ_DIR), $(addsuffix .o, $(SRC_UTILS_FILES)))

all: start $(OBJUTILS) $(CLIENT) $(SERVER)

$(CLIENT): $(OBJCL)
	$(CC) $(FLAGS) $(OBJCL) $(OBJUTILS) $(HEADER) -o $(CLIENT)

$(CLIENT_OBJ_DIR)%.o: $(CLIENT_DIR)%.c
	$(CC) $(FLAGS) $(HEADER) -c $< -o $@


$(SERVER): $(OBJSV)
	$(CC) $(FLAGS) $(OBJSV) $(OBJUTILS) $(HEADER) -o $(SERVER)

$(SERVER_OBJ_DIR)%.o: $(SERVER_DIR)%.c
	$(CC) $(FLAGS) $(HEADER) -c $< -o $@

$(UTILS_OBJ_DIR)%.o: $(UTILS_DIR)%.c
	$(CC) $(FLAGS) -c $< -o $@

start:
	@mkdir -p $(UTILS_OBJ_DIR)
	@mkdir -p $(CLIENT_OBJ_DIR)
	@mkdir -p $(SERVER_OBJ_DIR)

clean:
	$(RM) -r $(SERVER_OBJ_DIR)
	$(RM) -r $(CLIENT_OBJ_DIR)
	$(RM) -r $(UTILS_OBJ_DIR)

fclean:		clean
	$(RM) $(SERVER) $(CLIENT)

re:			fclean all

.PHONY:		start all clean fclean re
