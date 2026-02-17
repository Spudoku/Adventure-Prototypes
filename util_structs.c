#include "util_structs.h"

// calculates taxicab distance (square roots suck)
unsigned int Vector2Dist(Vector2 a, Vector2 b) {
    unsigned int xDist;
    unsigned int yDist;

    if (a.x > b.x) {
        xDist = a.x - b.x;
    } else {
        xDist = b.x - a.x;
    }

    if (a.y > b.y) {
        yDist = a.y - b.y;
    } else {
        yDist = b.y - a.y;
    }
  return xDist + yDist;
}