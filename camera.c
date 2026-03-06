#include "camera.h"

Camera camera;

Vector2 worldCoordView;
Vector2 debugEyeView;

STATUS cameraConstructor(Entity *toTrack){
    camera.draggingMargin = DEFAULT_MARGIN; 
    camera.screenResolution.x = 160;
    camera.screenResolution.y = 96;

    camera.innerMargin.x = _screenResolution.x - _cameraMargin * 2;
    camera.innerMargin.y = _screenResolution.y - _cameraMargin * 2;
    camera.centerPoint.x = _screenResolution.x/2;
    camera.centerPoint.y = _screenResolution.y/2;


    //for later
    //entityConstructor(&camera.cameraEntity, camera_FrameTask, camera_renderer);
    entityConstructor(&camera.cameraEntity, camera_FrameTask, camera_renderer);

    //put back-ref into entity

    camera.cameraEntity.entityData = &camera;


    if(toTrack != NULL){
      //WARNING WILL ASSIGN BLINDLY, you better make it null!
      camera._TrackedObject = toTrack;
      // camera.cameraEntity._worldCoords.x = camera._TrackedObject->_worldCoords.x;
      // camera.cameraEntity._worldCoords.y = camera._TrackedObject->_worldCoords.y;
      return PASS;
    }

    // camera.cameraEntity._worldCoords.x = camera.screenResolution.x / 2;
    // camera.cameraEntity._worldCoords.y = camera.screenResolution.y / 2;


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




STATUS camera_FrameTask(Entity* thisEntity){
  Vector2 offset;

  if(camera._TrackedObject == NULL ){
    return PASS;
  }


    //refresh tracked entity's eyecoord, they are priveleged
    //this does mean the entity may refresh it's eyecoord twice tho

  camera._TrackedObject->_eyeCoords = 
    convertToEyeCoords(camera._TrackedObject->_worldCoords);


  //margin move,
  if(ObjectInsideMargin(&camera._TrackedObject->transform) == FAIL) 
  {
    





    //hopefully it actually adds negatives correctly...
    offset = objectToMargin(&camera._TrackedObject->transform);
    camera.cameraEntity._worldCoords.x += offset.x;
    camera.cameraEntity._worldCoords.x += offset.x;
  }

  //basic centering for debug
  // if(camera._TrackedObject != NULL) {
  //   camera.cameraEntity._worldCoords.x = camera._TrackedObject->_worldCoords.x - camera.centerPoint.x;
  //   camera.cameraEntity._worldCoords.y = camera._TrackedObject->_worldCoords.y - camera.centerPoint.y;
  //   worldCoordView = thisEntity->_worldCoords; 
  // }

  debugEyeView = camera._TrackedObject->_eyeCoords;
  return PASS;
}



//bases on top left for now (calc center later?)
STATUS ObjectInsideMargin(Transform *toCheck) {
  //simplify later, premature optimization bad
  //there has to be a more optimal trick...
  //TODO: if the memory is there, can the margin result be calced and stored?


  //6502 does have hardware comp

  //check the likely option, its within
  //NOTE: i bet some weird casting issue will happen here between u16,s32
  if(toCheck->eyeCoords.x < camera.draggingMargin) return FAIL;

  if(toCheck->eyeCoords.y < camera.draggingMargin) return FAIL;

  if(toCheck->eyeCoords.x > camera.innerMargin.x)
    return FAIL;

  if(toCheck->eyeCoords.y > camera.innerMargin.y)
    return FAIL;

  return PASS;
}

//calculate the minimum relative displacement that would put the object within
//the margin, used for moving the camera
// //its possible this may utilize less cycles than comping then subtracting
Vector2 objectToMargin(Transform *toCheck){
  Vector2 result;

  if(toCheck->eyeCoords.x > camera.centerPoint.x) {
    //right side is closer, get x dist to that
    result.x = toCheck->eyeCoords.x - camera.innerMargin.x;
  } else {
    //left side is closer, eyecoords will be negative
    result.x = toCheck->eyeCoords.x - camera.draggingMargin;
  }

  if(toCheck->eyeCoords.y > camera.centerPoint.y) {
    result.y = toCheck->eyeCoords.y - camera.innerMargin.y;
  } else {
    result.y = toCheck->eyeCoords.y - camera.draggingMargin;
  }
  
  return result;
}





// the renderer stuff. it shifts the playfield according to camera data
STATUS camera_renderer(Entity* thisEntity){


  map_fastAbsoluteMove(camera.cameraEntity._worldCoords);

  return PASS;
} 