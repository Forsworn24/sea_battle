#include "ships.h"

int main() {
    t_player *p1;

    p1 = init_player();
    fill_data(p1, 1);
    print_player_info(p1);
    free_data(p1);

    return 0;
}
