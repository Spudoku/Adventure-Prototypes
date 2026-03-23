#ifndef CAMERA_H_
#define CAMERA_H_

#define DEFAULT_MARGIN 8

#include <stdbool.h>
#include "entity.h"
#include "util/transform.h"
#include "../../core-gfx/gfx.h"

//macros to make life easier
#define _TrackedObject cameraEntity.childEntity
#define _screenResolution camera.screenResolution 
#define _cameraMargin camera.draggingMargin




typedef struct Camera{
  Entity cameraEntity;
  //entity* _trackedobject (just pretend its here due to preprocessor)


  Vector2 centerPoint;
  //TODO: consider making these a compile time constant? if the compiler doesnt already
  //preprocessor directives WONT do that probably
  u16Vector2 innerMargin; //calced at init

  int8_t draggingMargin; //may make this a compile time thing?


  //rendering data (make into own struct?)
  uint8_t* tileMap;
  uint8_t* displayListLocation;

  

} Camera;

extern Camera camera;

//note that childEntity is being used as the Target entity to track
//idealy id like to have a way to point to it (preprocessor stuff?) from this level instead of
//camera.cameraEntity.childEntity 


//cam pos is in the top left of the displayed screen, so 0,0 in eyecoords is the cam



STATUS cameraConstructor(Entity *toTrack);  //pass NULL to not track, will default to 0,0
void setTrackedEntity(Entity *toTrack); //pass NULL to stop tracking
bool objectVisible(Transform *toCheck); //determines if the transform would be visible in eyeSpace
void marginalTrack();

STATUS ObjectInsideMargin(Transform *toCheck);
Vector2 objectToMargin(Transform *toCheck);

void MoveTo();


//i would imagine this frametask would be ran NOT anonomously
//probably after gamestate is calc'd, but before rendering



//entity overrides
STATUS camera_FrameTask(Entity* thisEntity);

//camera isnt a typical world obj so it doesnt need rendering
//unsure if i will make the camera move all the gfx data yet
STATUS camera_renderer(Entity* thisEntity); 

Vector2 convertToEyeCoords(Vector2 toConvert);


#endif