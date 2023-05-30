#!/bin/bash

gcc -o ser map.c keep_ships.c my_server.c init_ships.c
gcc -o cl map.c keep_ships.c my_client.c init_ships.c

