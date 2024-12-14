
#include "headers.h"

Entity* player;
Entity* hazard;

int collision = 0;
int boundary = 0;

int x;
int y;

int captured = 0;
int missed = 0;

int stopFlag = 0;

pthread_t t_id[4];

int main(int argc, char *argv[]){
 
    timeCounter.tv_nsec = STARTING_COUNTER;
    
    x = rand() % GRID_X + 1;
    y = rand() % (GRID_Y + 1 - 3) + 3;
    
    Position start_pos = { PLAYER_Y, PLAYER_X };
    Position h_pos = { y, x };
    
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    player = createPlayer(start_pos);
    mvaddch(player->pos.y, player->pos.x, player->ch);

    hazard = createHazard(h_pos);
    mvaddch(hazard->pos.y, hazard->pos.x, hazard->ch);

    updateScreen();

    pthread_create(&t_id[0], NULL, timedDrop, NULL );
    pthread_create(&t_id[1], NULL, drawScreen, NULL );
    pthread_create(&t_id[2], NULL, keyListen, NULL );
    pthread_create(&t_id[3], NULL, handleInput, (void *) &kch);
    
    pthread_join(t_id[0], NULL);  
    pthread_join(t_id[1], NULL);
    pthread_join(t_id[2], NULL);
    pthread_join(t_id[3], NULL);

    endwin();

    return 0;
}