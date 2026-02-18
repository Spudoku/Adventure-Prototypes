#include "camera.h"


STATUS cameraConstructor(Entity *toTrack){
    camera.draggingMargin = DEFAULT_MARGIN;
    camera.screenResolution.x = 160;
    camera.screenResolution.y = 96;


    //for later
    //entityConstructor(&camera.cameraEntity, camera_FrameTask, camera_renderer);
    entityConstructor(&camera.cameraEntity, camera_FrameTask, NULL);

    //put back-ref into entity

    camera.cameraEntity.entityData = &camera;


    if(camera._TrackedObject != NULL){
      //WARNING WILL ASSIGN BLINDLY, you better make it null!
      camera._TrackedObject = toTrack;
      camera.cameraEntity._worldCoords.x = camera._TrackedObject->_worldCoords.x;
      camera.cameraEntity._worldCoords.y = camera._TrackedObject->_worldCoords.y;
      return PASS;
    }

    camera.cameraEntity._worldCoords.x = camera.screenResolution.x / 2;
    camera.cameraEntity._worldCoords.y = camera.screenResolution.y / 2;


    return PASS;
} 

void setTrackedEntity(Entity *toTrack){
    if(camera._TrackedObject != NULL){
      camera._TrackedObject = toTrack;
    }
    
    camera._TrackedObject = NULL;
}

//checks against eye coords, assumes its updated
//to do, may need to have each entity update its eyecoords?
//may also need to consider lazy checks for stuff super far away
STATUS objectVisible(Transform *toCheck){
  //bounding box checks

  //will consider the following later :
  //if(!toCheck.active if i dont just pass the entity and have that store that) 
  // {return FAIL;}
  
  //Note: i bet this may be screwy with negatives


  //less to more computationally intensive
  //TODO: this is probably not efficient
  //check the left bound
  if(toCheck->eyeCoords.x > _screenResolution.x) {
    return FAIL;
  }

  if(toCheck->eyeCoords.y > _screenResolution.y) {
    return FAIL;
  }

  //checking if the right side of it is visible

  if(toCheck->eyeCoords.x + toCheck->objectBounds.x < 0){
    return FAIL;
  }

  if(toCheck->eyeCoords.y + toCheck->objectBounds.y < 0) {
    return FAIL;
  }

  return PASS;

}

//passes by value to get a free return copy
//TODO: fix lol
Vector2 convertToEyeCoords(Vector2 toConvert){
  toConvert.x -= camera.cameraEntity._worldCoords.x;
  toConvert.y -= camera.cameraEntity._worldCoords.y;
  return toConvert;
}