#include "headers.h"

Entity* createPlayer(Position start_pos){
  Entity* newPlayer = calloc(1, sizeof(Entity));

  newPlayer->pos.y = start_pos.y;
  newPlayer->pos.x = start_pos.x;
  newPlayer->ch = '@';

  return newPlayer;
}

