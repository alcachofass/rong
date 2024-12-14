#include "headers.h"

int kch;
struct timespec timeCounter;

void *keyListen (void * varg){       
    while(kch = getch()){
        if (kch == 'q'){
            stopFlag = 1;
            break;
        }        

        handleInput((void *) &kch);
        mvaddch(player->pos.y, player->pos.x, player->ch);
        mvaddch(hazard->pos.y, hazard->pos.x, hazard->ch);

        if (missed == MAX_MISSED){
            updateScreen();
            mvaddstr(20,20, "Game Over");
            refresh();
            stopFlag = 1;
            }

        int collision = checkCollision();

        if (collision == 1) {
            x = rand() % GRID_X + 1;
            y = rand() % (GRID_Y + 1 - 3) + 3;
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

        if (hazard->pos.y == 50){
            hazard->pos.y = 4;
            missed++;
            if (missed == MAX_MISSED){
                updateScreen();
                mvaddstr(20,20, "Game Over");
                refresh();
                stopFlag = 1;
            }
        }
        else{
            hazard->pos.y++;
            int collision = checkCollision();

            if (collision == 1) {
                x = rand() % GRID_X + 1;
                y = rand() % (GRID_Y + 1 - 3) + 3;

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
        nanosleep(&timeCounter, NULL);
    }
    
    return NULL;
}

void updateScreen (){
        char cap[3];
        char mis[3];
        char spd[20];
        
        sprintf(cap, "%d", captured );
        sprintf(mis, "%d", missed);
        sprintf(spd, "%d", timeCounter.tv_nsec);

        clear();        
        mvaddch(player->pos.y, player->pos.x, player->ch);
        mvaddch(hazard->pos.y, hazard->pos.x, hazard->ch);

        mvaddstr(0,0,  "Captured: ");
        mvaddstr(1,0,  "Missed: ");
        mvaddstr(2,0,  "Wait(ns): " );

        mvaddstr(51,0, "Press 'q' to quit.");
        mvaddstr(52,0, "RONG by alcachofass - https://github.com/alcachofass");
        mvaddstr(0,10, cap);
        mvaddstr(1,10, mis);
        mvaddstr(2,10, spd);

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
            updateScreen();
            break;
        case KEY_DOWN:
            player->pos.y++;
            updateScreen();
            break;
        case KEY_LEFT:
            player->pos.x--;
            updateScreen();
            break;
        case KEY_RIGHT:
            player->pos.x++;
            updateScreen();
            break;
        default:
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

