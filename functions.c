#include "headers.h"

int kch;
struct timespec timeCounter, screenCounter;

void *keyListen (void * varg){       
    while(kch = getch()){
        if (kch == 'q'){
            stopFlag = 1;
            break;
        }        

        handleInput((void *) &kch);
        mvaddch(player->pos.y, player->pos.x, player->ch);
        mvaddch(hazard->pos.y, hazard->pos.x, hazard->ch);

        // To keep Game Over text on screen even if keys are pressed
        if (missed == maxMissCount){
            updateScreen();
            mvaddstr(CENTER,CENTER - 3, "Game Over");
            refresh();
            stopFlag = 1;
        }

        int collision = checkCollision();

        if (collision == 1) {
            x = (rand() % (MAX_PLAYER_RIGHT - MAX_PLAYER_LEFT + 1) + MAX_PLAYER_LEFT);
            y = MAX_PLAYER_UP;
            Position h_pos_new = { y, x };
            hazard = createHazard(h_pos_new);
            mvaddch(hazard->pos.y, hazard->pos.x, hazard->ch);
        }
    }

    return NULL;
}

void *timedDrop (void * varg){   
    while (1){
        if (stopFlag == 1) {
            break;
        }
        
        if (timeCounter.tv_nsec <= REST_THRESHOLD){
            timeCounter.tv_nsec = STARTING_COUNTER;
        }

        nanosleep(&timeCounter, NULL);

        if (hazard->pos.y == MAX_PLAYER_DOWN){
            hazard->pos.y = MAX_PLAYER_UP;
            hazard->pos.x = (rand() % (MAX_PLAYER_RIGHT - MAX_PLAYER_LEFT + 1) + MAX_PLAYER_LEFT);
          
            missed++;
            if (missed == maxMissCount){
                updateScreen();
                mvaddstr(CENTER,CENTER - 3, "Game Over");
                refresh();
                stopFlag = 1;
            }
        }
        else{
            hazard->pos.y++;
            int collision = checkCollision();

            if (collision == 1) {
                x = (rand() % (MAX_PLAYER_RIGHT - MAX_PLAYER_LEFT + 1) + MAX_PLAYER_LEFT);
                y = MAX_PLAYER_UP;

                Position h_pos_new = { y, x };
                hazard = createHazard(h_pos_new);
                mvaddch(hazard->pos.y, hazard->pos.x, hazard->ch);
            }
        }
    }

    return NULL;
}

void *drawScreen (void * varg){
    while (1){
        if (stopFlag == 1) {
            break;
        }

        updateScreen();
        nanosleep(&screenCounter, NULL);
    }
    
    return NULL;
}

void updateScreen (){
        clear();        

        char cap[3];
        char mis[3];
        char spd[20];
        
        sprintf(cap, "%d", captured );
        sprintf(mis, "%d", missed);
        sprintf(spd, "%d", timeCounter.tv_nsec);

        mvaddstr(0,0,  "Captured: ");
        mvaddstr(0,10, cap);

        mvaddstr(1,0,  "Missed: ");
        mvaddstr(1,10, mis);

        mvaddstr(0,33, "Wait(ns): " );
        mvaddstr(0,43, spd);

        mvaddstr(0,20, "RONG");

        mvaddch(player->pos.y, player->pos.x, player->ch);
        mvaddch(hazard->pos.y, hazard->pos.x, hazard->ch);

        mvaddstr(51,0, "Press 'q' to quit.");
        mvaddstr(52,0, "RONG by alcachofass - https://github.com/alcachofass");
        
        refresh();

        return;
}

void *handleInput(void * varg)
{
    int input = *((int* ) varg);

    switch(input)
    {
        case KEY_UP:
            player->pos.y--;
            if (player->pos.y < MAX_PLAYER_UP){
                player->pos.y = MAX_PLAYER_UP;
            }
            updateScreen();
            break;
        case KEY_DOWN:
            player->pos.y++;
            if (player->pos.y > MAX_PLAYER_DOWN){
                player->pos.y = MAX_PLAYER_DOWN;
            }
            updateScreen();
            break;
        case KEY_LEFT:
            player->pos.x--;
            if (player->pos.x < MAX_PLAYER_LEFT){
                player->pos.x = MAX_PLAYER_LEFT;
            }
            updateScreen();
            break;
        case KEY_RIGHT:
            player->pos.x++;
            if (player->pos.x > MAX_PLAYER_RIGHT){
                player->pos.x = MAX_PLAYER_RIGHT;
            }
            updateScreen();
            break;
        default:
            updateScreen();
            break;
    }

    return NULL;
}

void increaseSpeed(){
    if ((timeCounter.tv_nsec - COUNTER_REDUCTION) <= 0){
        timeCounter.tv_nsec = MIN_NANOSECONDS;
    }
    else{
        timeCounter.tv_nsec = timeCounter.tv_nsec - COUNTER_REDUCTION; 
    }

    return;
}

int checkCollision(){
    int randomBonus;

    if (stopFlag !=1){
        if (player->pos.y == hazard->pos.y && player->pos.x == hazard->pos.x){
            free(hazard);
            captured++;
            updateScreen(); 
            increaseSpeed();

            randomBonus = MIN_NANOSECONDS/10 + (rand () % ( MIN_NANOSECONDS/BONUS_DENOMINATOR ));
            timeCounter.tv_nsec = timeCounter.tv_nsec + randomBonus;

            return 1;
        }
    }
  
    return 0;
}

