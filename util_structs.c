#include "util_structs.h"

unsigned int distTest;
// calculates taxicab distance (square roots suck)
unsigned int Vector2Dist(Vector2 *a, Vector2 *b) {
    unsigned int xDist;
    unsigned int yDist;

    xDist = max(a->x,b->x) - min(a->x,b->x);

    if (a->y > b->y) {
        yDist = a->y - b->y;
    } else {
        yDist = b->y - a->y;
    }
  return xDist;
}