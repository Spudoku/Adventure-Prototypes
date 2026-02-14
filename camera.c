#include "camera.h"


STATUS cameraConstructor(Entity *toTrack){
    camera.draggingMargin = DEFAULT_MARGIN;
    camera.screenResolution.x = 160;
    camera.screenResolution.y = 96;


    //for later
    //entityConstructor(&camera.cameraEntity, camera_FrameTask, camera_renderer);
    entityConstructor(&camera.cameraEntity, camera_FrameTask, NULL);

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


STATUS objectVisible(Transform *toCheck){

}

//passes by value to get a free return copy
Vector2 convertToEyeCoords(u32Vector2 toConvert){
  toConvert.x -= camera.cameraEntity._worldCoords.x;
  toConvert.y -= camera.cameraEntity._worldCoords.y;
  return toConvert;
}