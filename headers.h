#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define GRID_X     50
#define GRID_Y     20

#define PLAYER_X   20
#define PLAYER_Y   40

#define CENTER     25

#define MAX_MISSED       5
#define MAX_PLAYER_UP    2
#define MAX_PLAYER_DOWN  50
#define MAX_PLAYER_LEFT  0
#define MAX_PLAYER_RIGHT 51

#define SCREEN_REFRESH      50000000
#define STARTING_COUNTER   100000000
#define COUNTER_REDUCTION   10000000
#define MIN_NANOSECONDS     50000000
#define REST_THRESHOLD      25000000
#define BONUS_DENOMINATOR        100

typedef struct
{
    int y;
    int x;
} Position;

typedef struct
{
    Position pos;
    char ch;
} Entity;

extern struct timespec timeCounter, screenCounter;

int  checkCollision();
void increaseSpeed();
void updateScreen();

void *handleInput(void * input);
void *timedDrop(void * varg);
void *drawScreen(void * varg);
void *keyListen(void * varg);

Entity* createPlayer(Position start_pos);
Entity* createHazard(Position start_pos);

extern Entity* player;
extern Entity* hazard;
extern int x;
extern int y;
extern int kch;
extern int captured;
extern int missed;
extern int stopFlag;
extern int pauseFlag;
extern int maxMissCount;

extern pthread_mutex_t lock;

#endif