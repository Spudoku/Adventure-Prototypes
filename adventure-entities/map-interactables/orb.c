#include "orb.h"

Orb orb_singleton = {
  {NULL, NULL, orb_OnCollision, (void *)&orb_singleton, NULL, //entity
    {{0,0}, {0,0}, {0,0},{0,0}}}, //entity.transform
  ORB_DELAY
};
char *intrusiveThoughts[] = {
  "I wonder what's for dinner.\n",
  "Sometimes, I dream about cheese.\n",
  "Hello? Hello hello?\n",
  "head hurty :(\n",
  "Bro... We're so back\n",
  "I can't believe you didn't pick up on the context clues!\n",
  "Yep. Some orb of knowledge this is...\n",
  "I can't see diddly squat through this...\n",

  "...Am I supposed to fight that... thing? I see some sort of... unholy buglike dragon\n",
  "You're telling me a shrimp fried this rice?\n",
  "Thanks for plugging in the printer.\n",
  "You can hear me?\n",
  "This is better than ET. I can feel it\n",
  "I can feel it now. The face  i am making . its :3\n",
  "This orb is emminating an aura of... *shudders* ..unemployment\n",
  "Where is this connected to anyways? The future?\n"
};

void orb_OnCollision(Entity* thisEntity, Entity* otherEntity){
  if(orb_singleton.counter-- < 0){
    orb_singleton.counter = ORB_DELAY;
    printf(intrusiveThoughts[POKEY_READ.random & 15]);
  }
}