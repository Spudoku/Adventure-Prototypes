#include "util_structs.h"

unsigned int distTest;
// calculates taxicab distance (square roots suck)
unsigned int Vector2Dist(Vector2 *a, Vector2 *b) {
    unsigned int xDist;
    unsigned int yDist;

    xDist = max(a->x,b->x) - min(a->x,b->x);
    yDist = max(a->y,b->y) - min(a->y,b->y);

  return xDist + yDist;
}