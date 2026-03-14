#include "pmg.h"

void pmg_clear(PMGInstance* pmgInstance){
  memset(pmgInstance, 0, sizeof(PMGInstance));
}