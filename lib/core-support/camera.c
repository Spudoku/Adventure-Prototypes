#include "camera.h"
#pragma optimize(on)
#pragma static-locals(on)
//#include <stdio.h>

Camera camera;

// working variables; suggested by AI lmao
static int16_t temp_x, temp_y, temp_obj_bounds_x, temp_obj_bounds_y;



STATUS cameraConstructor(Entity *toTrack){
    camera.draggingMargin = DEFAULT_MARGIN; 

    camera.innerMargin.x = SCR_RES_X - _cameraMargin;
    camera.innerMargin.y = SCR_RES_Y - _cameraMargin;


    // call constructor
    entityConstructor(&camera.cameraEntity, camera_FrameTask, camera_renderer);

    //put back-ref into entity
    camera.cameraEntity.entityData = &camera;


    if(toTrack != NULL){
      //WARNING WILL ASSIGN BLINDLY, you better make it null!
      camera._TrackedObject = toTrack;
      camera_FrameTask(&camera.cameraEntity);
    }

    map_absoluteMove(camera.cameraEntity._worldCoords);

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
bool objectVisible(Transform *toCheck){

  temp_x = toCheck->eyeCoords.x;
  temp_y = toCheck->eyeCoords.y;
  //bounding box checks

  //Note: i bet this may be screwy with negatives


  //less to more computationally intensive
  //TODO: this is probably not efficient
  //check the left bound
  if(temp_x > SCR_RES_X) {
    return false;
  }

  if(temp_y > SCR_RES_Y) {
    return false;
  }

  if(temp_x < 1){
    return false;
  }

  if(temp_y < 1) {
    return false;
  }

  return true;

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


    //hopefully it actually adds negatives correctly...
  offset = objectToMargin(&camera._TrackedObject->transform);
  camera.cameraEntity._worldCoords.x += offset.x;
  camera.cameraEntity._worldCoords.y += offset.y;
  

  return PASS;
}



//bases on top left for now (calc center later?)
STATUS ObjectInsideMargin(Transform *toCheck) {

  temp_x = toCheck->eyeCoords.x;
  temp_y = toCheck->eyeCoords.y;


  //simplify later, premature optimization bad
  //there has to be a more optimal trick...
  //TODO: if the memory is there, can the margin result be calced and stored?


  //6502 does have hardware comp

  //check the likely option, its within
  //NOTE: i bet some weird casting issue will happen here between u16,s32
  if(temp_x < camera.draggingMargin) return FAIL;

  if(temp_y < camera.draggingMargin) return FAIL;

  if(temp_x > camera.innerMargin.x) return FAIL;

  if(temp_y > camera.innerMargin.y) return FAIL;

  return PASS;
}

//calculate the minimum relative displacement that would put the object within
//the margin, used for moving the camera
//TODO: please make faster, takes up 6-7% of frame time
Vector2 objectToMargin(Transform *toCheck){
  Vector2 result = {0, 0};

  // using static variables to reduce pointer lookups
  temp_x = toCheck->eyeCoords.x;
  temp_y = toCheck->eyeCoords.y;

  
  // if temp_x > SCR_RES_X - _cameraMargin
  if(temp_x  > camera.innerMargin.x) {
    //right side is closer, get x dist to that
    result.x = temp_x - camera.innerMargin.x;
  } else if (temp_x < camera.draggingMargin){
    //left side is closer, eyecoords will be negative
    result.x = temp_x - camera.draggingMargin;
  }


  if(temp_y > camera.innerMargin.y) {
    //bottom is closer, get x dist to that
    result.y = temp_y - camera.innerMargin.y;
  } else if (temp_y < camera.draggingMargin){
    //top side is closer, eyecoords will be negative
    result.y = temp_y - camera.draggingMargin;
  }
  

  return result;
}





// the renderer stuff. it shifts the playfield according to camera data
STATUS camera_renderer(Entity* thisEntity){


  map_fastAbsoluteMove(camera.cameraEntity._worldCoords);

  return PASS;
} 