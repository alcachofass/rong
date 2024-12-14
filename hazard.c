#include "headers.h"

Entity* createHazard(Position start_pos){
  Entity* newHazard = calloc(1, sizeof(Entity));

  newHazard->pos.y = start_pos.y;
  newHazard->pos.x = start_pos.x;
  newHazard->ch = '#';

  return newHazard;
}